package com.mearvk.sleela.connector.process;

import com.mearvk.sleela.connector.SleelaInvocation;
import com.mearvk.sleela.connector.SleelaJavaConnector;
import com.mearvk.sleela.connector.SleelaResult;
import com.mearvk.sleela.gui.ProcessSleelaRuntime;

import java.nio.file.Path;
import java.util.Objects;

/** Direct local connector for a SLeeLa executable. */
public final class SleelaProcessConnector implements SleelaJavaConnector {
    private final ProcessSleelaRuntime runtime;

    public SleelaProcessConnector(Path sleelaExecutable, Path workingDirectory) {
        this.runtime = new ProcessSleelaRuntime(
                Objects.requireNonNull(sleelaExecutable, "sleelaExecutable"),
                Objects.requireNonNull(workingDirectory, "workingDirectory"));
    }

    @Override
    public SleelaResult invoke(SleelaInvocation invocation) {
        try {
            Object value = runtime.call(invocation.operation(), invocation.arguments());
            return SleelaResult.success(value == null ? "" : String.valueOf(value));
        } catch (RuntimeException failure) {
            return SleelaResult.failure(messageOf(failure));
        }
    }

    @Override
    public String health() {
        return "SLeeLa process connector = ready";
    }

    @Override
    public void close() {
        runtime.close();
    }

    private static String messageOf(Throwable failure) {
        return failure.getMessage() == null ? failure.toString() : failure.getMessage();
    }
}
