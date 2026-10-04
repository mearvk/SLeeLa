/*
 * Dolby Digital Plus codec -- Backend handler.
 *
 * Dolby Digital Plus (E-AC-3). Shares the 0x0B77 sync word with AC-3; encode/decode require an approved E-AC-3 backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in eac3_detail.cpp. The manager
 * loads it via sleela_codec_plugin_eac3().
 */

#include "sleela_codec_plugin.h"
#include "eac3_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_EAC3, "Dolby Digital Plus", "audio/eac3", ".eac3,.ec3",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *eac3_describe(void) { return &k_meta; }

static sleela_codec_result eac3_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_eac3_probe_bytes(data, len);
}

static sleela_codec_result eac3_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_EAC3);
    if (b && b->decode) return b->decode(input, input_len, out);
    /* No backend wired: report cleanly rather than fake a decode. Register an
     * approved library adapter via sleela_codec_backend_register(SLEELA_CODEC_EAC3, ...). */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result eac3_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_EAC3);
    if (b && b->encode) return b->encode(pcm, output, inout_len);
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    eac3_describe, /*load*/ 0, /*unload*/ 0, eac3_probe, eac3_decode, eac3_encode
};

const sleela_codec_plugin *sleela_codec_plugin_eac3(void) { return &k_plugin; }
