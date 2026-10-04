#ifndef SLEELA_CODEC_AIFF_DETAIL_H
#define SLEELA_CODEC_AIFF_DETAIL_H

/*
 * AIFF (FORM/AIFF) container detail, implemented in aiff_detail.cpp (C++) with C
 * linkage. AIFF carries big-endian PCM and stores the sample rate as an IEEE-754
 * 80-bit extended float in the COMM chunk.
 */

#include <stddef.h>
#include <stdint.h>
#include "sleela_codec_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Canonical header size this writer emits: FORM(12) + COMM(26) + SSND hdr(16). */
#define SLEELA_AIFF_HEADER_SIZE 54

typedef struct sleela_aiff_format {
    uint16_t channels;
    uint32_t sample_rate;
    uint16_t bits_per_sample;
    uint32_t frames;
} sleela_aiff_format;

/* Probe: FORM....AIFF signature. */
sleela_codec_result sleela_aiff_probe_bytes(const uint8_t *data, size_t len);

/* Parse a FORM/AIFF buffer: fill `fmt` from COMM and point `*out_pcm` at the
 * SSND sample bytes (`*out_pcm_bytes` long). */
sleela_codec_result sleela_aiff_parse(const uint8_t *data, size_t len,
                                      sleela_aiff_format *fmt,
                                      const uint8_t **out_pcm, size_t *out_pcm_bytes);

/* Write the 54-byte canonical AIFF header into `dst`. */
void sleela_aiff_write_header(uint8_t *dst, uint16_t channels, uint32_t sample_rate,
                              uint16_t bits_per_sample, uint32_t frames, size_t data_bytes);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_AIFF_DETAIL_H */
