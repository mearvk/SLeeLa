/*
 * idea/sleela-native/sln_audio.h
 * Sleela-Native -- original PCM audio mixer + output interface.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Clean-room, original code. The mixer synthesises and sums voices into a
 * float PCM buffer entirely in native memory (no third-party audio library),
 * and is fully built and verified here. Actual device output is a thin per-OS
 * backend (WASAPI / CoreAudio / ALSA) behind sln_audio_sink -- those are
 * written-but-unverified in the sandbox and can also be bypassed by writing a
 * .wav (sln_audio_write_wav), which IS verified here.
 *
 * SPDX-License-Identifier: MIT  (see PROVENANCE.md)
 */
#ifndef SLN_AUDIO_H
#define SLN_AUDIO_H

#include <cstdint>
#include <vector>

namespace sln {

/* A short synthesised voice: a sine with a quick attack/decay envelope. */
struct Voice {
    float    freq_hz = 220.0f;
    float    gain    = 0.4f;
    uint32_t total   = 0;   /* total samples */
    uint32_t pos     = 0;   /* samples played */
    bool active() const { return pos < total; }
};

/* The mixer: a fixed sample rate, a set of live voices, and a scratch buffer.
 * mix() advances the voices and sums them into interleaved stereo float PCM. */
class Mixer {
public:
    explicit Mixer(int sample_rate = 48000) : rate_(sample_rate) {}

    int  sampleRate() const { return rate_; }

    /* Trigger a tone of `freq` Hz for `dur_s` seconds at `gain`. */
    void trigger(float freq, float gain, float dur_s);

    /* Render `frames` stereo frames into `out` (length frames*2). Returns the
     * number of voices still active afterward. */
    int  mix(float *out, int frames);

    /* Number of currently-active voices. */
    int  activeVoices() const;

private:
    int rate_;
    std::vector<Voice> voices_;
    double phase_accum_ = 0.0;  /* for a continuous waveform across blocks */
};

/* Write a mono/stereo float buffer to a 16-bit PCM .wav file (verified path,
 * no device needed). Returns 0 on success. `interleaved` has frames*channels
 * samples in [-1,1]. */
int audio_write_wav(const char *path, const float *interleaved,
                    int frames, int channels, int sample_rate);

/* ---- device sink (per-OS, optional) ---------------------------------- */
/* Opaque handle to an OS audio device. Opening/closing is implemented per OS
 * in sln_audio_sink_*.cpp; without a backend these report unavailable. */
typedef struct sln_audio_sink sln_audio_sink;

sln_audio_sink *audio_sink_open(int sample_rate, int channels);
int             audio_sink_write(sln_audio_sink *s, const float *interleaved, int frames);
void            audio_sink_close(sln_audio_sink *s);
int             audio_sink_available(void);   /* 1 if a real backend exists */

} // namespace sln

#endif /* SLN_AUDIO_H */
