/*
 * Dolby Digital AC-3 content probe -- C++ implementation. Positive signature detection only;
 * actual backend decode/encode is handled per the codec's state.
 */

#include "ac3_detail.h"

extern "C" sleela_codec_result sleela_ac3_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 2 && data[0] == 0x0B && data[1] == 0x77) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
