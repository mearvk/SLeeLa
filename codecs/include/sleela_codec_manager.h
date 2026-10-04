#ifndef SLEELA_CODEC_MANAGER_H
#define SLEELA_CODEC_MANAGER_H

/*
 * SLeeLa Codec Loader / Unloader / Management Controller.
 *
 * This is the single entry point SLeeLa calls for codecs. SLeeLa does not link
 * or call an individual codec directly; it asks the manager to load a codec by
 * id (or resolve one by file extension or content probe), then dispatches
 * decode/encode through the manager. The manager owns each plugin's lifecycle:
 *
 *   load   -> construct the plugin instance (plugin->load)
 *   use    -> probe / decode / encode via the loaded instance
 *   unload -> release the instance (plugin->unload)
 *
 * The manager is a controller, not a codec: it holds no audio data of its own.
 * It keeps a registry of every known codec plugin and a per-codec "loaded"
 * flag, so a backend-less codec can be present (recognized/metadata) without
 * being loaded, and a codec can be loaded and unloaded at runtime.
 */

#include <stddef.h>
#include <stdint.h>

#include "sleela_codec_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sleela_codec_manager sleela_codec_manager;

/* ---- lifecycle of the manager itself -------------------------------- */

/* Create a manager with every known codec registered but none loaded.
 * Returns NULL on allocation failure. */
sleela_codec_manager *sleela_codec_manager_create(void);

/* Destroy the manager, unloading any still-loaded codecs first. */
void sleela_codec_manager_destroy(sleela_codec_manager *mgr);

/* ---- registry inspection -------------------------------------------- */

/* Number of codecs known to the manager (equals SLEELA_CODEC_COUNT). */
size_t sleela_codec_manager_count(const sleela_codec_manager *mgr);

/* Metadata for a codec by id (does not require it to be loaded). */
const sleela_codec_handler *sleela_codec_manager_describe(const sleela_codec_manager *mgr,
                                                          sleela_codec_id id);

/* Is the codec currently loaded? 1 = loaded, 0 = not, <0 on bad args. */
int sleela_codec_manager_is_loaded(const sleela_codec_manager *mgr, sleela_codec_id id);

/* ---- loader / unloader ---------------------------------------------- */

/* Load a codec by id (idempotent: loading an already-loaded codec is OK). */
sleela_codec_result sleela_codec_manager_load(sleela_codec_manager *mgr, sleela_codec_id id);

/* Unload a codec by id (idempotent). */
sleela_codec_result sleela_codec_manager_unload(sleela_codec_manager *mgr, sleela_codec_id id);

/* Unload every currently loaded codec. */
void sleela_codec_manager_unload_all(sleela_codec_manager *mgr);

/* ---- resolution ------------------------------------------------------ */

/* Resolve a codec id from a file extension (e.g. ".flac"), or -1 if unknown. */
sleela_codec_id sleela_codec_manager_resolve_extension(const sleela_codec_manager *mgr,
                                                       const char *extension);

/* Resolve a codec id by probing the first `len` bytes of `data` against every
 * loaded codec's probe(), or -1 if none claim it. Only loaded codecs probe. */
sleela_codec_id sleela_codec_manager_resolve_probe(sleela_codec_manager *mgr,
                                                   const uint8_t *data, size_t len);

/* ---- dispatch (requires the codec to be loaded) --------------------- */

/* Decode input to PCM using the loaded codec `id`. */
sleela_codec_result sleela_codec_manager_decode(sleela_codec_manager *mgr, sleela_codec_id id,
                                                const uint8_t *input, size_t input_len,
                                                sleela_pcm_buffer *out);

/* Encode PCM to the loaded codec `id`. */
sleela_codec_result sleela_codec_manager_encode(sleela_codec_manager *mgr, sleela_codec_id id,
                                                const sleela_pcm_buffer *pcm,
                                                uint8_t *output, size_t *inout_len);

/* Human-readable text for a result code (never NULL). */
const char *sleela_codec_result_text(sleela_codec_result r);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_MANAGER_H */
