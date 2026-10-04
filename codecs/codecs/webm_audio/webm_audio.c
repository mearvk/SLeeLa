/*
 * WebM Audio codec -- Container handler.
 *
 * WebM container (a Matroska profile). Shares the EBML header; carries Opus or Vorbis audio rather than compressing itself.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_CONTAINER); the C++ signature probe is in webm_audio_detail.cpp. The manager
 * loads it via sleela_codec_plugin_webm_audio().
 */

#include "sleela_codec_plugin.h"
#include "webm_audio_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_WEBM_AUDIO, "WebM Audio", "audio/webm", ".webm",
    SLEELA_CODEC_CONTAINER, 1, 1
};

static const sleela_codec_handler *webm_audio_describe(void) { return &k_meta; }

static sleela_codec_result webm_audio_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_webm_audio_probe_bytes(data, len);
}

static sleela_codec_result webm_audio_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result webm_audio_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    webm_audio_describe, /*load*/ 0, /*unload*/ 0, webm_audio_probe, webm_audio_decode, webm_audio_encode
};

const sleela_codec_plugin *sleela_codec_plugin_webm_audio(void) { return &k_plugin; }
