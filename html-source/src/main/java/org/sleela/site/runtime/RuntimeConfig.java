package org.sleela.site.runtime;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

/**
 * Settings for the website runtime. Read from context parameters (Tomcat ROOT.xml, written by the deploy script),
 * then system properties, then defaults. Every limit has a hard ceiling so a typo cannot open things up.
 */
public final class RuntimeConfig {
    /** auto: run visitor code only when the sandbox self-test passes. off: the form is shown but disabled. */
    public enum Mode { AUTO, OFF }

    public final Mode mode;
    public final Path home;
    public final Path binary;
    public final String bwrap;
    public final String prlimit;
    public final int timeoutSeconds;
    public final int cpuSeconds;
    public final long memoryBytes;
    public final long fileBytes;
    public final int maxOutputBytes;
    public final int maxSourceBytes;
    public final int maxConcurrent;
    public final int runsPerMinutePerClient;
    public final List<String> extraReadOnlyBinds;

    private RuntimeConfig(Function<String, String> p, Path home) {
        this.home = home;
        this.mode = parseMode(p.apply("sleela.runtime.mode"));
        String bin = p.apply("sleela.runtime.bin");
        this.binary = bin != null && !bin.isBlank() ? Paths.get(bin) : home == null ? null : home.resolve("working-binaries/sleela");
        this.bwrap = which(p.apply("sleela.runtime.bwrap"), "/usr/bin/bwrap", "/bin/bwrap", "/usr/local/bin/bwrap");
        this.prlimit = which(p.apply("sleela.runtime.prlimit"), "/usr/bin/prlimit", "/bin/prlimit");
        this.timeoutSeconds = clamp(p.apply("sleela.runtime.timeoutSeconds"), 25, 5, 90);
        this.cpuSeconds = clamp(p.apply("sleela.runtime.cpuSeconds"), 20, 2, 90);
        this.memoryBytes = clamp(p.apply("sleela.runtime.memoryMB"), 2048, 1024, 4096) * 1024L * 1024L;
        this.fileBytes = clamp(p.apply("sleela.runtime.fileKB"), 1024, 0, 16384) * 1024L;
        this.maxOutputBytes = clamp(p.apply("sleela.runtime.maxOutputKB"), 64, 4, 1024) * 1024;
        this.maxSourceBytes = clamp(p.apply("sleela.runtime.maxSourceKB"), 32, 1, 256) * 1024;
        this.maxConcurrent = clamp(p.apply("sleela.runtime.maxConcurrent"), 2, 1, 16);
        this.runsPerMinutePerClient = clamp(p.apply("sleela.runtime.runsPerMinute"), 6, 1, 120);
        List<String> binds = new ArrayList<>();
        String extra = p.apply("sleela.runtime.extraReadOnlyBinds");
        if (extra != null) for (String s : extra.split(":")) if (s.startsWith("/") && Files.isDirectory(Paths.get(s))) binds.add(s);
        this.extraReadOnlyBinds = List.copyOf(binds);
    }

    public static RuntimeConfig load(Function<String, String> contextParam, Path home) {
        return new RuntimeConfig(k -> {
            String v = contextParam.apply(k);
            return v != null ? v : System.getProperty(k);
        }, home);
    }

    private static Mode parseMode(String s) {
        if (s == null || s.isBlank()) return Mode.AUTO;
        return s.trim().equalsIgnoreCase("auto") ? Mode.AUTO : Mode.OFF;
    }

    private static int clamp(String s, int def, int min, int max) {
        int v = def;
        if (s != null) try { v = Integer.parseInt(s.trim()); } catch (NumberFormatException ignored) { }
        return Math.max(min, Math.min(max, v));
    }

    private static String which(String configured, String... candidates) {
        if (configured != null && !configured.isBlank()) return Files.isExecutable(Paths.get(configured)) ? configured : null;
        for (String c : candidates) if (Files.isExecutable(Paths.get(c))) return c;
        return null;
    }
}
