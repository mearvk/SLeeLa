package com.mearvk.sleela.audio.gui;

import java.nio.file.Path;
import java.util.List;

public interface SleelaAudioVideo {
    enum TrackRole { MASTER, SECOND, INPUT }
    enum SourceType { FILE, LIVE }
    record Input(String id, TrackRole role, SourceType sourceType, String source,
                 long loadAtNs, long startAtNs, double quality, double gainDb) {}
    record MixerControls(double bassDb, double midDb, double trebleDb,
                         double gainDb, double pan, List<Double> channelGains) {}
    record MixConfiguration(int sampleRate, List<Input> inputs,
                            MixerControls controls, Path output) {}
    record AudioLevel(double level, double peak, double rms, double dominantHz) {}
    record VideoLevel(int width, int height, double motion,
                      double luminance, double edgeDensity) {}

    boolean validate(MixConfiguration configuration);
    SleelaAudioVideo withInput(Input input);
    SleelaAudioVideo withControls(MixerControls controls);
    void processTo(Path output);
    AudioLevel audioLevel();
    VideoLevel videoLevel();
}
