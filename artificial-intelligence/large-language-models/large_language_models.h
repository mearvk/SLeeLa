#ifndef SLEELA_AI_LARGE_LANGUAGE_MODELS_H
#define SLEELA_AI_LARGE_LANGUAGE_MODELS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
#  if defined(SLEELA_AI_BUILD)
#    define SLLM_API __declspec(dllexport)
#  else
#    define SLLM_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
#  define SLLM_API __attribute__((visibility("default")))
#else
#  define SLLM_API
#endif

#define SLLM_ABI_VERSION 1u

typedef struct sllm_engine sllm_engine;

typedef enum sllm_status {
    SLLM_OK = 0,
    SLLM_ERROR_INVALID_ARGUMENT = 1,
    SLLM_ERROR_OUT_OF_MEMORY = 2,
    SLLM_ERROR_BACKEND = 3,
    SLLM_ERROR_NOT_LOADED = 4,
    SLLM_ERROR_CANCELLED = 5,
    SLLM_ERROR_UNSUPPORTED = 6
} sllm_status;

typedef struct sllm_generation_options {
    uint32_t abi_version;
    uint32_t max_new_tokens;
    double temperature;       /* >= 0; zero requests deterministic decoding. */
    double top_p;             /* (0, 1]. */
    uint64_t seed;            /* Backend-defined deterministic seed. */
} sllm_generation_options;

/* A backend streams UTF-8 text chunks through this callback. */
typedef int (*sllm_chunk_callback)(const char *utf8, size_t byte_count, void *user_data);

/*
 * Provider adapter contract. The adapter and user_data remain owned by the
 * caller and must outlive the engine. Backend functions must be safe for the
 * concurrency guarantees they advertise; this API serializes no calls.
 */
typedef struct sllm_backend {
    uint32_t abi_version;
    const char *name;
    void *user_data;
    sllm_status (*load)(void *user_data, const char *model_id);
    void (*unload)(void *user_data);
    sllm_status (*generate)(
        void *user_data, const char *prompt,
        const sllm_generation_options *options,
        sllm_chunk_callback on_chunk, void *callback_data);
} sllm_backend;

SLLM_API const char *sllm_status_string(sllm_status status);
SLLM_API sllm_status sllm_generation_options_init(sllm_generation_options *options);
SLLM_API sllm_status sllm_engine_create(const sllm_backend *backend, sllm_engine **out_engine);
SLLM_API void sllm_engine_destroy(sllm_engine *engine);
SLLM_API const char *sllm_engine_backend_name(const sllm_engine *engine);
SLLM_API int sllm_engine_is_loaded(const sllm_engine *engine);
SLLM_API sllm_status sllm_engine_load(sllm_engine *engine, const char *model_id);
SLLM_API void sllm_engine_unload(sllm_engine *engine);

/* Returns SLLM_ERROR_CANCELLED when the chunk callback returns zero. */
SLLM_API sllm_status sllm_engine_generate(
    sllm_engine *engine, const char *prompt,
    const sllm_generation_options *options,
    sllm_chunk_callback on_chunk, void *callback_data);

#ifdef __cplusplus
}
#endif

#endif
