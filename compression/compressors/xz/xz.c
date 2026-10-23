/*
 * xz (LZMA2) compressor -- Backend handler.
 *
 * xz: the six-byte magic 0xFD '7zXZ' 0x00. Compress/decompress require an approved xz (LZMA2) backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in xz_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_xz(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_XZ, ...).
 */

#include "sleela_compressor_plugin.h"
#include "xz_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_XZ, "xz (LZMA2)", "application/x-xz", ".xz",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *xz_describe(void) { return &k_meta; }

static sleela_compression_result xz_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_xz_probe_bytes(data, len);
}

static sleela_compression_result xz_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_XZ);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result xz_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_XZ);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    xz_describe, /*load*/ 0, /*unload*/ 0, xz_probe, xz_compress, xz_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_xz(void) { return &k_plugin; }
