/*
 * AIFF content probe -- C++ implementation. Positive signature detection only;
 * actual recognized decode/encode is handled per the codec's state.
 */

#include "aiff_detail.h"

extern "C" sleela_codec_result sleela_aiff_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 4 && data[0] == 0x46 && data[1] == 0x4F && data[2] == 0x52 && data[3] == 0x4D) return SLEELA_CODEC_OK;
    if (len >= 12 && data[8] == 0x41 && data[9] == 0x49 && data[10] == 0x46 && data[11] == 0x46) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
