/*
 * Runtime compression backend registry.
 *
 * A process-global table mapping an algorithm id to the backend a host has
 * wired in (e.g. an adapter over zlib / libzstd / liblz4). Backend plugins
 * consult sleela_compression_backend_get() from their compress/decompress and
 * dispatch to the registered adapter, or return
 * SLEELA_COMPRESSION_ERR_UNSUPPORTED when none is set.
 */

#include "sleela_compressor_plugin.h"

#include <stddef.h>

static const sleela_compression_backend *g_backends[SLEELA_COMPRESSION_COUNT];

sleela_compression_result sleela_compression_backend_register(sleela_compressor_id id,
                                                              const sleela_compression_backend *backend) {
    if (id < 0 || id >= SLEELA_COMPRESSION_COUNT) return SLEELA_COMPRESSION_ERR_INVALID;
    g_backends[id] = backend;   /* NULL clears */
    return SLEELA_COMPRESSION_OK;
}

const sleela_compression_backend *sleela_compression_backend_get(sleela_compressor_id id) {
    if (id < 0 || id >= SLEELA_COMPRESSION_COUNT) return NULL;
    return g_backends[id];
}
