#ifndef SLEELA_COMPRESSION_LZW_DETAIL_H
#define SLEELA_COMPRESSION_LZW_DETAIL_H

/*
 * Lempel-Ziv-Welch byte codec (C++), declared with C linkage for lzw.c.
 *
 * Variable-width LZW over a byte alphabet. Codes start at 9 bits and grow up to
 * 12 bits (dictionary cap 4096); when the dictionary fills it is reset, so the
 * scheme is bounded and streaming-safe. Self-describing header:
 *
 *   magic   : 4 bytes  "SLZW"
 *   orig_len: 8 bytes  original byte count (little-endian)
 *   payload : packed MSB-first variable-width code stream
 *
 * Codes 0..255 are the literal bytes; code 256 is CLEAR (dictionary reset).
 * Both entry points use the two-phase (size, then fill) sizing contract.
 */

#include <stddef.h>
#include <stdint.h>
#include "sleela_compressor_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

sleela_compression_result sleela_lzw_encode(const uint8_t *input, size_t input_len,
                                            uint8_t *out, size_t *out_len);

sleela_compression_result sleela_lzw_decode(const uint8_t *input, size_t input_len,
                                            uint8_t *out, size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSION_LZW_DETAIL_H */
