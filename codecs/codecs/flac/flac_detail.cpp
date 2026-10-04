/*
 * FLAC content probe -- C++ implementation. Positive signature detection only;
 * actual backend decode/encode is handled per the codec's state.
 */

#include "flac_detail.h"

extern "C" sleela_codec_result sleela_flac_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 4 && data[0] == 0x66 && data[1] == 0x4C && data[2] == 0x61 && data[3] == 0x43) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
