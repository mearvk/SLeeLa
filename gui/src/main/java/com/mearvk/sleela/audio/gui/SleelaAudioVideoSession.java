package com.mearvk.sleela.audio.gui;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public final class SleelaAudioVideoSession implements SleelaAudioVideo {
    @FunctionalInterface public interface NativeProcessor { void process(MixConfiguration configuration); }
    private final List<Input> inputs = new ArrayList<>();
    private MixerControls controls = new MixerControls(0,0,0,0,0,List.of(1.0,1.0));
    private NativeProcessor processor;

    public SleelaAudioVideoSession() {
        String configured = System.getProperty("sleela.audio.native");
        if (configured == null || configured.isBlank()) {
            processor = configuration -> { throw new IllegalStateException("Set -Dsleela.audio.native=/path/to/sleela-audio-native"); };
        } else {
            NativeAudioProcess nativeProcess = new NativeAudioProcess(Path.of(configured));
            processor = configuration -> {
                List<Path> files = configuration.inputs().stream()
                        .filter(i -> i.sourceType() == SourceType.FILE)
                        .map(i -> Path.of(i.source()))
                        .toList();
                if (files.isEmpty()) throw new IllegalStateException("No file inputs available for native mixer");
                nativeProcess.process(configuration.output(), files);
            };
        }
    }

    public SleelaAudioVideoSession(NativeProcessor processor) { this.processor = Objects.requireNonNull(processor); }
    public void setNativeProcessor(NativeProcessor processor) { this.processor = Objects.requireNonNull(processor); }
    @Override public boolean validate(MixConfiguration c) {
        if(c==null||c.sampleRate()<=0||c.inputs().isEmpty()||c.controls()==null||c.output()==null)return false;
        MixerControls x=c.controls();
        if(!finite(x.bassDb())||!finite(x.midDb())||!finite(x.trebleDb())||!finite(x.gainDb())||!finite(x.pan()))return false;
        return c.inputs().stream().allMatch(this::validInput);
    }
    @Override public SleelaAudioVideo withInput(Input input){if(!validInput(input))throw new IllegalArgumentException("Invalid input");inputs.removeIf(x->x.id().equals(input.id()));inputs.add(input);return this;}
    @Override public SleelaAudioVideo withControls(MixerControls controls){this.controls=Objects.requireNonNull(controls);return this;}
    @Override public void processTo(Path output){MixConfiguration c=new MixConfiguration(AudioMixerModel.DEFAULT_SAMPLE_RATE,List.copyOf(inputs),controls,output);if(!validate(c))throw new IllegalArgumentException("Invalid synchronized mix configuration");processor.process(c);}
    @Override public AudioLevel audioLevel(){return new AudioLevel(0,0,0,0);}
    @Override public VideoLevel videoLevel(){return new VideoLevel(0,0,0,0,0);}
    public List<Input> inputs(){return List.copyOf(inputs);}
    public MixerControls controls(){return controls;}
    private boolean validInput(Input i){return i!=null&&i.id()!=null&&!i.id().isBlank()&&i.role()!=null&&i.sourceType()!=null&&i.source()!=null&&!i.source().isBlank()&&i.loadAtNs()>=0&&i.startAtNs()>=0&&finite(i.quality())&&i.quality()>0&&finite(i.gainDb());}
    private static boolean finite(double v){return Double.isFinite(v);}
}