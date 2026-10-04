/*
 * HE-AAC codec -- Backend handler.
 *
 * MPEG-4 High-Efficiency AAC (AAC + SBR/PS) for low bitrates. Requires an approved HE-AAC backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in he_aac_detail.cpp. The manager
 * loads it via sleela_codec_plugin_he_aac().
 */

#include "sleela_codec_plugin.h"
#include "he_aac_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_HE_AAC, "HE-AAC", "audio/aac", ".aac,.m4a",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *he_aac_describe(void) { return &k_meta; }

static sleela_codec_result he_aac_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_he_aac_probe_bytes(data, len);
}

static sleela_codec_result he_aac_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result he_aac_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    he_aac_describe, /*load*/ 0, /*unload*/ 0, he_aac_probe, he_aac_decode, he_aac_encode
};

const sleela_codec_plugin *sleela_codec_plugin_he_aac(void) { return &k_plugin; }
