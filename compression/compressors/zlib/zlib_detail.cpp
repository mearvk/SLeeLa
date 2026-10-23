/*
 * zlib (RFC 1950) content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. zlib (RFC 1950): the two-byte header has CM=8 (deflate) in the low nibble of byte 0 and (CMF*256 + FLG) is a multiple of 31.
 */

#include "zlib_detail.h"

extern "C" sleela_compression_result sleela_zlib_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 2 && (data[0] & 0x0F) == 0x08 && ((((unsigned)data[0] << 8) | data[1]) % 31u) == 0)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
