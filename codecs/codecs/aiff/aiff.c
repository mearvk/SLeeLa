/*
 * AIFF codec -- native handler.
 *
 * Audio Interchange File Format (Apple): uncompressed PCM in a FORM/AIFF
 * container with big-endian samples. Like PCM/WAV this is pure byte
 * manipulation and needs no third-party library, so AIFF is implemented
 * natively (not merely recognized). The container parsing/emission lives in the
 * C++ detail (aiff_detail.cpp); this C file is the plugin vtable.
 */

#include "sleela_codec_plugin.h"
#include "aiff_detail.h"

#include <stdlib.h>

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_AIFF, "AIFF", "audio/aiff", ".aif,.aiff",
    SLEELA_CODEC_NATIVE, 1, 1
};

struct sleela_codec_instance { int16_t *owned; };

static const sleela_codec_handler *aiff_describe(void) { return &k_meta; }

static sleela_codec_result aiff_load(sleela_codec_instance **out) {
    sleela_codec_instance *inst = (sleela_codec_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_CODEC_ERR_IO;
    *out = inst;
    return SLEELA_CODEC_OK;
}

static void aiff_unload(sleela_codec_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

static sleela_codec_result aiff_probe(sleela_codec_instance *inst,
                                      const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_aiff_probe_bytes(data, len);
}

static sleela_codec_result aiff_decode(sleela_codec_instance *inst,
                                       const uint8_t *input, size_t input_len,
                                       sleela_pcm_buffer *out) {
    if (!inst || !input || !out) return SLEELA_CODEC_ERR_INVALID;
    sleela_aiff_format fmt;
    const uint8_t *pcm; size_t pcm_bytes;
    sleela_codec_result r = sleela_aiff_parse(input, input_len, &fmt, &pcm, &pcm_bytes);
    if (r != SLEELA_CODEC_OK) return r;
    if (fmt.bits_per_sample != 16) return SLEELA_CODEC_ERR_UNSUPPORTED;

    size_t sample_count = pcm_bytes / 2;
    int16_t *samples = (int16_t *)malloc((sample_count ? sample_count : 1) * sizeof(int16_t));
    if (!samples) return SLEELA_CODEC_ERR_IO;
    for (size_t i = 0; i < sample_count; ++i)  /* AIFF PCM is big-endian */
        samples[i] = (int16_t)(((uint16_t)pcm[i * 2] << 8) | (uint16_t)pcm[i * 2 + 1]);

    free(inst->owned);
    inst->owned = samples;
    out->samples = samples;
    out->channels = fmt.channels ? fmt.channels : 1;
    out->sample_rate = fmt.sample_rate ? fmt.sample_rate : 44100;
    out->frame_count = sample_count / (out->channels ? out->channels : 1);
    return SLEELA_CODEC_OK;
}

static sleela_codec_result aiff_encode(sleela_codec_instance *inst,
                                       const sleela_pcm_buffer *pcm,
                                       uint8_t *output, size_t *inout_len) {
    (void)inst;
    if (!pcm || !pcm->samples || !inout_len) return SLEELA_CODEC_ERR_INVALID;
    size_t samples = pcm->frame_count * (pcm->channels ? pcm->channels : 1);
    size_t needed = SLEELA_AIFF_HEADER_SIZE + samples * 2;
    if (!output) { *inout_len = needed; return SLEELA_CODEC_OK; }
    if (*inout_len < needed) { *inout_len = needed; return SLEELA_CODEC_ERR_IO; }

    sleela_aiff_write_header(output, pcm->channels ? pcm->channels : 1,
                             pcm->sample_rate ? pcm->sample_rate : 44100, 16,
                             pcm->frame_count, samples * 2);
    uint8_t *dst = output + SLEELA_AIFF_HEADER_SIZE;
    for (size_t i = 0; i < samples; ++i) {       /* big-endian 16-bit */
        uint16_t s = (uint16_t)pcm->samples[i];
        dst[i * 2]     = (uint8_t)(s >> 8);
        dst[i * 2 + 1] = (uint8_t)(s & 0xFF);
    }
    *inout_len = needed;
    return SLEELA_CODEC_OK;
}

static const sleela_codec_plugin k_plugin = {
    aiff_describe, aiff_load, aiff_unload, aiff_probe, aiff_decode, aiff_encode
};

const sleela_codec_plugin *sleela_codec_plugin_aiff(void) { return &k_plugin; }
