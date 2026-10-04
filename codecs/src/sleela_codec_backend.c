/*
 * Runtime codec backend registry.
 *
 * A process-global table mapping a codec id to the backend a host has wired in
 * (e.g. an adapter over libFLAC / libopus). Backend plugins consult
 * sleela_codec_backend_get() from their decode/encode and dispatch to the
 * registered adapter, or return SLEELA_CODEC_ERR_UNSUPPORTED when none is set.
 */

#include "sleela_codec_plugin.h"

#include <stddef.h>

static const sleela_codec_backend *g_backends[SLEELA_CODEC_COUNT];

sleela_codec_result sleela_codec_backend_register(sleela_codec_id id,
                                                  const sleela_codec_backend *backend) {
    if (id < 0 || id >= SLEELA_CODEC_COUNT) return SLEELA_CODEC_ERR_INVALID;
    g_backends[id] = backend;   /* NULL clears */
    return SLEELA_CODEC_OK;
}

const sleela_codec_backend *sleela_codec_backend_get(sleela_codec_id id) {
    if (id < 0 || id >= SLEELA_CODEC_COUNT) return NULL;
    return g_backends[id];
}
