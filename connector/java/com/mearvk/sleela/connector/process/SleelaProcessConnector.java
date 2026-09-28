package com.mearvk.sleela.connector.process;

import com.mearvk.sleela.connector.SleelaInvocation;
import com.mearvk.sleela.connector.SleelaJavaConnector;
import com.mearvk.sleela.connector.SleelaResult;
import com.mearvk.sleela.gui.ProcessSleelaRuntime;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Objects;

/** Direct local connector for a SLeeLa executable. */
public final class SleelaProcessConnector implements SleelaJavaConnector {
    private final ProcessSleelaRuntime runtime;
    private final Path sleelaExecutable;
    private final Path workingDirectory;

    public SleelaProcessConnector(Path sleelaExecutable, Path workingDirectory) {
        this.sleelaExecutable = Objects.requireNonNull(sleelaExecutable, "sleelaExecutable");
        this.workingDirectory = Objects.requireNonNull(workingDirectory, "workingDirectory");
        this.runtime = new ProcessSleelaRuntime(this.sleelaExecutable, this.workingDirectory);
    }

    @Override
    public SleelaResult invoke(SleelaInvocation invocation) {
        try {
            Object value = runtime.call(invocation.operation(), invocation.arguments());
            return SleelaResult.success(value == null ? "" : String.valueOf(value));
        } catch (Throwable failure) {
            // Fold any fault (RuntimeException OR Error) into a failure result so
            // the transport-neutral contract never leaks an unchecked throwable.
            return SleelaResult.failure(messageOf(failure));
        }
    }

    /**
     * Real, cheap liveness check: confirm the SLeeLa executable exists, is a
     * regular file, and is executable, and that the working directory is a
     * directory. Previously this returned a hardcoded "ready" regardless of
     * whether anything was actually runnable.
     */
    @Override
    public String health() {
        if (!Files.isRegularFile(sleelaExecutable)) {
            throw new IllegalStateException("SLeeLa executable not found: " + sleelaExecutable);
        }
        if (!Files.isExecutable(sleelaExecutable)) {
            throw new IllegalStateException("SLeeLa executable is not executable: " + sleelaExecutable);
        }
        if (!Files.isDirectory(workingDirectory)) {
            throw new IllegalStateException("working directory not found: " + workingDirectory);
        }
        return "SLeeLa process connector = ready (" + sleelaExecutable + ")";
    }

    @Override
    public void close() {
        runtime.close();
    }

    private static String messageOf(Throwable failure) {
        return failure.getMessage() == null ? failure.toString() : failure.getMessage();
    }
}
