package com.mearvk.sleela.gui;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.Objects;

/**
 * Runnable Path 1 adapter. Java hosts the GUI and launches the native SLeeLa
 * executable for a business operation. The operation is represented by a
 * Wrapper™ path; the native SLeeLa process remains authoritative for logic.
 */
public final class ProcessSleelaRuntime implements SleelaRuntime {
    private final Path sleelaExecutable;
    private final Path workingDirectory;

    public ProcessSleelaRuntime(Path sleelaExecutable, Path workingDirectory) {
        this.sleelaExecutable = Objects.requireNonNull(sleelaExecutable, "sleelaExecutable");
        this.workingDirectory = Objects.requireNonNull(workingDirectory, "workingDirectory");
    }

    @Override
    public Object call(String operation, Object... arguments) {
        Objects.requireNonNull(operation, "operation");
        try {
            Process process = new ProcessBuilder(
                    sleelaExecutable.toString(), "run", operation)
                    .directory(workingDirectory.toFile())
                    .redirectErrorStream(true)
                    .start();
            String output = new String(process.getInputStream().readAllBytes(), StandardCharsets.UTF_8);
            int exit = process.waitFor();
            if (exit != 0) {
                throw new IllegalStateException("SLeeLa operation failed: " + output.trim());
            }
            return output.trim();
        } catch (IOException e) {
            throw new IllegalStateException("Unable to start SLeeLa runtime", e);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("SLeeLa runtime interrupted", e);
        }
    }

    @Override
    public void close() {
        // Each call is an isolated SLeeLa process.
    }
}
