/*
 * Store (raw) compressor -- native handler.
 *
 * The identity transform: "compress" copies the input verbatim and
 * "decompress" copies it back. It is the raw/store boundary every other
 * algorithm is measured against, and a guaranteed-available fallback. There is
 * no header, so probe() never positively claims arbitrary bytes.
 */

#include "sleela_compressor_plugin.h"

#include <stdlib.h>
#include <string.h>

static const sleela_compressor_handler k_meta = {
    SLEELA_COMPRESSION_STORE, "Store (raw)", "application/octet-stream", ".bin",
    SLEELA_COMPRESSION_NATIVE, 1, 1
};

struct sleela_compressor_instance { uint8_t *owned; };

static const sleela_compressor_handler *store_describe(void) { return &k_meta; }

static sleela_compression_result store_load(sleela_compressor_instance **out) {
    sleela_compressor_instance *inst = (sleela_compressor_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_COMPRESSION_ERR_IO;
    *out = inst;
    return SLEELA_COMPRESSION_OK;
}

static void store_unload(sleela_compressor_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

/* Store has no signature; it is the explicit fallback, not a probe target. */
static sleela_compression_result store_probe(sleela_compressor_instance *inst,
                                             const uint8_t *data, size_t len) {
    (void)inst; (void)data; (void)len;
    return SLEELA_COMPRESSION_ERR_FORMAT;
}

static sleela_compression_result store_copy(sleela_compressor_instance *inst,
                                            const uint8_t *input, size_t input_len,
                                            sleela_byte_buffer *out) {
    if (!inst || !out || (!input && input_len)) return SLEELA_COMPRESSION_ERR_INVALID;
    uint8_t *buf = (uint8_t *)malloc(input_len ? input_len : 1);
    if (!buf) return SLEELA_COMPRESSION_ERR_IO;
    if (input_len) memcpy(buf, input, input_len);
    free(inst->owned);
    inst->owned = buf;
    out->data = buf;
    out->len = input_len;
    return SLEELA_COMPRESSION_OK;
}

static const sleela_compressor_plugin k_plugin = {
    store_describe, store_load, store_unload, store_probe, store_copy, store_copy
};

const sleela_compressor_plugin *sleela_compressor_plugin_store(void) { return &k_plugin; }
