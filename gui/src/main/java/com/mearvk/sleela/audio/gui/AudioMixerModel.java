package com.mearvk.sleela.audio.gui;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import java.util.List;

public final class AudioMixerModel {
    public static final int DEFAULT_SAMPLE_RATE = 48_000;

    public record Track(String id, String role, String source, double startSeconds,
                        double quality, double gainDb) {
        public Track {
            if (id == null || id.isBlank()) throw new IllegalArgumentException("id");
            if (role == null || role.isBlank()) throw new IllegalArgumentException("role");
            if (source == null || source.isBlank()) throw new IllegalArgumentException("source");
            if (startSeconds < 0 || !Double.isFinite(startSeconds)) throw new IllegalArgumentException("startSeconds");
            if (quality <= 0 || !Double.isFinite(quality)) throw new IllegalArgumentException("quality");
            if (!Double.isFinite(gainDb)) throw new IllegalArgumentException("gainDb");
        }
    }

    private final ObservableList<Track> tracks = FXCollections.observableArrayList(
            new Track("master", "Master", "audio/master.wav", 0.0, 1.00, 0.0),
            new Track("second", "Second", "audio/second.flac", 0.125, 0.92, -3.0),
            new Track("live", "Live Input", "default", 0.500, 1.00, -6.0));
    private double bassDb = 3.0, midDb = -1.0, trebleDb = 2.0, masterGainDb, pan;

    public ObservableList<Track> tracks() { return tracks; }
    public List<Track> snapshotTracks() { return List.copyOf(tracks); }
    public double bassDb() { return bassDb; }
    public double midDb() { return midDb; }
    public double trebleDb() { return trebleDb; }
    public double masterGainDb() { return masterGainDb; }
    public double pan() { return pan; }

    public void setBassDb(double value) { bassDb = finite(value, "bassDb"); }
    public void setMidDb(double value) { midDb = finite(value, "midDb"); }
    public void setTrebleDb(double value) { trebleDb = finite(value, "trebleDb"); }
    public void setMasterGainDb(double value) { masterGainDb = finite(value, "masterGainDb"); }
    public void setPan(double value) { pan = finite(value, "pan"); }

    public void setTrackGain(String id, double gainDb) {
        if (id == null || id.isBlank()) throw new IllegalArgumentException("id");
        gainDb = finite(gainDb, "gainDb");
        for (int i = 0; i < tracks.size(); i++) {
            Track t = tracks.get(i);
            if (t.id().equals(id)) {
                tracks.set(i, new Track(t.id(), t.role(), t.source(), t.startSeconds(), t.quality(), gainDb));
                return;
            }
        }
        throw new IllegalArgumentException("Unknown track: " + id);
    }

    public String summary() {
        return String.format(java.util.Locale.ROOT,
                "%d tracks • %d Hz • Bass %+.1f dB • Mid %+.1f dB • Treble %+.1f dB • Gain %+.1f dB",
                tracks.size(), DEFAULT_SAMPLE_RATE, bassDb, midDb, trebleDb, masterGainDb);
    }

    private static double finite(double value, String name) {
        if (!Double.isFinite(value)) throw new IllegalArgumentException(name);
        return value;
    }
}
