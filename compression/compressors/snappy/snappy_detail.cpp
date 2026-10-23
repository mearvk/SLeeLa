/*
 * Snappy content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. Snappy framing format: a stream-identifier chunk 0xFF 0x06 0x00 0x00 followed by 'sN' ('sNaPpY').
 */

#include "snappy_detail.h"

extern "C" sleela_compression_result sleela_snappy_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 6 && data[0] == 0xFF && data[1] == 0x06 && data[2] == 0x00 && data[3] == 0x00 && data[4] == 0x73 && data[5] == 0x4E)) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
