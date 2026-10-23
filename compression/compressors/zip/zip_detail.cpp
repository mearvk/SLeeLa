/*
 * ZIP archive content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. ZIP container: the 'PK' signature 0x50 0x4B followed by a record marker (0x03 0x04 local file, 0x05 0x06 end-of-central-directory, or 0x07 0x08 spanned).
 */

#include "zip_detail.h"

extern "C" sleela_compression_result sleela_zip_probe_bytes(const uint8_t *data, size_t len) {
    if (!data) return SLEELA_COMPRESSION_ERR_FORMAT;
    if ((len >= 4 && data[0] == 0x50 && data[1] == 0x4B && (data[2] == 0x03 || data[2] == 0x05 || data[2] == 0x07))) return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
