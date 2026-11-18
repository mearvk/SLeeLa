package org.sleela.site.runtime;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.regex.Pattern;

/**
 * Runs one SLeeLa program for a website visitor.
 *
 * Layers, outermost first:
 *   1. SourcePolicy refuses programs (and the library units they pull in) that reach files, processes, network, native code.
 *   2. bubblewrap: new user/pid/ipc/uts/cgroup/net namespaces (no network at all), read-only /usr and SLeeLa tree,
 *      empty tmpfs /tmp, a private writable /work that is deleted afterwards, all capabilities dropped, cleared environment.
 *   3. prlimit: address space, CPU seconds, written file size, open files, no core dumps.
 *   4. Wall-clock deadline here (the whole sandbox is killed), output cap, one run at a time per slot, per-client rate limit.
 * The program name is always the fixed string main.sleela and the arguments are fixed; visitors choose only the file contents
 * and one VM from the fixed VmChoice table. A premium VM is booted with `sleela vm run premium N /work/main.sleela` from the
 * read-only SLeeLa tree (the CLI resolves sleela-virtual-machine/NNN relative to its working directory).
 */
public final class SleelaRunner {

    public enum Status { OK, ERROR, DENIED, TIMEOUT, BUSY, RATE_LIMITED, DISABLED, TOO_LARGE, VM_UNAVAILABLE }

    public static final class Result {
        public final Status status;
        public final int exitCode;
        public final String output;
        public final long millis;
        public final boolean truncated;
        public final List<String> notes;
        /** Which VM ran (or would have run) the program, as shown to the visitor. */
        public final String vm;
        /** One line on why that VM differs from or improves on the others; null when not applicable. */
        public final String why;
        Result(Status s, int exit, String out, long ms, boolean trunc, List<String> notes) {
            this(s, exit, out, ms, trunc, notes, null);
        }
        Result(Status s, int exit, String out, long ms, boolean trunc, List<String> notes, String vm) {
            this(s, exit, out, ms, trunc, notes, vm, null);
        }
        Result(Status s, int exit, String out, long ms, boolean trunc, List<String> notes, String vm, String why) {
            this.status = s; this.exitCode = exit; this.output = out; this.millis = ms; this.truncated = trunc; this.notes = notes; this.vm = vm; this.why = why;
        }
        Result withVm(String vm, String why) { return new Result(status, exitCode, output, millis, truncated, notes, vm, why); }
        public String getWhy() { return why; }
        public String getStatus() { return status.name(); }
        public int getExitCode() { return exitCode; }
        public String getOutput() { return output; }
        public long getMillis() { return millis; }
        public boolean isTruncated() { return truncated; }
        public List<String> getNotes() { return notes; }
        public boolean isOk() { return status == Status.OK; }
        public String getVm() { return vm; }
    }

    /** Startup lines the compiler prints for every run; hidden from visitors. */
    private static final Pattern NOISE = Pattern.compile("^(\\[(memory-manager|security|defender)\\].*|verification: PASS.*)$");

    private final RuntimeConfig cfg;
    private final LibUnits libs;
    private final Semaphore slots;
    private final Map<String, long[]> windows = new ConcurrentHashMap<>();
    private volatile long selfTestAt;
    private volatile String selfTestError = "not tested yet";
    private final Object testLock = new Object();

    public SleelaRunner(RuntimeConfig cfg, LibUnits libs) {
        this.cfg = cfg;
        this.libs = libs;
        this.slots = new Semaphore(cfg.maxConcurrent);
    }

    public RuntimeConfig config() { return cfg; }

    /** The VMs a visitor can choose from, with availability checked against this server. */
    public List<VmChoice> vms() { return VmChoice.catalog(cfg.home); }

    /** null when ready, else the reason the runtime is unavailable. Re-checked at most once a minute. */
    public String unavailableReason() {
        if (cfg.mode == RuntimeConfig.Mode.OFF) return "The runtime is switched off on this server (sleela.runtime.mode=off).";
        long now = System.currentTimeMillis();
        if (now - selfTestAt < 60_000L && selfTestAt != 0) return selfTestError;
        synchronized (testLock) {
            if (now - selfTestAt < 60_000L && selfTestAt != 0) return selfTestError;
            selfTestError = selfTest();
            selfTestAt = System.currentTimeMillis();
            return selfTestError;
        }
    }

    private String selfTest() {
        if (!isLinux()) return "The runtime needs Linux (bubblewrap sandbox).";
        if (cfg.home == null) return "The SLeeLa source tree was not found, so there is no compiler to run.";
        if (cfg.bwrap == null) return "bubblewrap (bwrap) is not installed on the server. Install it with: apt install bubblewrap";
        if (cfg.prlimit == null) return "prlimit (util-linux) is not installed on the server.";
        if (cfg.binary == null || !Files.isExecutable(cfg.binary)) return "The SLeeLa compiler binary was not found or is not executable: " + cfg.binary;
        if (!Files.isRegularFile(cfg.home.resolve("tools/verify-before-execution.py"))) return "tools/verify-before-execution.py is missing from " + cfg.home + ".";
        Result r = execute("run", VmChoice.catalog(cfg.home).get(0), "#sleela 2.2\nclass SelfTest { void main() { print(\"selftest-ok\"); } }\n");
        if (r.status == Status.OK && r.output.contains("selftest-ok")) return null;
        String why = r.output.isBlank() ? String.join(" ", r.notes) : r.output.trim();
        if (why.length() > 300) why = why.substring(0, 300) + "...";
        return "The sandbox self-test failed (" + r.status + "): " + why;
    }

    /** Visitor entry point. action is "run" or "check"; vm is from vms(). */
    public Result submit(String action, String source, String clientId, VmChoice vm) {
        boolean check = "check".equals(action);
        String used = check ? "None. A syntax check runs in the compiler only, so no VM is booted." : vm.getDisplay();
        return submitOn(check, source, clientId, vm).withVm(used, check ? null : vm.getWhy());
    }

    private Result submitOn(boolean check, String source, String clientId, VmChoice vm) {
        String unavailable = unavailableReason();
        if (unavailable != null) return new Result(Status.DISABLED, -1, "", 0, false, List.of(unavailable));
        if (!check && !vm.isAvailable()) {
            return new Result(Status.VM_UNAVAILABLE, -1, "", 0, false, List.of(vm.getDisplay() + " is not available: " + vm.unavailable));
        }

        // rate limit first: refused programs cost CPU to analyse, so they count too
        if (!allow(clientId)) {
            return new Result(Status.RATE_LIMITED, -1, "", 0, false,
                    List.of("Too many runs from your address. The limit is " + cfg.runsPerMinutePerClient + " per minute."));
        }
        SourcePolicy.Verdict v = SourcePolicy.check(source, cfg.maxSourceBytes, libs::sourceOf);
        if (!v.allowed) {
            boolean tooLarge = source != null && source.getBytes(StandardCharsets.UTF_8).length > cfg.maxSourceBytes;
            return new Result(tooLarge ? Status.TOO_LARGE : Status.DENIED, -1, "", 0, false, v.reasons);
        }
        if (!slots.tryAcquire()) {
            return new Result(Status.BUSY, -1, "", 0, false, List.of("The runtime is busy with other visitors. Try again in a few seconds."));
        }
        try {
            return execute(check ? "check" : "run", vm, source);
        } finally {
            slots.release();
        }
    }

    private boolean allow(String client) {
        long now = System.currentTimeMillis();
        if (windows.size() > 5000) windows.clear();
        long[] w = windows.computeIfAbsent(client, k -> new long[cfg.runsPerMinutePerClient + 1]);
        synchronized (w) {
            int used = 0;
            long oldest = Long.MAX_VALUE;
            for (int i = 0; i < w.length - 1; i++) {
                if (now - w[i] < 60_000L) { used++; oldest = Math.min(oldest, w[i]); }
            }
            if (used >= cfg.runsPerMinutePerClient) return false;
            for (int i = 0; i < w.length - 1; i++) if (now - w[i] >= 60_000L) { w[i] = now; return true; }
            return true;
        }
    }

    // ------------------------------------------------------------------ execution

    /**
     * Runs one action. A standard VM (idea-vm-NNN) is a self-check program layered on the same native core, not an engine
     * that takes a program, so for a standard run the visitor program runs on the native engine first (its print output is
     * shown), and then the standard VM's own self-check runs and its output is appended under a labelled divider.
     */
    Result execute(String sub, VmChoice vm, String source) {
        if (!"run".equals(sub) || vm.tier != VmChoice.Tier.STANDARD) return executeOnce(sub, vm, source);
        VmChoice nativeVm = VmChoice.catalog(cfg.home).get(0);
        Result prog = executeOnce("run", nativeVm, source);
        Result self = executeOnce("run", vm, "");
        List<String> notes = new ArrayList<>();
        notes.add("Your program ran on the native core that " + vm.getVmName() + " is built on (" + vm.getVmName()
                + " does not load programs itself). Its output is first; the " + vm.getVmName() + " self-check follows.");
        notes.addAll(prog.notes);
        if (self.status != Status.OK) notes.add(vm.getVmName() + " self-check did not pass (" + self.status + ", exit code " + self.exitCode + ").");
        String out = (prog.output.isEmpty() ? "(your program printed nothing)" : prog.output)
                + "\n\n---- " + vm.getVmName() + " self-check ----\n" + self.output;
        return new Result(prog.status, prog.exitCode, out, prog.millis + self.millis, prog.truncated || self.truncated, notes);
    }

    private Result executeOnce(String sub, VmChoice vm, String source) {
        boolean premium = "run".equals(sub) && vm.tier == VmChoice.Tier.PREMIUM;
        boolean standard = "run".equals(sub) && vm.tier == VmChoice.Tier.STANDARD;
        long t0 = System.nanoTime();
        Path dir = null;
        List<String> notes = new ArrayList<>();
        try {
            dir = Files.createTempDirectory("sleela-run-", PosixFilePermissions.asFileAttribute(PosixFilePermissions.fromString("rwx------")));
            Files.writeString(dir.resolve("main.sleela"), source, StandardCharsets.UTF_8);

            List<String> cmd = new ArrayList<>();
            cmd.add(cfg.bwrap);
            for (String a : new String[] {"--unshare-all", "--die-with-parent", "--new-session", "--cap-drop", "ALL", "--clearenv"}) cmd.add(a);
            for (String d : new String[] {"/usr"}) { cmd.add("--ro-bind"); cmd.add(d); cmd.add(d); }
            for (String[] l : new String[][] {{"usr/bin", "/bin"}, {"usr/sbin", "/sbin"}, {"usr/lib", "/lib"}, {"usr/lib64", "/lib64"}}) {
                if (Files.exists(java.nio.file.Paths.get("/" + l[0]))) { cmd.add("--symlink"); cmd.add(l[0]); cmd.add(l[1]); }
            }
            for (String t : new String[] {"/etc/ld.so.cache", "/etc/alternatives", "/etc/localtime"}) { cmd.add("--ro-bind-try"); cmd.add(t); cmd.add(t); }
            cmd.add("--dev"); cmd.add("/dev");
            cmd.add("--tmpfs"); cmd.add("/tmp");
            String home = cfg.home.toRealPath().toString();
            cmd.add("--ro-bind"); cmd.add(home); cmd.add(home);
            for (String extra : cfg.extraReadOnlyBinds) { cmd.add("--ro-bind"); cmd.add(extra); cmd.add(extra); }
            cmd.add("--bind"); cmd.add(dir.toString()); cmd.add("/work");
            // The premium CLI path finds sleela-virtual-machine/NNN relative to its working directory, so it starts in the
            // read-only SLeeLa tree; its compiled artifact goes to the empty tmpfs /tmp. Everything else starts in /work.
            cmd.add("--chdir"); cmd.add(premium ? home : "/work");
            for (String[] e : new String[][] {{"PATH", "/usr/bin:/bin"}, {"HOME", "/tmp"}, {"LANG", "C.UTF-8"}, {"SLEELA_HOME", home}}) {
                cmd.add("--setenv"); cmd.add(e[0]); cmd.add(e[1]);
            }
            cmd.add(cfg.prlimit);
            cmd.add("--as=" + cfg.memoryBytes);
            cmd.add("--cpu=" + cfg.cpuSeconds);
            cmd.add("--fsize=" + cfg.fileBytes);
            cmd.add("--nofile=64");
            cmd.add("--core=0");
            cmd.add("--");
            if (standard) {
                // A standard VM is a built smoke/demo program, not an engine: run its own self-check. The visitor source is
                // written to /work like any run but is not executed by it.
                cmd.add(cfg.home.resolve(vm.binaryPath).toRealPath().toString());
            } else {
                cmd.add(cfg.binary.toRealPath().toString());
                if (premium) {
                    for (String a : new String[] {"vm", "run", "premium", Integer.toString(vm.number), "/work/main.sleela"}) cmd.add(a);
                } else {
                    cmd.add(sub);
                    cmd.add("main.sleela");
                }
            }

            ProcessBuilder pb = new ProcessBuilder(cmd);
            pb.redirectErrorStream(true);
            pb.environment().clear();
            pb.environment().put("PATH", "/usr/bin:/bin");
            Process p = pb.start();

            ByteArrayOutputStream buf = new ByteArrayOutputStream();
            boolean[] truncated = {false};
            Thread reader = new Thread(() -> {
                byte[] chunk = new byte[4096];
                try (InputStream in = p.getInputStream()) {
                    int n;
                    while ((n = in.read(chunk)) > 0) {
                        int room = cfg.maxOutputBytes - buf.size();
                        if (room > 0) buf.write(chunk, 0, Math.min(room, n));
                        if (n > room) { truncated[0] = true; p.destroyForcibly(); break; }
                    }
                } catch (IOException ignored) { }
            }, "sleela-run-reader");
            reader.setDaemon(true);
            reader.start();

            boolean finished = p.waitFor(cfg.timeoutSeconds, TimeUnit.SECONDS);
            if (!finished) {
                p.descendants().forEach(ProcessHandle::destroyForcibly);
                p.destroyForcibly();
                p.waitFor(5, TimeUnit.SECONDS);
            }
            reader.join(2000);
            String out = clean(buf.toString(StandardCharsets.UTF_8));
            long ms = (System.nanoTime() - t0) / 1_000_000L;
            if (!finished) {
                notes.add("Stopped after " + cfg.timeoutSeconds + " seconds.");
                return new Result(Status.TIMEOUT, -1, out, ms, truncated[0], notes);
            }
            if (truncated[0]) notes.add("Output was cut at " + (cfg.maxOutputBytes / 1024) + " KB and the program was stopped.");
            int code = p.exitValue();
            if (code == 137 || code == 152 || code == 159) notes.add("The program hit a resource limit (CPU " + cfg.cpuSeconds + " s or memory " + (cfg.memoryBytes >> 20) + " MB).");
            return new Result(code == 0 ? Status.OK : Status.ERROR, code, out, ms, truncated[0], notes);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return new Result(Status.ERROR, -1, "", 0, false, List.of("Interrupted."));
        } catch (IOException | RuntimeException e) {
            return new Result(Status.ERROR, -1, "", (System.nanoTime() - t0) / 1_000_000L, false,
                    List.of("The runtime could not start: " + e.getClass().getSimpleName()));
        } finally {
            if (dir != null) deleteTree(dir);
        }
    }

    static String clean(String raw) {
        StringBuilder sb = new StringBuilder();
        for (String line : raw.split("\r?\n", -1)) {
            if (NOISE.matcher(line).matches()) continue;
            sb.append(line).append('\n');
        }
        return sb.toString().replaceAll("\\s+$", "");
    }

    private static void deleteTree(Path dir) {
        try {
            Files.walkFileTree(dir, new SimpleFileVisitor<Path>() {
                @Override public FileVisitResult visitFile(Path f, BasicFileAttributes a) throws IOException { Files.deleteIfExists(f); return FileVisitResult.CONTINUE; }
                @Override public FileVisitResult postVisitDirectory(Path d, IOException e) throws IOException { Files.deleteIfExists(d); return FileVisitResult.CONTINUE; }
            });
        } catch (IOException ignored) { }
    }

    private static boolean isLinux() { return System.getProperty("os.name", "").toLowerCase(java.util.Locale.ROOT).contains("linux"); }
}
