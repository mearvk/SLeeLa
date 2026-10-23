/*
 * bzip2 compressor -- Backend handler.
 *
 * bzip2: ASCII magic 'BZh' (0x42 0x5A 0x68). Compress/decompress require an approved bzip2 backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in bzip2_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_bzip2(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_BZIP2, ...).
 */

#include "sleela_compressor_plugin.h"
#include "bzip2_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_BZIP2, "bzip2", "application/x-bzip2", ".bz2",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *bzip2_describe(void) { return &k_meta; }

static sleela_compression_result bzip2_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_bzip2_probe_bytes(data, len);
}

static sleela_compression_result bzip2_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_BZIP2);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result bzip2_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_BZIP2);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    bzip2_describe, /*load*/ 0, /*unload*/ 0, bzip2_probe, bzip2_compress, bzip2_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_bzip2(void) { return &k_plugin; }
