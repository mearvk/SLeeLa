/*
 * Run-length encoding compressor -- native handler.
 *
 * Lossless PackBits-style RLE. The byte-level encode/decode lives in the shared
 * C++ helper rle_detail.cpp; this C file is the plugin vtable and owns the
 * output buffer across a load/use/unload cycle. Native: no external backend.
 */

#include "sleela_compressor_plugin.h"
#include "rle_detail.h"

#include <stdlib.h>

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_RLE, "Run-Length Encoding", "application/x-rle", ".rle",
    SLEELA_COMPRESSION_NATIVE, 1, 1
};

struct sleela_compressor_instance { uint8_t *owned; };

static const sleela_compressor_handler *rle_describe(void) { return &k_meta; }

static sleela_compression_result rle_load(sleela_compressor_instance **out) {
    sleela_compressor_instance *inst = (sleela_compressor_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_COMPRESSION_ERR_IO;
    *out = inst;
    return SLEELA_COMPRESSION_OK;
}

static void rle_unload(sleela_compressor_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

/* RLE is a bare byte stream with no magic; it is selected by id/extension, not
 * positively probed out of arbitrary content. */
static sleela_compression_result rle_probe(sleela_compressor_instance *inst,
                                           const uint8_t *data, size_t len) {
    (void)inst; (void)data; (void)len;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}

/* Run a two-phase (size, then fill) codec entry point into an owned buffer. */
static sleela_compression_result run_codec(
        sleela_compression_result (*fn)(const uint8_t *, size_t, uint8_t *, size_t *),
        sleela_compressor_instance *inst,
        const uint8_t *input, size_t input_len, sleela_byte_buffer *out) {
    if (!inst || !out || (!input && input_len)) return SLEELA_COMPRESSION_ERR_INVALID;

    size_t need = 0;
    sleela_compression_result r = fn(input, input_len, NULL, &need);
    if (r != SLEELA_COMPRESSION_OK) return r;

    uint8_t *buf = (uint8_t *)malloc(need ? need : 1);
    if (!buf) return SLEELA_COMPRESSION_ERR_IO;

    size_t cap = need;
    r = fn(input, input_len, buf, &cap);
    if (r != SLEELA_COMPRESSION_OK) { free(buf); return r; }

    free(inst->owned);
    inst->owned = buf;
    out->data = buf;
    out->len = cap;
    return SLEELA_COMPRESSION_OK;
}

static sleela_compression_result rle_compress(sleela_compressor_instance *inst,
                                              const uint8_t *input, size_t input_len,
                                              sleela_byte_buffer *out) {
    return run_codec(sleela_rle_encode, inst, input, input_len, out);
}

static sleela_compression_result rle_decompress(sleela_compressor_instance *inst,
                                                const uint8_t *input, size_t input_len,
                                                sleela_byte_buffer *out) {
    return run_codec(sleela_rle_decode, inst, input, input_len, out);
}

static const sleela_compressor_plugin k_plugin = {
    rle_describe, rle_load, rle_unload, rle_probe, rle_compress, rle_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_rle(void) { return &k_plugin; }
