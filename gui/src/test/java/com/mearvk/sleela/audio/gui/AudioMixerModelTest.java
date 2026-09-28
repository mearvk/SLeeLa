package com.mearvk.sleela.audio.gui;

public final class AudioMixerModelTest {
    public static void main(String[] args) {
        AudioMixerModel model = new AudioMixerModel();
        require(model.tracks().size() == 3, "default track count");
        model.setTrackGain("second", -9.0);
        require(model.snapshotTracks().get(1).gainDb() == -9.0, "track gain update");
        model.setPan(0.25);
        require(model.pan() == 0.25, "pan update");
        require(model.summary().contains("3 tracks"), "summary");
    }
    private static void require(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }
}
