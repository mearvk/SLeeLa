/*
 * Brotli compressor -- Backend handler.
 *
 * Brotli has no fixed magic signature (the stream begins with a window-size descriptor), so it is selected by id/extension, not positively probed. Compress/decompress require an approved Brotli backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in brotli_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_brotli(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_BROTLI, ...).
 */

#include "sleela_compressor_plugin.h"
#include "brotli_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_BROTLI, "Brotli", "application/x-brotli", ".br",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *brotli_describe(void) { return &k_meta; }

static sleela_compression_result brotli_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_brotli_probe_bytes(data, len);
}

static sleela_compression_result brotli_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_BROTLI);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result brotli_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_BROTLI);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    brotli_describe, /*load*/ 0, /*unload*/ 0, brotli_probe, brotli_compress, brotli_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_brotli(void) { return &k_plugin; }
