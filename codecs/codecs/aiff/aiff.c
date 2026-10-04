/*
 * AIFF codec -- Recognized handler.
 *
 * Audio Interchange File Format (Apple). Recognized and identified by its FORM/AIFF container signature; sample decode is not claimed.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_RECOGNIZED); the C++ signature probe is in aiff_detail.cpp. The manager
 * loads it via sleela_codec_plugin_aiff().
 */

#include "sleela_codec_plugin.h"
#include "aiff_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_AIFF, "AIFF", "audio/aiff", ".aif,.aiff",
    SLEELA_CODEC_RECOGNIZED, 0, 0
};

static const sleela_codec_handler *aiff_describe(void) { return &k_meta; }

static sleela_codec_result aiff_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_aiff_probe_bytes(data, len);
}

static sleela_codec_result aiff_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result aiff_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    aiff_describe, /*load*/ 0, /*unload*/ 0, aiff_probe, aiff_decode, aiff_encode
};

const sleela_codec_plugin *sleela_codec_plugin_aiff(void) { return &k_plugin; }
