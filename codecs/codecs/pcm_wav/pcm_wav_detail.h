#ifndef SLEELA_CODEC_PCM_WAV_DETAIL_H
#define SLEELA_CODEC_PCM_WAV_DETAIL_H

/*
 * PCM/WAV byte-level detail. Implemented in pcm_wav.cpp (C++), declared with C
 * linkage so the plugin's C file (pcm_wav.c) can call it. This is the "C and
 * C++ for the codec itself": the vtable glue is C, the RIFF container scanning
 * is C++.
 */

#include <stddef.h>
#include <stdint.h>

#include "sleela_codec_plugin.h"   /* sleela_codec_result */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sleela_wav_format {
    uint16_t audio_format;    /* 1 = PCM */
    uint16_t channels;
    uint32_t sample_rate;
    uint16_t bits_per_sample;
} sleela_wav_format;

/* Parse a RIFF/WAVE buffer: fill `fmt` from the "fmt " chunk and point
 * `*out_pcm` at the "data" chunk payload with `*out_pcm_bytes` its length. */
sleela_codec_result sleela_wav_parse(const uint8_t *data, size_t len,
                                     sleela_wav_format *fmt,
                                     const uint8_t **out_pcm, size_t *out_pcm_bytes);

/* Write a 44-byte canonical PCM WAVE header into `dst` (must have >= 44 bytes). */
void sleela_wav_write_header(uint8_t *dst, uint16_t channels, uint32_t sample_rate,
                             uint16_t bits_per_sample, size_t data_bytes);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_PCM_WAV_DETAIL_H */
