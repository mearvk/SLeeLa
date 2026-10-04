/*
 * Opus codec -- Backend handler.
 *
 * IETF Opus (RFC 6716), usually in Ogg or WebM. Identified by the container page/marker; encode/decode require an approved Opus backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in opus_detail.cpp. The manager
 * loads it via sleela_codec_plugin_opus().
 */

#include "sleela_codec_plugin.h"
#include "opus_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_OPUS, "Opus", "audio/opus", ".opus,.ogg,.webm",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *opus_describe(void) { return &k_meta; }

static sleela_codec_result opus_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_opus_probe_bytes(data, len);
}

static sleela_codec_result opus_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_OPUS);
    if (b && b->decode) return b->decode(input, input_len, out);
    /* No backend wired: report cleanly rather than fake a decode. Register an
     * approved library adapter via sleela_codec_backend_register(SLEELA_CODEC_OPUS, ...). */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result opus_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_OPUS);
    if (b && b->encode) return b->encode(pcm, output, inout_len);
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    opus_describe, /*load*/ 0, /*unload*/ 0, opus_probe, opus_decode, opus_encode
};

const sleela_codec_plugin *sleela_codec_plugin_opus(void) { return &k_plugin; }
