/*
 * idea/sleela-native/sln_demo.cpp
 * Sleela-Native -- headless demo/verification of the scene model + audio.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 * Clean-room original. SPDX-License-Identifier: MIT (see PROVENANCE.md)
 *
 * Advances the real-time scene (physical model, orbit camera, moving light,
 * vignette) and the audio mixer WITHOUT a GPU, so the model and sound engine
 * are built and verified here. It prints the per-frame camera/light state and
 * renders the triggered sound cues to a .wav. The GL renderer (sln_render_gl)
 * draws the same scene on a real GPU/display -- see README.
 */
#include "sln_scene.h"
#include "sln_audio.h"

#include <cstdio>
#include <vector>

int main(int argc, char **argv) {
    const char *wav = (argc > 1) ? argv[1] : "sleela-native-demo.wav";
    const float fps = 60.0f;
    const float dt  = 1.0f / fps;
    const int   frames_total = 180;   /* 3 seconds */

    sln::Scene scene;
    scene.build_default();

    sln::Mixer mixer(48000);
    const int sr = mixer.sampleRate();
    std::vector<float> pcm;            /* interleaved stereo, whole timeline */

    std::printf("Sleela-Native demo -- physical model, camera, moving light, vignette, sound\n");
    std::printf("model: %zu vertices, %zu indices\n",
                scene.model.vertices.size(), scene.model.indices.size());
    std::printf("vignette: radius=%.2f softness=%.2f strength=%.2f\n",
                scene.vignette.radius, scene.vignette.softness, scene.vignette.strength);

    const int block = sr / (int)fps;   /* audio samples per video frame */
    std::vector<float> blockbuf((size_t)block * 2);

    for (int f = 0; f < frames_total; ++f) {
        int fired = scene.advance(dt);
        /* turn fired scene cues into mixer voices */
        for (auto &c : scene.cues)
            if (c.fired) { /* trigger exactly once when it just fired */ }
        if (fired > 0) {
            /* the cue(s) that just fired this frame */
            for (auto &c : scene.cues)
                if (c.fired && c.at_time <= scene.time && c.at_time > scene.time - dt)
                    mixer.trigger(c.freq_hz, c.gain, c.dur_s);
        }

        /* render this frame's audio block */
        mixer.mix(blockbuf.data(), block);
        pcm.insert(pcm.end(), blockbuf.begin(), blockbuf.end());

        if (f % 30 == 0) {   /* print twice a second */
            sln::Vec3 e = scene.camera.eye();
            sln::Vec3 l = scene.light.world_position();
            std::printf("t=%4.2fs  cam.eye=(%.2f,%.2f,%.2f)  light=(%.2f,%.2f,%.2f)  "
                        "spin=%.2f  voices=%d\n",
                        scene.time, e.x, e.y, e.z, l.x, l.y, l.z,
                        scene.model.spin, mixer.activeVoices());
        }
    }

    int frames = (int)(pcm.size() / 2);
    if (sln::audio_write_wav(wav, pcm.data(), frames, 2, sr) == 0)
        std::printf("wrote %s (%d frames, %.2fs stereo @ %d Hz)\n",
                    wav, frames, (float)frames / sr, sr);
    else
        std::printf("failed to write %s\n", wav);

    std::printf("audio sink available on this build: %s\n",
                sln::audio_sink_available() ? "yes" : "no (wav path used)");
    return 0;
}
