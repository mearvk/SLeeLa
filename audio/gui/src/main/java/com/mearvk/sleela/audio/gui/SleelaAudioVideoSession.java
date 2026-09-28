package com.mearvk.sleela.audio.gui;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public final class SleelaAudioVideoSession implements SleelaAudioVideo {
    @FunctionalInterface
    public interface NativeProcessor { void process(MixConfiguration configuration); }

    private final List<Input> inputs = new ArrayList<>();
    private MixerControls controls = new MixerControls(0.0, 0.0, 0.0, 0.0, 0.0, List.of(1.0, 1.0));
    private NativeProcessor processor;

    public SleelaAudioVideoSession() {
        this(configuration -> {
            throw new IllegalStateException("SLeeLa native Audio/Video backend is not connected");
        });
    }

    public SleelaAudioVideoSession(NativeProcessor processor) {
        this.processor = Objects.requireNonNull(processor, "processor");
    }

    public void setNativeProcessor(NativeProcessor processor) {
        this.processor = Objects.requireNonNull(processor, "processor");
    }

    @Override
    public boolean validate(MixConfiguration configuration) {
        if (configuration == null || configuration.sampleRate() <= 0
                || configuration.inputs().isEmpty()
                || configuration.controls() == null
                || configuration.output() == null) return false;
        MixerControls c = configuration.controls();
        if (!finite(c.bassDb()) || !finite(c.midDb()) || !finite(c.trebleDb())
                || !finite(c.gainDb()) || !finite(c.pan())) return false;
        return configuration.inputs().stream().allMatch(this::validInput);
    }

    @Override
    public SleelaAudioVideo withInput(Input input) {
        if (!validInput(input)) throw new IllegalArgumentException("Invalid input");
        inputs.removeIf(existing -> existing.id().equals(input.id()));
        inputs.add(input);
        return this;
    }

    @Override
    public SleelaAudioVideo withControls(MixerControls controls) {
        this.controls = Objects.requireNonNull(controls, "controls");
        return this;
    }

    @Override
    public void processTo(Path output) {
        MixConfiguration configuration = new MixConfiguration(
                AudioMixerModel.DEFAULT_SAMPLE_RATE, List.copyOf(inputs), controls, output);
        if (!validate(configuration)) throw new IllegalArgumentException("Invalid synchronized mix configuration");
        processor.process(configuration);
    }

    @Override public AudioLevel audioLevel() { return new AudioLevel(0.0, 0.0, 0.0, 0.0); }
    @Override public VideoLevel videoLevel() { return new VideoLevel(0, 0, 0.0, 0.0, 0.0); }
    public List<Input> inputs() { return List.copyOf(inputs); }
    public MixerControls controls() { return controls; }

    private boolean validInput(Input input) {
        return input != null && input.id() != null && !input.id().isBlank()
                && input.role() != null && input.sourceType() != null
                && input.source() != null && !input.source().isBlank()
                && input.loadAtNs() >= 0 && input.startAtNs() >= 0
                && finite(input.quality()) && input.quality() > 0.0
                && finite(input.gainDb());
    }

    private static boolean finite(double value) { return Double.isFinite(value); }
}
