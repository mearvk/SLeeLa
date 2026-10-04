package com.mearvk.sleela.skya;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.time.format.DateTimeFormatter;
import java.time.ZonedDateTime;

/**
 * Tiny dependency-free logger shared by the Skya JavaFX apps and the Guia
 * footer. There was no logging layer before; this gives every component one
 * consistent, timestamped, level-filtered output path.
 *
 * <p>Format: {@code 2026-10-04T12:00:00.123Z LEVEL [component] message}.
 *
 * <p>Configuration (environment, read once at class load):
 * <ul>
 *   <li>{@code SKYA_LOG_LEVEL} — DEBUG | INFO | WARN | ERROR | OFF (default INFO)</li>
 *   <li>{@code SKYA_LOG_FILE}  — append log lines to this file in addition to the console</li>
 * </ul>
 * Console output always goes to {@code stderr} so it never corrupts any
 * stdout-based protocol stream.
 */
public final class SkyaLog {
    public enum Level { DEBUG, INFO, WARN, ERROR, OFF }

    private static final DateTimeFormatter TS =
            DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
    private static final Level THRESHOLD = resolveLevel();
    private static final PrintStream CONSOLE = System.err;
    private static final Object LOCK = new Object();
    private static PrintStream fileStream = openFileStream();

    private SkyaLog() {}

    public static void debug(String component, String message) { log(Level.DEBUG, component, message); }
    public static void info(String component, String message)  { log(Level.INFO, component, message); }
    public static void warn(String component, String message)  { log(Level.WARN, component, message); }
    public static void error(String component, String message) { log(Level.ERROR, component, message); }

    /** Log a Guia command/event line under a component tag at INFO level. */
    public static void guia(String component, String direction, String line) {
        info(component, direction + " " + line);
    }

    public static Level level() { return THRESHOLD; }
    public static boolean isEnabled(Level level) {
        return THRESHOLD != Level.OFF && level.ordinal() >= THRESHOLD.ordinal();
    }

    private static void log(Level level, String component, String message) {
        if (!isEnabled(level)) return;
        String line = ZonedDateTime.now(java.time.ZoneOffset.UTC).format(TS)
                + " " + level + " [" + component + "] " + sanitize(message);
        synchronized (LOCK) {
            CONSOLE.println(line);
            if (fileStream != null) { fileStream.println(line); fileStream.flush(); }
        }
    }

    private static Level resolveLevel() {
        String v = System.getenv("SKYA_LOG_LEVEL");
        if (v == null || v.isBlank()) return Level.INFO;
        try { return Level.valueOf(v.trim().toUpperCase()); }
        catch (IllegalArgumentException e) { return Level.INFO; }
    }

    private static PrintStream openFileStream() {
        String f = System.getenv("SKYA_LOG_FILE");
        if (f == null || f.isBlank()) return null;
        try {
            Path p = Paths.get(f.trim());
            if (p.getParent() != null) Files.createDirectories(p.getParent());
            OutputStream os = Files.newOutputStream(p, StandardOpenOption.CREATE, StandardOpenOption.APPEND);
            return new PrintStream(os, false, StandardCharsets.UTF_8);
        } catch (IOException e) {
            System.err.println("[skya-log] cannot open SKYA_LOG_FILE '" + f + "': " + e.getMessage());
            return null;
        }
    }

    private static String sanitize(String s) {
        return s == null ? "" : s.replace('\n', ' ').replace('\r', ' ');
    }
}
