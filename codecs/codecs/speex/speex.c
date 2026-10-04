/*
 * Speex codec -- Backend handler.
 *
 * Xiph.Org Speex speech codec in Ogg. Identified by the container; encode/decode require an approved Speex backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in speex_detail.cpp. The manager
 * loads it via sleela_codec_plugin_speex().
 */

#include "sleela_codec_plugin.h"
#include "speex_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_SPEEX, "Speex", "audio/ogg", ".spx,.ogg",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *speex_describe(void) { return &k_meta; }

static sleela_codec_result speex_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_speex_probe_bytes(data, len);
}

static sleela_codec_result speex_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_SPEEX);
    if (b && b->decode) return b->decode(input, input_len, out);
    /* No backend wired: report cleanly rather than fake a decode. Register an
     * approved library adapter via sleela_codec_backend_register(SLEELA_CODEC_SPEEX, ...). */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result speex_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_SPEEX);
    if (b && b->encode) return b->encode(pcm, output, inout_len);
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    speex_describe, /*load*/ 0, /*unload*/ 0, speex_probe, speex_decode, speex_encode
};

const sleela_codec_plugin *sleela_codec_plugin_speex(void) { return &k_plugin; }
