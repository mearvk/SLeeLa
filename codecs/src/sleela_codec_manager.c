/*
 * SLeeLa Codec Loader / Unloader / Management Controller -- implementation.
 *
 * Builds a fixed table of every known codec plugin (one accessor per codec),
 * owns each plugin's loaded/unloaded lifecycle and instance pointer, and
 * dispatches probe/decode/encode to the loaded plugin. This is the only codec
 * surface SLeeLa calls.
 */

#include "sleela_codec_manager.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    const sleela_codec_plugin *plugin;   /* vtable (static singleton)     */
    sleela_codec_instance     *instance; /* per-load state, NULL if unloaded */
    int                        loaded;
} codec_slot;

struct sleela_codec_manager {
    codec_slot slots[SLEELA_CODEC_COUNT];
};

/* The registry of plugin accessors, indexed by sleela_codec_id order. Keeping
 * this ordered by the enum lets the manager map id -> slot in O(1). */
typedef const sleela_codec_plugin *(*plugin_accessor)(void);

static const plugin_accessor k_accessors[SLEELA_CODEC_COUNT] = {
    sleela_codec_plugin_pcm_wav,
    sleela_codec_plugin_aiff,
    sleela_codec_plugin_flac,
    sleela_codec_plugin_alac,
    sleela_codec_plugin_mp3,
    sleela_codec_plugin_aac,
    sleela_codec_plugin_he_aac,
    sleela_codec_plugin_vorbis,
    sleela_codec_plugin_opus,
    sleela_codec_plugin_speex,
    sleela_codec_plugin_wma,
    sleela_codec_plugin_ac3,
    sleela_codec_plugin_eac3,
    sleela_codec_plugin_amr_nb,
    sleela_codec_plugin_amr_wb,
    sleela_codec_plugin_g711_mulaw,
    sleela_codec_plugin_g711_alaw,
    sleela_codec_plugin_midi,
    sleela_codec_plugin_matroska_audio,
    sleela_codec_plugin_webm_audio
};

static int id_ok(sleela_codec_id id) { return id >= 0 && id < SLEELA_CODEC_COUNT; }

sleela_codec_manager *sleela_codec_manager_create(void) {
    sleela_codec_manager *mgr = (sleela_codec_manager *)calloc(1, sizeof(*mgr));
    if (!mgr) return NULL;
    for (size_t i = 0; i < SLEELA_CODEC_COUNT; ++i) {
        mgr->slots[i].plugin = k_accessors[i] ? k_accessors[i]() : NULL;
        mgr->slots[i].instance = NULL;
        mgr->slots[i].loaded = 0;
    }
    return mgr;
}

void sleela_codec_manager_destroy(sleela_codec_manager *mgr) {
    if (!mgr) return;
    sleela_codec_manager_unload_all(mgr);
    free(mgr);
}

size_t sleela_codec_manager_count(const sleela_codec_manager *mgr) {
    return mgr ? SLEELA_CODEC_COUNT : 0;
}

const sleela_codec_handler *sleela_codec_manager_describe(const sleela_codec_manager *mgr,
                                                          sleela_codec_id id) {
    if (!mgr || !id_ok(id) || !mgr->slots[id].plugin || !mgr->slots[id].plugin->describe)
        return NULL;
    return mgr->slots[id].plugin->describe();
}

int sleela_codec_manager_is_loaded(const sleela_codec_manager *mgr, sleela_codec_id id) {
    if (!mgr || !id_ok(id)) return -1;
    return mgr->slots[id].loaded ? 1 : 0;
}

sleela_codec_result sleela_codec_manager_load(sleela_codec_manager *mgr, sleela_codec_id id) {
    if (!mgr || !id_ok(id)) return SLEELA_CODEC_ERR_INVALID;
    codec_slot *s = &mgr->slots[id];
    if (!s->plugin) return SLEELA_CODEC_ERR_UNSUPPORTED;
    if (s->loaded) return SLEELA_CODEC_OK;         /* idempotent */
    if (s->plugin->load) {
        sleela_codec_result r = s->plugin->load(&s->instance);
        if (r != SLEELA_CODEC_OK) { s->instance = NULL; return r; }
    } else {
        s->instance = NULL;                        /* stateless plugin */
    }
    s->loaded = 1;
    return SLEELA_CODEC_OK;
}

sleela_codec_result sleela_codec_manager_unload(sleela_codec_manager *mgr, sleela_codec_id id) {
    if (!mgr || !id_ok(id)) return SLEELA_CODEC_ERR_INVALID;
    codec_slot *s = &mgr->slots[id];
    if (!s->loaded) return SLEELA_CODEC_OK;        /* idempotent */
    if (s->plugin && s->plugin->unload) s->plugin->unload(s->instance);
    s->instance = NULL;
    s->loaded = 0;
    return SLEELA_CODEC_OK;
}

void sleela_codec_manager_unload_all(sleela_codec_manager *mgr) {
    if (!mgr) return;
    for (size_t i = 0; i < SLEELA_CODEC_COUNT; ++i)
        (void)sleela_codec_manager_unload(mgr, (sleela_codec_id)i);
}

sleela_codec_id sleela_codec_manager_resolve_extension(const sleela_codec_manager *mgr,
                                                       const char *extension) {
    (void)mgr;
    const sleela_codec_handler *h = sleela_codec_by_extension(extension);
    return h ? h->id : (sleela_codec_id)-1;
}

sleela_codec_id sleela_codec_manager_resolve_probe(sleela_codec_manager *mgr,
                                                   const uint8_t *data, size_t len) {
    if (!mgr || !data || len == 0) return (sleela_codec_id)-1;
    for (size_t i = 0; i < SLEELA_CODEC_COUNT; ++i) {
        codec_slot *s = &mgr->slots[i];
        if (!s->loaded || !s->plugin || !s->plugin->probe) continue;
        if (s->plugin->probe(s->instance, data, len) == SLEELA_CODEC_OK)
            return (sleela_codec_id)i;
    }
    return (sleela_codec_id)-1;
}

sleela_codec_result sleela_codec_manager_decode(sleela_codec_manager *mgr, sleela_codec_id id,
                                                const uint8_t *input, size_t input_len,
                                                sleela_pcm_buffer *out) {
    if (!mgr || !id_ok(id) || !out) return SLEELA_CODEC_ERR_INVALID;
    codec_slot *s = &mgr->slots[id];
    if (!s->loaded) return SLEELA_CODEC_ERR_STATE;
    if (!s->plugin || !s->plugin->decode) return SLEELA_CODEC_ERR_UNSUPPORTED;
    return s->plugin->decode(s->instance, input, input_len, out);
}

sleela_codec_result sleela_codec_manager_encode(sleela_codec_manager *mgr, sleela_codec_id id,
                                                const sleela_pcm_buffer *pcm,
                                                uint8_t *output, size_t *inout_len) {
    if (!mgr || !id_ok(id) || !pcm || !inout_len) return SLEELA_CODEC_ERR_INVALID;
    codec_slot *s = &mgr->slots[id];
    if (!s->loaded) return SLEELA_CODEC_ERR_STATE;
    if (!s->plugin || !s->plugin->encode) return SLEELA_CODEC_ERR_UNSUPPORTED;
    return s->plugin->encode(s->instance, pcm, output, inout_len);
}

const char *sleela_codec_result_text(sleela_codec_result r) {
    switch (r) {
        case SLEELA_CODEC_OK:              return "ok";
        case SLEELA_CODEC_ERR_UNSUPPORTED: return "unsupported (backend not present)";
        case SLEELA_CODEC_ERR_INVALID:     return "invalid arguments";
        case SLEELA_CODEC_ERR_FORMAT:      return "input is not this codec's format";
        case SLEELA_CODEC_ERR_IO:          return "I/O or allocation failure";
        case SLEELA_CODEC_ERR_STATE:       return "codec not loaded";
    }
    return "unknown";
}
