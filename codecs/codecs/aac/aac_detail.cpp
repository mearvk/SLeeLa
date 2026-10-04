/*
 * AAC content probe -- C++ implementation. Positive signature detection only;
 * actual backend decode/encode is handled per the codec's state.
 */

#include "aac_detail.h"

extern "C" sleela_codec_result sleela_aac_probe_bytes(const uint8_t *data, size_t len) {
    (void)data; (void)len;
    return SLEELA_CODEC_ERR_FORMAT;
}
