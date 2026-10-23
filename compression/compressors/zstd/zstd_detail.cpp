/*
 * Zstandard content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. Zstandard: the frame magic 0x28 0xB5 0x2F 0xFD (little-endian 0xFD2FB528).
 */

#include "zstd_detail.h"

extern "C" sleela_compression_result sleela_zstd_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 4 && data[0] == 0x28 && data[1] == 0xB5 && data[2] == 0x2F && data[3] == 0xFD)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
