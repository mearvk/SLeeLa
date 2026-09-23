package com.mearvk.sleela.audio.gui;

import java.util.ArrayList;
import java.util.List;

/**
 * Java-side presentation model for the native SLeeLa synchronized audio mixer.
 *
 * This class intentionally contains no native bindings. A future SLeeLa
 * mapping can replace or back this model while keeping the JavaFX view stable.
 */
public final class AudioMixerModel {
    public static final int DEFAULT_SAMPLE_RATE = 48_000;

    public record Track(String id, String role, String source, double startSeconds,
                        double quality, double gainDb) {}

    private final List<Track> tracks = new ArrayList<>();
    private double bassDb = 3.0;
    private double midDb = -1.0;
    private double trebleDb = 2.0;
    private double masterGainDb = 0.0;
    private double pan = 0.0;

    public AudioMixerModel() {
        tracks.add(new Track("master", "Master", "audio/master.wav", 0.0, 1.00, 0.0));
        tracks.add(new Track("second", "Second", "audio/second.flac", 0.125, 0.92, -3.0));
        tracks.add(new Track("live", "Live Input", "default", 0.500, 1.00, -6.0));
    }

    public List<Track> tracks() { return List.copyOf(tracks); }
    public double bassDb() { return bassDb; }
    public double midDb() { return midDb; }
    public double trebleDb() { return trebleDb; }
    public double masterGainDb() { return masterGainDb; }
    public double pan() { return pan; }

    public void setBassDb(double value) { bassDb = value; }
    public void setMidDb(double value) { midDb = value; }
    public void setTrebleDb(double value) { trebleDb = value; }
    public void setMasterGainDb(double value) { masterGainDb = value; }
    public void setPan(double value) { pan = value; }

    public String summary() {
        return String.format(
                "48 kHz • %d tracks • Bass %+.1f dB • Mid %+.1f dB • Treble %+.1f dB • Gain %+.1f dB",
                tracks.size(), bassDb, midDb, trebleDb, masterGainDb);
    }
}
