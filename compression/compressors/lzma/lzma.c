/*
 * LZMA compressor -- Backend handler.
 *
 * LZMA (.lzma / alone format): a properties byte of 0x5D (lc=3,lp=0,pb=2) followed by a little-endian dictionary size. Compress/decompress require an approved LZMA backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in lzma_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_lzma(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_LZMA, ...).
 */

#include "sleela_compressor_plugin.h"
#include "lzma_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_LZMA, "LZMA", "application/x-lzma", ".lzma",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *lzma_describe(void) { return &k_meta; }

static sleela_compression_result lzma_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_lzma_probe_bytes(data, len);
}

static sleela_compression_result lzma_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_LZMA);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result lzma_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_LZMA);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    lzma_describe, /*load*/ 0, /*unload*/ 0, lzma_probe, lzma_compress, lzma_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_lzma(void) { return &k_plugin; }
