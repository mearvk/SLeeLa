/*
 * MIDI content probe -- C++ implementation. Positive signature detection only;
 * actual event decode/encode is handled per the codec's state.
 */

#include "midi_detail.h"

extern "C" sleela_codec_result sleela_midi_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 4 && data[0] == 0x4D && data[1] == 0x54 && data[2] == 0x68 && data[3] == 0x64) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
