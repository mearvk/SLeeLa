/*
 * xz (LZMA2) content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. xz: the six-byte magic 0xFD '7zXZ' 0x00.
 */

#include "xz_detail.h"

extern "C" sleela_compression_result sleela_xz_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 6 && data[0] == 0xFD && data[1] == 0x37 && data[2] == 0x7A && data[3] == 0x58 && data[4] == 0x5A && data[5] == 0x00)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
