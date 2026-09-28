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
     * --output OUTPUT --sample-rate SAMPLE_RATE --input PATH START_SECONDS GAIN_DB ...
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
        command.add("--output");
        command.add(c.output().toString());
        command.add("--sample-rate");
        command.add(Integer.toString(c.sampleRate()));
        c.inputs().forEach(i -> {
            command.add("--input");
            command.add(i.source());
            command.add(Double.toString(i.startSeconds()));
            command.add(Double.toString(i.gainDb()));
        });
        command.add("--bass"); command.add(Double.toString(c.controls().bassDb()));
        command.add("--mid"); command.add(Double.toString(c.controls().midDb()));
        command.add("--treble"); command.add(Double.toString(c.controls().trebleDb()));
        command.add("--master-gain"); command.add(Double.toString(c.controls().masterGainDb()));
        command.add("--pan"); command.add(Double.toString(c.controls().pan()));
        command.add("--left-gain"); command.add(Double.toString(c.controls().leftGain()));
        command.add("--right-gain"); command.add(Double.toString(c.controls().rightGain()));

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
