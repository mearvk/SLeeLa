#ifndef SLEELA_COMPRESSION_HUFFMAN_DETAIL_H
#define SLEELA_COMPRESSION_HUFFMAN_DETAIL_H

/*
 * Canonical static Huffman byte codec (C++), declared with C linkage for
 * huffman.c.
 *
 * Self-describing stream layout (all integers little-endian):
 *
 *   magic   : 4 bytes  "SHF1"            (SLeeLa Huffman v1)
 *   orig_len: 8 bytes  original byte count
 *   lengths : 256 bytes code length per symbol (0 = symbol absent, 1..32 used)
 *   payload : canonical-Huffman bitstream, MSB-first, of orig_len symbols
 *
 * Canonical codes are derived purely from the length table, so the decoder
 * rebuilds the same codes from the header without storing the codes themselves.
 * An empty input is encoded as header + zero-length payload and decodes back to
 * empty. Both entry points use the two-phase (size, then fill) sizing contract.
 */

#include <stddef.h>
#include <stdint.h>
#include "sleela_compressor_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

sleela_compression_result sleela_huffman_encode(const uint8_t *input, size_t input_len,
                                                uint8_t *out, size_t *out_len);

sleela_compression_result sleela_huffman_decode(const uint8_t *input, size_t input_len,
                                                uint8_t *out, size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSION_HUFFMAN_DETAIL_H */
