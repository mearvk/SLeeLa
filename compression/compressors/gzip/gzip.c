/*
 * gzip (RFC 1952) compressor -- Backend handler.
 *
 * gzip (RFC 1952): magic 0x1F 0x8B followed by compression method 0x08 (deflate). Compress/decompress require an approved gzip (RFC 1952) backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in gzip_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_gzip(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_GZIP, ...).
 */

#include "sleela_compressor_plugin.h"
#include "gzip_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_GZIP, "gzip (RFC 1952)", "application/gzip", ".gz",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *gzip_describe(void) { return &k_meta; }

static sleela_compression_result gzip_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_gzip_probe_bytes(data, len);
}

static sleela_compression_result gzip_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_GZIP);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result gzip_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_GZIP);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    gzip_describe, /*load*/ 0, /*unload*/ 0, gzip_probe, gzip_compress, gzip_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_gzip(void) { return &k_plugin; }
