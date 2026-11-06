#include "large_language_models.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct sllm_engine {
    sllm_backend backend;
    int loaded;
};

static int valid_backend(const sllm_backend *b) {
    return b && b->abi_version == SLLM_ABI_VERSION && b->name && b->name[0] &&
           b->load && b->unload && b->generate;
}

static int valid_options(const sllm_generation_options *o) {
    return o && o->abi_version == SLLM_ABI_VERSION &&
           o->max_new_tokens > 0 && isfinite(o->temperature) &&
           o->temperature >= 0.0 && isfinite(o->top_p) &&
           o->top_p > 0.0 && o->top_p <= 1.0;
}

const char *sllm_status_string(sllm_status status) {
    switch (status) {
        case SLLM_OK: return "success";
        case SLLM_ERROR_INVALID_ARGUMENT: return "invalid argument";
        case SLLM_ERROR_OUT_OF_MEMORY: return "out of memory";
        case SLLM_ERROR_BACKEND: return "backend failure";
        case SLLM_ERROR_NOT_LOADED: return "model is not loaded";
        case SLLM_ERROR_CANCELLED: return "generation cancelled";
        case SLLM_ERROR_UNSUPPORTED: return "operation unsupported";
        default: return "unknown status";
    }
}

sllm_status sllm_generation_options_init(sllm_generation_options *o) {
    if (!o) return SLLM_ERROR_INVALID_ARGUMENT;
    o->abi_version = SLLM_ABI_VERSION;
    o->max_new_tokens = 256;
    o->temperature = 0.7;
    o->top_p = 0.95;
    o->seed = 0;
    return SLLM_OK;
}

sllm_status sllm_engine_create(const sllm_backend *backend, sllm_engine **out) {
    sllm_engine *engine;
    if (!out || !valid_backend(backend)) return SLLM_ERROR_INVALID_ARGUMENT;
    *out = NULL;
    engine = (sllm_engine *)calloc(1, sizeof(*engine));
    if (!engine) return SLLM_ERROR_OUT_OF_MEMORY;
    engine->backend = *backend;
    *out = engine;
    return SLLM_OK;
}

void sllm_engine_unload(sllm_engine *engine) {
    if (engine && engine->loaded) {
        engine->backend.unload(engine->backend.user_data);
        engine->loaded = 0;
    }
}

void sllm_engine_destroy(sllm_engine *engine) {
    if (!engine) return;
    sllm_engine_unload(engine);
    free(engine);
}

const char *sllm_engine_backend_name(const sllm_engine *engine) {
    return engine ? engine->backend.name : NULL;
}

int sllm_engine_is_loaded(const sllm_engine *engine) {
    return engine ? engine->loaded : 0;
}

sllm_status sllm_engine_load(sllm_engine *engine, const char *model_id) {
    sllm_status status;
    if (!engine || !model_id || !model_id[0]) return SLLM_ERROR_INVALID_ARGUMENT;
    if (engine->loaded) sllm_engine_unload(engine);
    status = engine->backend.load(engine->backend.user_data, model_id);
    if (status == SLLM_OK) engine->loaded = 1;
    return status;
}

typedef struct sllm_callback_state {
    sllm_chunk_callback callback;
    void *user_data;
    int cancelled;
} sllm_callback_state;

static int dispatch_chunk(const char *chunk, size_t length, void *opaque) {
    sllm_callback_state *state = (sllm_callback_state *)opaque;
    if (!chunk && length != 0) return 0;
    if (!state->callback) return 1;
    if (!state->callback(chunk, length, state->user_data)) {
        state->cancelled = 1;
        return 0;
    }
    return 1;
}

sllm_status sllm_engine_generate(sllm_engine *engine, const char *prompt,
    const sllm_generation_options *options, sllm_chunk_callback callback, void *data) {
    sllm_callback_state state;
    sllm_status status;
    if (!engine || !prompt || !valid_options(options))
        return SLLM_ERROR_INVALID_ARGUMENT;
    if (!engine->loaded) return SLLM_ERROR_NOT_LOADED;
    state.callback = callback;
    state.user_data = data;
    state.cancelled = 0;
    status = engine->backend.generate(engine->backend.user_data, prompt,
                                      options, dispatch_chunk, &state);
    if (state.cancelled) return SLLM_ERROR_CANCELLED;
    return status;
}
