package com.mearvk.sleela.audio.gui;

import java.nio.file.Path;
import java.util.List;

/**
 * Clean Java-facing signature for SLeeLa Audio/Video operations.
 *
 * <p>This is deliberately a presentation/integration contract. Native media
 * acquisition, decoding, synchronization, mixing, and output remain owned by
 * SLeeLa's native implementation.
 */
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

    /** Validate the Java-facing configuration before native admission. */
    boolean validate(MixConfiguration configuration);

    /** Add or replace an input in the synchronized mix configuration. */
    SleelaAudioVideo withInput(Input input);

    /** Apply mixer controls to the pending synchronized mix. */
    SleelaAudioVideo withControls(MixerControls controls);

    /** Process and persist synchronized output through the SLeeLa runtime. */
    void processTo(Path output);

    /** Obtain the current audio analysis result for the presentation layer. */
    AudioLevel audioLevel();

    /** Obtain the current video analysis result for the presentation layer. */
    VideoLevel videoLevel();
}
