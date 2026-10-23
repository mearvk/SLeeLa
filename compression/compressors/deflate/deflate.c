/*
 * DEFLATE (RFC 1951) compressor -- Backend handler.
 *
 * Raw DEFLATE (RFC 1951) has no header, so there is no reliable signature; it is selected by id/extension, not positively probed. Compress/decompress require an approved DEFLATE (RFC 1951) backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in deflate_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_deflate(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_DEFLATE, ...).
 */

#include "sleela_compressor_plugin.h"
#include "deflate_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_DEFLATE, "DEFLATE (RFC 1951)", "application/x-deflate", ".deflate",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *deflate_describe(void) { return &k_meta; }

static sleela_compression_result deflate_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_deflate_probe_bytes(data, len);
}

static sleela_compression_result deflate_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_DEFLATE);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result deflate_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_DEFLATE);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    deflate_describe, /*load*/ 0, /*unload*/ 0, deflate_probe, deflate_compress, deflate_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_deflate(void) { return &k_plugin; }
