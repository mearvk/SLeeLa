/*
 * G.711 A-law codec -- native handler.
 *
 * ITU-T G.711 A-law companding (European telephony). One byte <-> one 16-bit
 * PCM sample. Shares the companding math with g711_mulaw via g711_detail.
 * A small A-law-specific C++ helper lives in g711_alaw.cpp.
 */

#include "sleela_codec_plugin.h"
#include "../g711_mulaw/g711_detail.h"
#include "g711_alaw_detail.h"

#include <stdlib.h>

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_G711_ALAW, "G.711 A-law", "audio/basic", ".au,.alaw",
    SLEELA_CODEC_NATIVE, 1, 1
};

struct sleela_codec_instance { int16_t *owned; };

static const sleela_codec_handler *alaw_describe(void) { return &k_meta; }

static sleela_codec_result alaw_load(sleela_codec_instance **out) {
    sleela_codec_instance *inst = (sleela_codec_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_CODEC_ERR_IO;
    *out = inst;
    return SLEELA_CODEC_OK;
}

static void alaw_unload(sleela_codec_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

static sleela_codec_result alaw_probe(sleela_codec_instance *inst,
                                      const uint8_t *data, size_t len) {
    (void)inst;
    return (data && len > 0) ? SLEELA_CODEC_OK : SLEELA_CODEC_ERR_FORMAT;
}

static sleela_codec_result alaw_decode(sleela_codec_instance *inst,
                                       const uint8_t *input, size_t input_len,
                                       sleela_pcm_buffer *out) {
    if (!inst || !input || !out) return SLEELA_CODEC_ERR_INVALID;
    int16_t *samples = (int16_t *)malloc((input_len ? input_len : 1) * sizeof(int16_t));
    if (!samples) return SLEELA_CODEC_ERR_IO;
    for (size_t i = 0; i < input_len; ++i) samples[i] = sleela_g711_alaw_decode(input[i]);
    free(inst->owned);
    inst->owned = samples;
    out->samples = samples;
    out->channels = 1;
    out->sample_rate = 8000;
    out->frame_count = input_len;
    return SLEELA_CODEC_OK;
}

static sleela_codec_result alaw_encode(sleela_codec_instance *inst,
                                       const sleela_pcm_buffer *pcm,
                                       uint8_t *output, size_t *inout_len) {
    (void)inst;
    if (!pcm || !pcm->samples || !inout_len) return SLEELA_CODEC_ERR_INVALID;
    size_t samples = pcm->frame_count * (pcm->channels ? pcm->channels : 1);
    if (!output) { *inout_len = samples; return SLEELA_CODEC_OK; }
    if (*inout_len < samples) { *inout_len = samples; return SLEELA_CODEC_ERR_IO; }
    for (size_t i = 0; i < samples; ++i) output[i] = sleela_g711_alaw_encode(pcm->samples[i]);
    *inout_len = samples;
    return SLEELA_CODEC_OK;
}

static const sleela_codec_plugin k_plugin = {
    alaw_describe, alaw_load, alaw_unload, alaw_probe, alaw_decode, alaw_encode
};

const sleela_codec_plugin *sleela_codec_plugin_g711_alaw(void) {
    (void)sleela_g711_alaw_is_european;  /* keep the A-law C++ helper referenced */
    return &k_plugin;
}
