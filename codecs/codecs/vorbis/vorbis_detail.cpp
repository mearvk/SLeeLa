/*
 * Ogg Vorbis content probe -- C++ implementation. Positive signature detection only;
 * actual backend decode/encode is handled per the codec's state.
 */

#include "vorbis_detail.h"

extern "C" sleela_codec_result sleela_vorbis_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_CODEC_ERR_FORMAT;
    if (len >= 4 && data[0] == 0x4F && data[1] == 0x67 && data[2] == 0x67 && data[3] == 0x53) return SLEELA_CODEC_OK;
    return SLEELA_CODEC_ERR_FORMAT;
}
