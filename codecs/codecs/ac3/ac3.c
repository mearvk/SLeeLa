/*
 * Dolby Digital AC-3 codec -- Backend handler.
 *
 * Dolby Digital AC-3 multichannel. Identified by the 0x0B77 sync word; encode/decode require an approved AC-3 backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in ac3_detail.cpp. The manager
 * loads it via sleela_codec_plugin_ac3().
 */

#include "sleela_codec_plugin.h"
#include "ac3_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_AC3, "Dolby Digital AC-3", "audio/ac3", ".ac3",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *ac3_describe(void) { return &k_meta; }

static sleela_codec_result ac3_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_ac3_probe_bytes(data, len);
}

static sleela_codec_result ac3_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_AC3);
    if (b && b->decode) return b->decode(input, input_len, out);
    /* No backend wired: report cleanly rather than fake a decode. Register an
     * approved library adapter via sleela_codec_backend_register(SLEELA_CODEC_AC3, ...). */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result ac3_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_AC3);
    if (b && b->encode) return b->encode(pcm, output, inout_len);
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    ac3_describe, /*load*/ 0, /*unload*/ 0, ac3_probe, ac3_decode, ac3_encode
};

const sleela_codec_plugin *sleela_codec_plugin_ac3(void) { return &k_plugin; }
