package com.mearvk.sleela.gui;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.TimeUnit;

/**
 * Runnable Path 1 adapter. Java hosts the GUI and launches the native SLeeLa
 * executable for a business operation. The operation is represented by a
 * Wrapper™ path; the native SLeeLa process remains authoritative for logic.
 */
public final class ProcessSleelaRuntime implements SleelaRuntime {
    /** Default wall-clock ceiling for one native call. */
    private static final long DEFAULT_TIMEOUT_SECONDS = 60L;
    /** Hard cap on captured stdout, to bound memory if the child floods output. */
    private static final int MAX_OUTPUT_BYTES = 8 << 20; // 8 MiB

    private final Path sleelaExecutable;
    private final Path workingDirectory;
    private final long timeoutSeconds;

    public ProcessSleelaRuntime(Path sleelaExecutable, Path workingDirectory) {
        this(sleelaExecutable, workingDirectory, DEFAULT_TIMEOUT_SECONDS);
    }

    public ProcessSleelaRuntime(Path sleelaExecutable, Path workingDirectory, long timeoutSeconds) {
        this.sleelaExecutable = Objects.requireNonNull(sleelaExecutable, "sleelaExecutable");
        this.workingDirectory = Objects.requireNonNull(workingDirectory, "workingDirectory");
        if (timeoutSeconds <= 0) {
            throw new IllegalArgumentException("timeoutSeconds must be positive");
        }
        this.timeoutSeconds = timeoutSeconds;
    }

    @Override
    public Object call(String operation, Object... arguments) {
        Objects.requireNonNull(operation, "operation");
        // Build the command line, forwarding each argument (previously dropped).
        List<String> command = new ArrayList<>();
        command.add(sleelaExecutable.toString());
        command.add("run");
        command.add(operation);
        if (arguments != null) {
            for (Object arg : arguments) {
                command.add(String.valueOf(arg));
            }
        }
        Process process = null;
        try {
            process = new ProcessBuilder(command)
                    .directory(workingDirectory.toFile())
                    .redirectErrorStream(true)
                    .start();
            // Bounded read of stdout so a runaway child cannot exhaust memory.
            String output = readBounded(process.getInputStream(), MAX_OUTPUT_BYTES);
            // Enforce a wall-clock ceiling so a hung child cannot block forever.
            if (!process.waitFor(timeoutSeconds, TimeUnit.SECONDS)) {
                process.destroyForcibly();
                throw new IllegalStateException(
                        "SLeeLa operation timed out after " + timeoutSeconds + "s: " + operation);
            }
            int exit = process.exitValue();
            if (exit != 0) {
                throw new IllegalStateException("SLeeLa operation failed: " + output.trim());
            }
            return output.trim();
        } catch (IOException e) {
            throw new IllegalStateException("Unable to start SLeeLa runtime", e);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("SLeeLa runtime interrupted", e);
        } finally {
            if (process != null && process.isAlive()) {
                process.destroyForcibly();
            }
        }
    }

    /** Reads up to {@code limit} bytes; stops early rather than growing without bound. */
    private static String readBounded(InputStream in, int limit) throws IOException {
        ByteArrayOutputStream acc = new ByteArrayOutputStream();
        byte[] buf = new byte[8192];
        int total = 0;
        int n;
        while ((n = in.read(buf)) != -1) {
            int room = limit - total;
            if (room <= 0) {
                break;
            }
            acc.write(buf, 0, Math.min(n, room));
            total += n;
        }
        return new String(acc.toByteArray(), StandardCharsets.UTF_8);
    }

    @Override
    public void close() {
        // Each call is an isolated SLeeLa process.
    }
}
