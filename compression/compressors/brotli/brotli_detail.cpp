/*
 * Brotli content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. Brotli has no fixed magic signature (the stream begins with a window-size descriptor), so it is selected by id/extension, not positively probed.
 */

#include "brotli_detail.h"

extern "C" sleela_compression_result sleela_brotli_probe_bytes(const uint8_t *data, size_t len) {
    (void)data;
    (void)len;
    /* Brotli has no fixed magic: no positive probe, selected by id/extension. */
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
