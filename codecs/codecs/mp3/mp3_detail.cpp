/*
 * MPEG Layer III content probe -- C++ implementation. Positive signature detection only;
 * actual backend decode/encode is handled per the codec's state.
 */

#include "mp3_detail.h"

extern "C" sleela_codec_result sleela_mp3_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 3 && data[0] == 0x49 && data[1] == 0x44 && data[2] == 0x33) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
