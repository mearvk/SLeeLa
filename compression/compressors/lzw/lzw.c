/*
 * Lempel-Ziv-Welch compressor -- native handler.
 *
 * Variable-width LZW (9..12 bits) with an explicit CLEAR code and a
 * self-describing header (magic "SLZW" + original length). The bit-level codec
 * is in the C++ helper lzw_detail.cpp; this C file is the plugin vtable and
 * owns the output buffer across a load/use/unload cycle.
 */

#include "sleela_compressor_plugin.h"
#include "lzw_detail.h"

#include <stdlib.h>

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_LZW, "Lempel-Ziv-Welch", "application/x-lzw", ".lzw,.Z",
    SLEELA_COMPRESSION_NATIVE, 1, 1
};

struct sleela_compressor_instance { uint8_t *owned; };

static const sleela_compressor_handler *lzw_describe(void) { return &k_meta; }

static sleela_compression_result lzw_load(sleela_compressor_instance **out) {
    sleela_compressor_instance *inst = (sleela_compressor_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_COMPRESSION_ERR_IO;
    *out = inst;
    return SLEELA_COMPRESSION_OK;
}

static void lzw_unload(sleela_compressor_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

/* The "SLZW" magic identifies a SLeeLa LZW stream. */
static sleela_compression_result lzw_probe(sleela_compressor_instance *inst,
                                           const uint8_t *data, size_t len) {
    (void)inst;
    if (data && len >= 4 && data[0] == 'S' && data[1] == 'L' && data[2] == 'Z' && data[3] == 'W')
        return SLEELA_COMPRESSION_OK;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}

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

static sleela_compression_result lzw_compress(sleela_compressor_instance *inst,
                                              const uint8_t *input, size_t input_len,
                                              sleela_byte_buffer *out) {
    return run_codec(sleela_lzw_encode, inst, input, input_len, out);
}

static sleela_compression_result lzw_decompress(sleela_compressor_instance *inst,
                                                const uint8_t *input, size_t input_len,
                                                sleela_byte_buffer *out) {
    return run_codec(sleela_lzw_decode, inst, input, input_len, out);
}

static const sleela_compressor_plugin k_plugin = {
    lzw_describe, lzw_load, lzw_unload, lzw_probe, lzw_compress, lzw_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_lzw(void) { return &k_plugin; }
