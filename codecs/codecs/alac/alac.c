/*
 * ALAC codec -- Backend handler.
 *
 * Apple Lossless Audio Codec, carried in MP4/CAF containers. Requires an approved ALAC backend for encode/decode.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in alac_detail.cpp. The manager
 * loads it via sleela_codec_plugin_alac().
 */

#include "sleela_codec_plugin.h"
#include "alac_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_ALAC, "ALAC", "audio/alac", ".m4a,.caf",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *alac_describe(void) { return &k_meta; }

static sleela_codec_result alac_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_alac_probe_bytes(data, len);
}

static sleela_codec_result alac_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result alac_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    alac_describe, /*load*/ 0, /*unload*/ 0, alac_probe, alac_decode, alac_encode
};

const sleela_codec_plugin *sleela_codec_plugin_alac(void) { return &k_plugin; }
