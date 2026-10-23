/*
 * Snappy compressor -- Backend handler.
 *
 * Snappy framing format: a stream-identifier chunk 0xFF 0x06 0x00 0x00 followed by 'sN' ('sNaPpY'). Compress/decompress require an approved Snappy backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_COMPRESSION_BACKEND); the C++ signature probe is in snappy_detail.cpp. The
 * manager loads it via sleela_compressor_plugin_snappy(). With no backend
 * registered, compress/decompress return SLEELA_COMPRESSION_ERR_UNSUPPORTED --
 * never a false success. Register one via
 * sleela_compression_backend_register(SLEELA_COMPRESSION_SNAPPY, ...).
 */

#include "sleela_compressor_plugin.h"
#include "snappy_detail.h"

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_SNAPPY, "Snappy", "application/x-snappy-framed", ".sz",
    SLEELA_COMPRESSION_BACKEND, 1, 1
};

static const sleela_compressor_handler *snappy_describe(void) { return &k_meta; }

static sleela_compression_result snappy_probe(sleela_compressor_instance *inst,
                                              const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_snappy_probe_bytes(data, len);
}

static sleela_compression_result snappy_compress(sleela_compressor_instance *inst,
                                                 const uint8_t *input, size_t input_len,
                                                 sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_SNAPPY);
    if (b && b->compress) return b->compress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static sleela_compression_result snappy_decompress(sleela_compressor_instance *inst,
                                                   const uint8_t *input, size_t input_len,
                                                   sleela_byte_buffer *out) {
    (void)inst;
    const sleela_compression_backend *b = sleela_compression_backend_get(SLEELA_COMPRESSION_SNAPPY);
    if (b && b->decompress) return b->decompress(input, input_len, out);
    return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
}

static const sleela_compressor_plugin k_plugin = {
    snappy_describe, /*load*/ 0, /*unload*/ 0, snappy_probe, snappy_compress, snappy_decompress
};

const sleela_compressor_plugin *sleela_compressor_plugin_snappy(void) { return &k_plugin; }
