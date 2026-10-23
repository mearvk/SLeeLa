/*
 * LZ4 content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. LZ4 frame format: the magic 0x04 0x22 0x4D 0x18 (little-endian 0x184D2204).
 */

#include "lz4_detail.h"

extern "C" sleela_compression_result sleela_lz4_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 4 && data[0] == 0x04 && data[1] == 0x22 && data[2] == 0x4D && data[3] == 0x18)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
