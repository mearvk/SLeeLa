/*
 * idea/sleela-native/sln_audio.cpp
 * Sleela-Native -- PCM mixer + WAV writer + (stub) device sink.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 * Clean-room original. SPDX-License-Identifier: MIT (see PROVENANCE.md)
 */
#include "sln_audio.h"

#include <cmath>
#include <cstdio>
#include <cstring>

namespace sln {

static const float kPi = 3.14159265358979323846f;

void Mixer::trigger(float freq, float gain, float dur_s) {
    Voice v;
    v.freq_hz = freq;
    v.gain = gain;
    v.total = (uint32_t)(dur_s * (float)rate_);
    v.pos = 0;
    voices_.push_back(v);
}

int Mixer::activeVoices() const {
    int n = 0;
    for (const auto &v : voices_) if (v.active()) ++n;
    return n;
}

int Mixer::mix(float *out, int frames) {
    for (int i = 0; i < frames * 2; ++i) out[i] = 0.0f;

    for (auto &v : voices_) {
        if (!v.active()) continue;
        for (int f = 0; f < frames && v.active(); ++f) {
            float t = (float)v.pos / (float)rate_;
            /* linear attack (5 ms) + exponential-ish decay envelope */
            float env;
            float a = 0.005f * (float)rate_;              /* attack samples */
            if ((float)v.pos < a) env = (float)v.pos / a;
            else {
                float left = (float)(v.total - v.pos) / (float)v.total;
                env = left * left;                        /* smooth tail */
            }
            float s = std::sin(2.0f * kPi * v.freq_hz * t) * v.gain * env;
            out[f * 2 + 0] += s;   /* L */
            out[f * 2 + 1] += s;   /* R */
            ++v.pos;
        }
    }

    /* soft clip to [-1,1] so summed voices never wrap */
    for (int i = 0; i < frames * 2; ++i) {
        float x = out[i];
        out[i] = (x >  1.0f) ? 1.0f : (x < -1.0f ? -1.0f : x);
    }

    /* drop finished voices */
    size_t w = 0;
    for (size_t r = 0; r < voices_.size(); ++r)
        if (voices_[r].active()) voices_[w++] = voices_[r];
    voices_.resize(w);
    return (int)voices_.size();
}

/* ---- WAV writer (16-bit PCM) ----------------------------------------- */

static void put_u32(FILE *f, uint32_t v) { fwrite(&v, 4, 1, f); }
static void put_u16(FILE *f, uint16_t v) { fwrite(&v, 2, 1, f); }

int audio_write_wav(const char *path, const float *inter, int frames,
                    int channels, int sample_rate) {
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    uint32_t data_bytes = (uint32_t)frames * channels * 2;
    uint32_t byte_rate  = (uint32_t)sample_rate * channels * 2;

    fwrite("RIFF", 1, 4, f);  put_u32(f, 36 + data_bytes);  fwrite("WAVE", 1, 4, f);
    fwrite("fmt ", 1, 4, f);  put_u32(f, 16);
    put_u16(f, 1);                              /* PCM */
    put_u16(f, (uint16_t)channels);
    put_u32(f, (uint32_t)sample_rate);
    put_u32(f, byte_rate);
    put_u16(f, (uint16_t)(channels * 2));       /* block align */
    put_u16(f, 16);                             /* bits/sample */
    fwrite("data", 1, 4, f);  put_u32(f, data_bytes);

    for (int i = 0; i < frames * channels; ++i) {
        float x = inter[i];
        if (x >  1.0f) x =  1.0f;
        if (x < -1.0f) x = -1.0f;
        int16_t s = (int16_t)lrintf(x * 32767.0f);
        put_u16(f, (uint16_t)s);
    }
    fclose(f);
    return 0;
}

/* ---- device sink: default stub (no OS backend compiled) -------------- */
/* Real backends (WASAPI/CoreAudio/ALSA) live in sln_audio_sink_<os>.cpp and
 * are selected by the build; this weakly-linked default makes the API usable
 * (and honest) when none is present. */
#if !defined(SLN_AUDIO_HAVE_SINK)
struct sln_audio_sink { int unused; };
sln_audio_sink *audio_sink_open(int, int)                 { return nullptr; }
int             audio_sink_write(sln_audio_sink *, const float *, int) { return -1; }
void            audio_sink_close(sln_audio_sink *)         {}
int             audio_sink_available(void)                 { return 0; }
#endif

} // namespace sln
