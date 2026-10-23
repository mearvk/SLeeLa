/*
 * DEFLATE (RFC 1951) content probe -- C++ implementation. Positive signature detection
 * only; actual backend compress/decompress is handled per the algorithm's
 * state. Raw DEFLATE (RFC 1951) has no header, so there is no reliable signature; it is selected by id/extension, not positively probed.
 */

#include "deflate_detail.h"

extern "C" sleela_compression_result sleela_deflate_probe_bytes(const uint8_t *data, size_t len) {
    (void)data;
    (void)len;
    /* Raw DEFLATE is headerless: no reliable signature, so never a positive probe. */
    return SLEELA_COMPRESSION_ERR_FORMAT;
}
