/*
 * bzip2 content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. bzip2: ASCII magic 'BZh' (0x42 0x5A 0x68).
 */

#include "bzip2_detail.h"

extern "C" sleela_compression_result sleela_bzip2_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 3 && data[0] == 0x42 && data[1] == 0x5A && data[2] == 0x68)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
