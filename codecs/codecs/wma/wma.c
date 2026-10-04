/*
 * Windows Media Audio codec -- Backend handler.
 *
 * Windows Media Audio family (ASF container). Requires an approved WMA backend for encode/decode.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in wma_detail.cpp. The manager
 * loads it via sleela_codec_plugin_wma().
 */

#include "sleela_codec_plugin.h"
#include "wma_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_WMA, "Windows Media Audio", "audio/x-ms-wma", ".wma",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *wma_describe(void) { return &k_meta; }

static sleela_codec_result wma_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_wma_probe_bytes(data, len);
}

static sleela_codec_result wma_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result wma_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    wma_describe, /*load*/ 0, /*unload*/ 0, wma_probe, wma_decode, wma_encode
};

const sleela_codec_plugin *sleela_codec_plugin_wma(void) { return &k_plugin; }
