/*
 * LZ4 compressor -- Backend handler.
 *
 * LZ4 frame format: the magic 0x04 0x22 0x4D 0x18 (little-endian 0x184D2204). Compress/decompress require an approved LZ4 backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in lz4_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_lz4(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_LZ4, ...).
 */

#include "sleela_compressor_plugin.h"
#include "lz4_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_LZ4, "LZ4", "application/x-lz4", ".lz4",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *lz4_describe(void) { return &k_meta; }

static sleela_compression_result lz4_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_lz4_probe_bytes(data, len);
}

static sleela_compression_result lz4_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_LZ4);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result lz4_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_LZ4);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    lz4_describe, /*load*/ 0, /*unload*/ 0, lz4_probe, lz4_compress, lz4_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_lz4(void) { return &k_plugin; }
