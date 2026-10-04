/*
 * AAC codec -- Backend handler.
 *
 * MPEG-2/4 Advanced Audio Coding (ADTS or MP4). Requires an approved AAC backend for encode/decode.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in aac_detail.cpp. The manager
 * loads it via sleela_codec_plugin_aac().
 */

#include "sleela_codec_plugin.h"
#include "aac_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_AAC, "AAC", "audio/aac", ".aac,.m4a",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *aac_describe(void) { return &k_meta; }

static sleela_codec_result aac_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_aac_probe_bytes(data, len);
}

static sleela_codec_result aac_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result aac_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    aac_describe, /*load*/ 0, /*unload*/ 0, aac_probe, aac_decode, aac_encode
};

const sleela_codec_plugin *sleela_codec_plugin_aac(void) { return &k_plugin; }
