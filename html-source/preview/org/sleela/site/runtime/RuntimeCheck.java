package org.sleela.site.runtime;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

/**
 * Self-check for the website runtime: the source policy, and real escape attempts against the sandbox.
 * usage: java org.sleela.site.runtime.RuntimeCheck <SLeeLa-checkout>
 * Exits non-zero if any check fails. Needs bubblewrap, prlimit and a built working-binaries/sleela.
 */
public final class RuntimeCheck {
    static int failed = 0, passed = 0;

    static void check(String name, boolean ok, String detail) {
        if (ok) passed++; else failed++;
        System.out.println((ok ? "  PASS  " : "  FAIL  ") + name + (ok ? "" : "   <- " + detail));
    }

    static String prog(String body) { return "#sleela 2.2\nclass T {\n  void main() {\n" + body + "\n  }\n}\n"; }

    public static void main(String[] a) throws Exception {
        Path home = Paths.get(a[0]).toAbsolutePath();
        System.setProperty("sleela.runtime.timeoutSeconds", "6");
        System.setProperty("sleela.runtime.cpuSeconds", "4");
        RuntimeConfig cfg = RuntimeConfig.load(k -> null, home);
        LibUnits libs = new LibUnits(home);
        SleelaRunner runner = new SleelaRunner(cfg, libs);

        System.out.println("== policy: bundled samples must be allowed");
        for (Samples.Sample s : Samples.list()) {
            SourcePolicy.Verdict v = SourcePolicy.check(s.getSource(), cfg.maxSourceBytes, libs::sourceOf);
            check("sample " + s.getId(), v.allowed, String.valueOf(v.reasons));
        }

        System.out.println("== policy: host-reaching programs must be refused");
        String[][] bad = {
            {"osRun", "int r = osRun(\"id\");"}, {"osSpawn", "int r = osSpawn(\"sleep 1\");"},
            {"openFile", "int h = openFile(\"/etc/passwd\", \"r\");"}, {"unlinkFile", "unlinkFile(\"/tmp/x\");"},
            {"connect", "int s = connect(\"example.com\", 80);"}, {"listen", "int s = listen(8080);"},
            {"osGetEnv", "String s = osGetEnv(\"HOME\");"}, {"Munction", "Munction m = Munction.start(\"x\");"},
            {"nativeOpen", "int h = nativeOpen(\"/lib/x.so\");"}, {"spawn", "spawn(\"main\");"},
            {"oslsListDirectory", "oslsListDirectory(\"/\");"}, {"synchroOpen", "synchroOpen(\"h\", 1);"},
            {"bestOfNew", "bestOfNew();"}, {"timeNtp", "timeNtp(\"pool.ntp.org\");"},
        };
        for (String[] b : bad) {
            SourcePolicy.Verdict v = SourcePolicy.check(prog(b[1]), cfg.maxSourceBytes, libs::sourceOf);
            check("deny " + b[0], !v.allowed && v.reasons.toString().contains(b[0]), String.valueOf(v.reasons));
        }
        check("deny native member", !SourcePolicy.check("#sleela 3.0\nclass T { native int f(); void main() {} }\n", 65536, libs::sourceOf).allowed, "allowed");
        check("deny dynamite connector", !SourcePolicy.check("#sleela 2.2\nimport dynamite connector x = a;\nclass T { void main() {} }\n", 65536, libs::sourceOf).allowed, "allowed");
        check("deny oversize", !SourcePolicy.check(prog("print(\"" + "x".repeat(70000) + "\");"), 32768, libs::sourceOf).allowed, "allowed");
        check("deny control chars", !SourcePolicy.check("#sleela 2.2\u0001\nclass T { void main() {} }\n", 65536, libs::sourceOf).allowed, "allowed");
        // evasion attempts: each hides a host call behind a construct a sloppy stripper would treat as a comment or string
        String[][] evade = {
            {"char literal quote", "char c = '\"';\nosRun(\"echo CANARY\");"},
            {"shift operator before block-comment text", "int a = 1 << 2;\nosRun(\"echo CANARY\");\nint b = a >> 1;"},
            {"angle comment not at line start", "int a = 1; < x >\nosRun(\"echo CANARY\");"},
            {"escaped quote in string", "String s = \"a\\\"b\";\nosRun(\"echo CANARY\");"},
            {"unterminated block comment", "<< never closed\nosRun(\"echo CANARY\");"},
            {"string then code on one line", "String s = \"x\"; osRun(\"echo CANARY\");"},
            {"lone apostrophe", "int a = 1; ' osRun(\"echo CANARY\");"},
        };
        for (String[] e : evade) {
            SourcePolicy.Verdict v = SourcePolicy.check(prog(e[1]), cfg.maxSourceBytes, libs::sourceOf);
            check("evasion refused: " + e[0], !v.allowed, "ALLOWED");
        }
        check("user class named like a lib unit is fine", SourcePolicy.check("#sleela 2.2\nclass SLCompatLayer { void hi() { print(\"mine\"); } }\nclass T { void main() { SLCompatLayer c = new SLCompatLayer(); c.hi(); } }\n", 65536, libs::sourceOf).allowed, "denied");
        check("allow banned word inside string", SourcePolicy.check(prog("print(\"osRun connect openFile\");"), 65536, libs::sourceOf).allowed, "denied");
        check("allow banned word in // comment", SourcePolicy.check(prog("// osRun is not used\nprint(\"hi\");"), 65536, libs::sourceOf).allowed, "denied");
        check("allow banned word in << >> comment", SourcePolicy.check("#sleela 2.2\n<< osRun connect >>\nclass T { void main() { print(\"x\"); } }\n", 65536, libs::sourceOf).allowed, "denied");

        System.out.println("== policy: library units are followed (a program cannot reach osRun through lib/)");
        String viaLib = null;
        try (Stream<Path> w = Files.walk(home.resolve("lib/operating-system"), 2)) {
            for (Path p : (Iterable<Path>) w::iterator) {
                if (p.toString().endsWith(".sleela") && Files.readString(p).contains("osRun(")) { viaLib = p.getFileName().toString().replace(".sleela", ""); break; }
            }
        }
        if (viaLib == null) check("find lib unit that calls osRun", false, "none found");
        else {
            SourcePolicy.Verdict v = SourcePolicy.check(prog(viaLib + " x = new " + viaLib + "();"), cfg.maxSourceBytes, libs::sourceOf);
            check("deny via library unit " + viaLib, !v.allowed && v.reasons.toString().contains("library unit"), String.valueOf(v.reasons));
        }

        System.out.println("== policy: how many repo examples are runnable on the website");
        int total = 0, ok = 0;
        for (String root : new String[] {"examples", "lib/examples"}) {
            Path r = home.resolve(root);
            if (!Files.isDirectory(r)) continue;
            try (Stream<Path> w = Files.walk(r, 3)) {
                for (Path p : (Iterable<Path>) w::iterator) {
                    if (!p.toString().endsWith(".sleela") || !Files.isRegularFile(p)) continue;
                    total++;
                    if (SourcePolicy.check(Files.readString(p), cfg.maxSourceBytes, libs::sourceOf).allowed) ok++;
                }
            }
        }
        System.out.println("  info  " + ok + " of " + total + " example .sleela files pass the policy");

        System.out.println("== sandbox availability");
        String why = runner.unavailableReason();
        check("self-test", why == null, why);
        if (why != null) { System.out.println("\n" + passed + " passed, " + failed + " failed (sandbox checks skipped)"); System.exit(1); }

        System.out.println("== sandbox: normal run");
        SleelaRunner.Result r = runner.submit("run", Samples.list().get(1).getSource(), "t1");
        check("sample runs", r.status == SleelaRunner.Status.OK && r.output.contains("sum 1..10 = 55"), r.status + " " + r.output);
        check("startup banner hidden", !r.output.contains("[memory-manager]") && !r.output.contains("verification"), r.output);
        r = runner.submit("check", Samples.list().get(0).getSource(), "t1");
        check("check action works", r.status == SleelaRunner.Status.OK || r.status == SleelaRunner.Status.ERROR, r.status.toString());
        r = runner.submit("run", prog("print(1 +"), "t1");
        check("syntax error reported, not hidden", r.status == SleelaRunner.Status.ERROR && !r.output.isBlank(), r.status + " " + r.output);

        System.out.println("== sandbox: escape attempts (policy bypassed on purpose, to test the OS layer)");
        r = runner.execute("run", prog("osRun(\"cat /etc/passwd 2>&1; ls / 2>&1; ls /root /home /srv /var 2>&1; id 2>&1\");"));
        check("host files /etc/passwd, /root, /home, /srv are not visible", !r.output.contains("root:x") && !r.output.contains("sleela-site"), r.output);
        r = runner.execute("run", prog("int s = connect(\"1.1.1.1\", 80);\nprint(\"connect=\" + s);\nosRun(\"cat /proc/net/dev 2>&1 | head -3\");"));
        check("no network (connect fails)", r.output.contains("connect=-1"), r.output);
        r = runner.execute("run", prog("int h = openFile(\"" + home + "/README.md\", \"a\");\nprint(\"h=\" + h);\nint g = openFile(\"/usr/bin/x\", \"w\");\nprint(\"g=\" + g);"));
        check("SLeeLa tree and /usr are read-only", r.output.contains("h=-1") && r.output.contains("g=-1"), r.output);
        r = runner.execute("run", prog("osRun(\"echo \\\"$SECRET_TOKEN|$CATALINA_BASE|$SLEELA_X\\\"\");"));
        check("server environment is not inherited", r.output.trim().equals("||") || r.output.contains("||"), r.output);
        r = runner.execute("run", prog("osRun(\"echo hi > /work/note.txt; cat /work/note.txt; ls /tmp\");"));
        check("private /work and /tmp work inside", r.output.contains("hi"), r.output);
        r = runner.execute("run", prog("osRun(\"ls /work\");"));
        check("nothing left over from the previous run", !r.output.contains("note.txt"), r.output);
        long t = System.nanoTime();
        r = runner.execute("run", prog("while (true) { int x = 1; }"));
        long secs = (System.nanoTime() - t) / 1_000_000_000L;
        check("infinite loop is stopped (" + secs + " s)", (r.status == SleelaRunner.Status.TIMEOUT || r.status == SleelaRunner.Status.ERROR) && secs <= 12, r.status + " after " + secs + "s");
        r = runner.execute("run", prog("while (true) { print(\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"); }"));
        check("output flood is capped (" + r.output.length() + " chars)", r.output.length() <= cfg.maxOutputBytes + 10, String.valueOf(r.output.length()));
        r = runner.execute("run", prog("osSpawn(\"sleep 300\");\nprint(\"spawned\");"));
        Thread.sleep(500);
        Process ps = new ProcessBuilder("sh", "-c", "ps -eo args | grep -c '^sleep 300' || true").start();
        String n = new String(ps.getInputStream().readAllBytes()).trim();
        check("child processes die with the run", n.equals("0"), "still running: " + n);
        long[] mem = new long[1];
        r = runner.execute("run", prog("int i = 0;\nwhile (i < 3000) { int[] a = new int[30000000]; i = i + 1; }\nprint(\"survived\");"));
        check("memory bomb does not succeed", !r.output.contains("survived"), r.status + " " + r.output);

        System.out.println("== limits: rate limit and concurrency");
        System.setProperty("sleela.runtime.runsPerMinute", "3");
        SleelaRunner small = new SleelaRunner(RuntimeConfig.load(k -> null, home), libs);
        int limited = 0;
        for (int i = 0; i < 5; i++) if (small.submit("check", Samples.list().get(0).getSource(), "same-client").status == SleelaRunner.Status.RATE_LIMITED) limited++;
        check("4th and 5th run in a minute are rate limited", limited == 2, "limited=" + limited);
        check("a different client is not affected", small.submit("check", Samples.list().get(0).getSource(), "other").status != SleelaRunner.Status.RATE_LIMITED, "limited");

        System.out.println("\n" + passed + " passed, " + failed + " failed");
        System.exit(failed == 0 ? 0 : 1);
    }
}
