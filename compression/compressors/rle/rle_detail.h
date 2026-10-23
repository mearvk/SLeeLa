#ifndef SLEELA_COMPRESSION_RLE_DETAIL_H
#define SLEELA_COMPRESSION_RLE_DETAIL_H

/*
 * Run-length encoding byte codec (C++), declared with C linkage for rle.c.
 *
 * The scheme is a PackBits-style control-byte RLE that is self-describing and
 * lossless for any input:
 *
 *   control n in [0x00..0x7F]  -> literal run: the next (n + 1) bytes verbatim
 *   control n in [0x80..0xFF]  -> repeat run:  one following byte repeated
 *                                 (257 - n) times, i.e. 2..128 copies
 *
 * Both entry points are two-phase: pass out == NULL to learn the exact output
 * size (written to *out_len), then call again with a buffer of that size. This
 * keeps allocation in the caller/plugin and lets the manager validate sizes.
 */

#include <stddef.h>
#include <stdint.h>
#include "sleela_compressor_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Compute/produce the RLE encoding of `input`. With out==NULL, sets *out_len to
 * the required byte count and returns OK. With out!=NULL, *out_len is the
 * capacity on entry and the bytes produced on return. */
sleela_compression_result sleela_rle_encode(const uint8_t *input, size_t input_len,
                                            uint8_t *out, size_t *out_len);

/* Inverse of sleela_rle_encode, same two-phase sizing contract. Returns
 * SLEELA_COMPRESSION_ERR_FORMAT on a truncated/malformed stream. */
sleela_compression_result sleela_rle_decode(const uint8_t *input, size_t input_len,
                                            uint8_t *out, size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSION_RLE_DETAIL_H */
