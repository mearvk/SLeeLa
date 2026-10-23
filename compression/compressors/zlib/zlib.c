/*
 * zlib (RFC 1950) compressor -- Backend handler.
 *
 * zlib (RFC 1950): the two-byte header has CM=8 (deflate) in the low nibble of byte 0 and (CMF*256 + FLG) is a multiple of 31. Compress/decompress require an approved zlib (RFC 1950) backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in zlib_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_zlib(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_ZLIB, ...).
 */

#include "sleela_compressor_plugin.h"
#include "zlib_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_ZLIB, "zlib (RFC 1950)", "application/zlib", ".zz",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *zlib_describe(void) { return &k_meta; }

static sleela_compression_result zlib_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_zlib_probe_bytes(data, len);
}

static sleela_compression_result zlib_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_ZLIB);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result zlib_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_ZLIB);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    zlib_describe, /*load*/ 0, /*unload*/ 0, zlib_probe, zlib_compress, zlib_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_zlib(void) { return &k_plugin; }
