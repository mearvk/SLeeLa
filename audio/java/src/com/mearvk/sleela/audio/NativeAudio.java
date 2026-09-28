package com.mearvk.sleela.audio;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Objects;

public final class NativeAudio implements Audio {
    private final Path executable;

    public NativeAudio(Path executable) {
        this.executable = Objects.requireNonNull(executable, "executable")
                .toAbsolutePath()
                .normalize();
    }

    @Override
    public boolean validate(Configuration c) {
        return c != null
                && c.sampleRate() > 0
                && c.inputs() != null
                && !c.inputs().isEmpty()
                && c.inputs().size() <= 128
                && c.output() != null
                && c.controls() != null
                && Files.isExecutable(executable)
                && c.inputs().stream().allMatch(i ->
                    i != null
                    && i.source() != null
                    && !i.source().isBlank()
                    && i.startSeconds() >= 0
                    && Double.isFinite(i.startSeconds())
                    && Double.isFinite(i.gainDb()));
    }

    /**
     * Executes a native adapter using the SLeeLa audio process contract:
     * executable OUTPUT SAMPLE_RATE INPUT...
     *
     * The executable must document and implement this argument contract.
     */
    @Override
    public void process(Configuration c) {
        if (!validate(c)) {
            throw new IllegalArgumentException("Invalid audio configuration or native executable");
        }

        final var command = new java.util.ArrayList<String>();
        command.add(executable.toString());
        command.add(c.output().toString());
        command.add(Integer.toString(c.sampleRate()));
        c.inputs().forEach(i -> command.add(i.source()));

        try {
            final Process process = new ProcessBuilder(command).inheritIO().start();
            final int status = process.waitFor();
            if (status != 0) {
                throw new IllegalStateException("Native audio exit " + status);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("Native audio processing interrupted", e);
        } catch (java.io.IOException e) {
            throw new IllegalStateException("Native audio process could not start", e);
        }
    }
}
