/*
 * WebM Audio content probe -- C++ implementation. Positive signature detection only;
 * actual container decode/encode is handled per the codec's state.
 */

#include "webm_audio_detail.h"

extern "C" sleela_codec_result sleela_webm_audio_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 4 && data[0] == 0x1A && data[1] == 0x45 && data[2] == 0xDF && data[3] == 0xA3) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
