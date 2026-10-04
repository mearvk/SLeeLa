/*
 * FLAC codec -- Backend handler.
 *
 * Free Lossless Audio Codec (Xiph). Identified by the 'fLaC' stream marker; encode/decode require an approved FLAC backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in flac_detail.cpp. The manager
 * loads it via sleela_codec_plugin_flac().
 */

#include "sleela_codec_plugin.h"
#include "flac_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_FLAC, "FLAC", "audio/flac", ".flac",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *flac_describe(void) { return &k_meta; }

static sleela_codec_result flac_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_flac_probe_bytes(data, len);
}

static sleela_codec_result flac_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_FLAC);
    if (b && b->decode) return b->decode(input, input_len, out);
    /* No backend wired: report cleanly rather than fake a decode. Register an
     * approved library adapter via sleela_codec_backend_register(SLEELA_CODEC_FLAC, ...). */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result flac_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_FLAC);
    if (b && b->encode) return b->encode(pcm, output, inout_len);
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    flac_describe, /*load*/ 0, /*unload*/ 0, flac_probe, flac_decode, flac_encode
};

const sleela_codec_plugin *sleela_codec_plugin_flac(void) { return &k_plugin; }
