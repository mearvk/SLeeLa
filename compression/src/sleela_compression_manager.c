/*
 * SLeeLa Compression Loader / Unloader / Management Controller -- implementation.
 *
 * Builds a fixed table of every known compressor plugin (one accessor per
 * algorithm), owns each plugin's loaded/unloaded lifecycle and instance
 * pointer, and dispatches probe/compress/decompress to the loaded plugin. This
 * is the only compression surface SLeeLa calls.
 */

#include "sleela_compression_manager.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    const sleela_compressor_plugin *plugin;    /* vtable (static singleton)        */
    sleela_compressor_instance     *instance;  /* per-load state, NULL if unloaded */
    int                             loaded;
} compressor_slot;

struct sleela_compression_manager {
    compressor_slot slots[SLEELA_COMPRESSION_COUNT];
};

/* The registry of plugin accessors, indexed by sleela_compressor_id order.
 * Keeping this ordered by the enum lets the manager map id -> slot in O(1). */
typedef const sleela_compressor_plugin *(*plugin_accessor)(void);

static const plugin_accessor k_accessors[SLEELA_COMPRESSION_COUNT] = {
    sleela_compressor_plugin_store,
    sleela_compressor_plugin_rle,
    sleela_compressor_plugin_huffman,
    sleela_compressor_plugin_lzw,
    sleela_compressor_plugin_deflate,
    sleela_compressor_plugin_zlib,
    sleela_compressor_plugin_gzip,
    sleela_compressor_plugin_bzip2,
    sleela_compressor_plugin_lzma,
    sleela_compressor_plugin_xz,
    sleela_compressor_plugin_zstd,
    sleela_compressor_plugin_lz4,
    sleela_compressor_plugin_brotli,
    sleela_compressor_plugin_snappy,
    sleela_compressor_plugin_zip
};

static int id_ok(sleela_compressor_id id) { return id >= 0 && id < SLEELA_COMPRESSION_COUNT; }

sleela_compression_manager *sleela_compression_manager_create(void) {
    sleela_compression_manager *mgr = (sleela_compression_manager *)calloc(1, sizeof(*mgr));
    if (!mgr) return NULL;
    for (size_t i = 0; i < SLEELA_COMPRESSION_COUNT; ++i) {
        mgr->slots[i].plugin = k_accessors[i] ? k_accessors[i]() : NULL;
        mgr->slots[i].instance = NULL;
        mgr->slots[i].loaded = 0;
    }
    return mgr;
}

void sleela_compression_manager_destroy(sleela_compression_manager *mgr) {
    if (!mgr) return;
    sleela_compression_manager_unload_all(mgr);
    free(mgr);
}

size_t sleela_compression_manager_count(const sleela_compression_manager *mgr) {
    return mgr ? SLEELA_COMPRESSION_COUNT : 0;
}

const sleela_compressor_handler *sleela_compression_manager_describe(const sleela_compression_manager *mgr,
                                                                     sleela_compressor_id id) {
    if (!mgr || !id_ok(id) || !mgr->slots[id].plugin || !mgr->slots[id].plugin->describe)
        return NULL;
    return mgr->slots[id].plugin->describe();
}

int sleela_compression_manager_is_loaded(const sleela_compression_manager *mgr, sleela_compressor_id id) {
    if (!mgr || !id_ok(id)) return -1;
    return mgr->slots[id].loaded ? 1 : 0;
}

sleela_compression_result sleela_compression_manager_load(sleela_compression_manager *mgr, sleela_compressor_id id) {
    if (!mgr || !id_ok(id)) return SLEELA_COMPRESSION_ERR_INVALID;
    compressor_slot *s = &mgr->slots[id];
    if (!s->plugin) return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
    if (s->loaded) return SLEELA_COMPRESSION_OK;       /* idempotent */
    if (s->plugin->load) {
        sleela_compression_result r = s->plugin->load(&s->instance);
        if (r != SLEELA_COMPRESSION_OK) { s->instance = NULL; return r; }
    } else {
        s->instance = NULL;                            /* stateless plugin */
    }
    s->loaded = 1;
    return SLEELA_COMPRESSION_OK;
}

sleela_compression_result sleela_compression_manager_unload(sleela_compression_manager *mgr, sleela_compressor_id id) {
    if (!mgr || !id_ok(id)) return SLEELA_COMPRESSION_ERR_INVALID;
    compressor_slot *s = &mgr->slots[id];
    if (!s->loaded) return SLEELA_COMPRESSION_OK;      /* idempotent */
    if (s->plugin && s->plugin->unload) s->plugin->unload(s->instance);
    s->instance = NULL;
    s->loaded = 0;
    return SLEELA_COMPRESSION_OK;
}

void sleela_compression_manager_unload_all(sleela_compression_manager *mgr) {
    if (!mgr) return;
    for (size_t i = 0; i < SLEELA_COMPRESSION_COUNT; ++i)
        (void)sleela_compression_manager_unload(mgr, (sleela_compressor_id)i);
}

sleela_compressor_id sleela_compression_manager_resolve_extension(const sleela_compression_manager *mgr,
                                                                  const char *extension) {
    (void)mgr;
    const sleela_compressor_handler *h = sleela_compression_by_extension(extension);
    return h ? h->id : (sleela_compressor_id)-1;
}

sleela_compressor_id sleela_compression_manager_resolve_probe(sleela_compression_manager *mgr,
                                                             const uint8_t *data, size_t len) {
    if (!mgr || !data || len == 0) return (sleela_compressor_id)-1;
    for (size_t i = 0; i < SLEELA_COMPRESSION_COUNT; ++i) {
        compressor_slot *s = &mgr->slots[i];
        if (!s->loaded || !s->plugin || !s->plugin->probe) continue;
        if (s->plugin->probe(s->instance, data, len) == SLEELA_COMPRESSION_OK)
            return (sleela_compressor_id)i;
    }
    return (sleela_compressor_id)-1;
}

sleela_compression_result sleela_compression_manager_compress(sleela_compression_manager *mgr, sleela_compressor_id id,
                                                             const uint8_t *input, size_t input_len,
                                                             sleela_byte_buffer *out) {
    if (!mgr || !id_ok(id) || !out) return SLEELA_COMPRESSION_ERR_INVALID;
    compressor_slot *s = &mgr->slots[id];
    if (!s->loaded) return SLEELA_COMPRESSION_ERR_STATE;
    if (!s->plugin || !s->plugin->compress) return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
    return s->plugin->compress(s->instance, input, input_len, out);
}

sleela_compression_result sleela_compression_manager_decompress(sleela_compression_manager *mgr, sleela_compressor_id id,
                                                               const uint8_t *input, size_t input_len,
                                                               sleela_byte_buffer *out) {
    if (!mgr || !id_ok(id) || !out) return SLEELA_COMPRESSION_ERR_INVALID;
    compressor_slot *s = &mgr->slots[id];
    if (!s->loaded) return SLEELA_COMPRESSION_ERR_STATE;
    if (!s->plugin || !s->plugin->decompress) return SLEELA_COMPRESSION_ERR_UNSUPPORTED;
    return s->plugin->decompress(s->instance, input, input_len, out);
}

const char *sleela_compression_result_text(sleela_compression_result r) {
    switch (r) {
        case SLEELA_COMPRESSION_OK:              return "ok";
        case SLEELA_COMPRESSION_ERR_UNSUPPORTED: return "unsupported (backend not present)";
        case SLEELA_COMPRESSION_ERR_INVALID:     return "invalid arguments";
        case SLEELA_COMPRESSION_ERR_FORMAT:      return "input is not this algorithm's format";
        case SLEELA_COMPRESSION_ERR_IO:          return "I/O or allocation failure";
        case SLEELA_COMPRESSION_ERR_STATE:       return "algorithm not loaded";
        case SLEELA_COMPRESSION_ERR_BUFFER:      return "output buffer too small";
    }
    return "unknown";
}
