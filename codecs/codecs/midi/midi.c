/*
 * MIDI codec -- Event handler.
 *
 * Standard MIDI note/event data (not sampled audio). Identified by the 'MThd' header chunk; it describes events, so there is no PCM decode.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_EVENT); the C++ signature probe is in midi_detail.cpp. The manager
 * loads it via sleela_codec_plugin_midi().
 */

#include "sleela_codec_plugin.h"
#include "midi_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_MIDI, "MIDI", "audio/midi", ".mid,.midi",
    SLEELA_CODEC_EVENT, 1, 1
};

static const sleela_codec_handler *midi_describe(void) { return &k_meta; }

static sleela_codec_result midi_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_midi_probe_bytes(data, len);
}

static sleela_codec_result midi_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst; (void)input; (void)input_len; (void)out;
    /* Metadata/identification handler: no in-package decoder. The
     * manager surfaces this as a clean 'unsupported' rather than a
     * false success; wire an approved backend to enable decode. */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result midi_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst; (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    midi_describe, /*load*/ 0, /*unload*/ 0, midi_probe, midi_decode, midi_encode
};

const sleela_codec_plugin *sleela_codec_plugin_midi(void) { return &k_plugin; }
