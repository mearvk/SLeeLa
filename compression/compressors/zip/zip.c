/*
 * ZIP archive compressor -- Container handler.
 *
 * ZIP container: the 'PK' signature 0x50 0x4B followed by a record marker (0x03 0x04 local file, 0x05 0x06 end-of-central-directory, or 0x07 0x08 spanned). Compress/decompress require an approved ZIP archive backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_CONTAINER); the C++ signature probe is in zip_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_zip(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_ZIP, ...).
 */

#include "sleela_compressor_plugin.h"
#include "zip_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_ZIP, "ZIP archive", "application/zip", ".zip",
    SLEELA_COMPRESSION_CONTAINER, 1, 1
};

static const sleela_compressor_handler *zip_describe(void) { return &k_meta; }

static sleela_compression_result zip_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_zip_probe_bytes(data, len);
}

static sleela_compression_result zip_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_ZIP);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result zip_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_ZIP);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    zip_describe, /*load*/ 0, /*unload*/ 0, zip_probe, zip_compress, zip_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_zip(void) { return &k_plugin; }
