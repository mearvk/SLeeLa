package com.mearvk.sleela.audio.gui;

import java.nio.file.Path;

public final class SleelaAudioVideoSessionTest {
    public static void main(String[] args) {
        SleelaAudioVideoSession session = new SleelaAudioVideoSession();
        session.withInput(new SleelaAudioVideo.Input("master", SleelaAudioVideo.TrackRole.MASTER,
                SleelaAudioVideo.SourceType.FILE, "master.wav", 0, 0, 1.0, 0.0));
        final boolean[] called = {false};
        session.setNativeProcessor(configuration -> {
            called[0] = true;
            require(configuration.output().equals(Path.of("out.wav")), "output");
            require(configuration.inputs().size() == 1, "input count");
        });
        session.processTo(Path.of("out.wav"));
        require(called[0], "native processor invocation");
    }
    private static void require(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }
}
