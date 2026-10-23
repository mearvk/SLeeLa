/*
 * gzip (RFC 1952) content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. gzip (RFC 1952): magic 0x1F 0x8B followed by compression method 0x08 (deflate).
 */

#include "gzip_detail.h"

extern "C" sleela_compression_result sleela_gzip_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 3 && data[0] == 0x1F && data[1] == 0x8B && data[2] == 0x08)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
