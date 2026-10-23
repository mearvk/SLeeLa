/*
 * LZMA content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. LZMA (.lzma / alone format): a properties byte of 0x5D (lc=3,lp=0,pb=2) followed by a little-endian dictionary size.
 */

#include "lzma_detail.h"

extern "C" sleela_compression_result sleela_lzma_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 13 && data[0] == 0x5D && data[1] == 0x00 && data[2] == 0x00)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
