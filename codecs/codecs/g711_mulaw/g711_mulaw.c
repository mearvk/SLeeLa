/*
 * G.711 mu-law codec -- native handler.
 *
 * ITU-T G.711 mu-law companding: each byte decodes to one 16-bit PCM sample and
 * each PCM sample encodes to one byte. The companding math lives in the shared
 * C++ helper g711_detail.cpp; this C file is the plugin vtable.
 */

#include "sleela_codec_plugin.h"
#include "g711_detail.h"

#include <stdlib.h>

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_G711_MULAW, "G.711 mu-law", "audio/basic", ".au,.ulaw",
    SLEELA_CODEC_NATIVE, 1, 1
};

struct sleela_codec_instance { int16_t *owned; };

static const sleela_codec_handler *mulaw_describe(void) { return &k_meta; }

static sleela_codec_result mulaw_load(sleela_codec_instance **out) {
    sleela_codec_instance *inst = (sleela_codec_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_CODEC_ERR_IO;
    *out = inst;
    return SLEELA_CODEC_OK;
}

static void mulaw_unload(sleela_codec_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

/* G.711 is headerless telephony data; there is no reliable magic. Accept any
 * non-empty buffer as plausibly mu-law when probed explicitly. */
static sleela_codec_result mulaw_probe(sleela_codec_instance *inst,
                                       const uint8_t *data, size_t len) {
    (void)inst;
    return (data && len > 0) ? SLEELA_CODEC_OK : SLEELA_CODEC_ERR_FORMAT;
}

static sleela_codec_result mulaw_decode(sleela_codec_instance *inst,
                                        const uint8_t *input, size_t input_len,
                                        sleela_pcm_buffer *out) {
    if (!inst || !input || !out) return SLEELA_CODEC_ERR_INVALID;
    int16_t *samples = (int16_t *)malloc((input_len ? input_len : 1) * sizeof(int16_t));
    if (!samples) return SLEELA_CODEC_ERR_IO;
    for (size_t i = 0; i < input_len; ++i) samples[i] = sleela_g711_mulaw_decode(input[i]);
    free(inst->owned);
    inst->owned = samples;
    out->samples = samples;
    out->channels = 1;            /* G.711 is single-channel telephony */
    out->sample_rate = 8000;      /* standard narrowband rate */
    out->frame_count = input_len;
    return SLEELA_CODEC_OK;
}

static sleela_codec_result mulaw_encode(sleela_codec_instance *inst,
                                        const sleela_pcm_buffer *pcm,
                                        uint8_t *output, size_t *inout_len) {
    (void)inst;
    if (!pcm || !pcm->samples || !inout_len) return SLEELA_CODEC_ERR_INVALID;
    size_t samples = pcm->frame_count * (pcm->channels ? pcm->channels : 1);
    if (!output) { *inout_len = samples; return SLEELA_CODEC_OK; }
    if (*inout_len < samples) { *inout_len = samples; return SLEELA_CODEC_ERR_IO; }
    for (size_t i = 0; i < samples; ++i) output[i] = sleela_g711_mulaw_encode(pcm->samples[i]);
    *inout_len = samples;
    return SLEELA_CODEC_OK;
}

static const sleela_codec_plugin k_plugin = {
    mulaw_describe, mulaw_load, mulaw_unload, mulaw_probe, mulaw_decode, mulaw_encode
};

const sleela_codec_plugin *sleela_codec_plugin_g711_mulaw(void) { return &k_plugin; }
