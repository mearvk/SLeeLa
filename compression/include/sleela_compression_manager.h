#ifndef SLEELA_COMPRESSION_MANAGER_H
#define SLEELA_COMPRESSION_MANAGER_H

/*
 * SLeeLa Compression Loader / Unloader / Management Controller.
 *
 * This is the single entry point SLeeLa calls for compression. SLeeLa does not
 * link or call an individual algorithm directly; it asks the manager to load an
 * algorithm by id (or resolve one by file extension or content probe), then
 * dispatches compress/decompress through the manager. The manager owns each
 * plugin's lifecycle:
 *
 *   load   -> construct the plugin instance (plugin->load)
 *   use    -> probe / compress / decompress via the loaded instance
 *   unload -> release the instance (plugin->unload)
 *
 * The manager is a controller, not a compressor: it holds no payload data of
 * its own. It keeps a registry of every known plugin and a per-algorithm
 * "loaded" flag, so a backend-less algorithm can be present (recognized /
 * metadata) without being loaded, and an algorithm can be loaded and unloaded
 * at runtime.
 */

#include <stddef.h>
#include <stdint.h>

#include "sleela_compressor_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sleela_compression_manager sleela_compression_manager;

/* ---- lifecycle of the manager itself -------------------------------- */

/* Create a manager with every known algorithm registered but none loaded.
 * Returns NULL on allocation failure. */
sleela_compression_manager *sleela_compression_manager_create(void);

/* Destroy the manager, unloading any still-loaded algorithms first. */
void sleela_compression_manager_destroy(sleela_compression_manager *mgr);

/* ---- registry inspection -------------------------------------------- */

/* Number of algorithms known to the manager (equals SLEELA_COMPRESSION_COUNT). */
size_t sleela_compression_manager_count(const sleela_compression_manager *mgr);

/* Metadata for an algorithm by id (does not require it to be loaded). */
const sleela_compressor_handler *sleela_compression_manager_describe(const sleela_compression_manager *mgr,
                                                                     sleela_compressor_id id);

/* Is the algorithm currently loaded? 1 = loaded, 0 = not, <0 on bad args. */
int sleela_compression_manager_is_loaded(const sleela_compression_manager *mgr, sleela_compressor_id id);

/* ---- loader / unloader ---------------------------------------------- */

/* Load an algorithm by id (idempotent: loading an already-loaded one is OK). */
sleela_compression_result sleela_compression_manager_load(sleela_compression_manager *mgr, sleela_compressor_id id);

/* Unload an algorithm by id (idempotent). */
sleela_compression_result sleela_compression_manager_unload(sleela_compression_manager *mgr, sleela_compressor_id id);

/* Unload every currently loaded algorithm. */
void sleela_compression_manager_unload_all(sleela_compression_manager *mgr);

/* ---- resolution ------------------------------------------------------ */

/* Resolve an algorithm id from a file extension (e.g. ".gz"), or -1 if unknown. */
sleela_compressor_id sleela_compression_manager_resolve_extension(const sleela_compression_manager *mgr,
                                                                  const char *extension);

/* Resolve an algorithm id by probing the first `len` bytes of `data` against
 * every loaded algorithm's probe(), or -1 if none claim it. Only loaded
 * algorithms probe. */
sleela_compressor_id sleela_compression_manager_resolve_probe(sleela_compression_manager *mgr,
                                                             const uint8_t *data, size_t len);

/* ---- dispatch (requires the algorithm to be loaded) ----------------- */

/* Compress input using the loaded algorithm `id`. */
sleela_compression_result sleela_compression_manager_compress(sleela_compression_manager *mgr, sleela_compressor_id id,
                                                             const uint8_t *input, size_t input_len,
                                                             sleela_byte_buffer *out);

/* Decompress input using the loaded algorithm `id`. */
sleela_compression_result sleela_compression_manager_decompress(sleela_compression_manager *mgr, sleela_compressor_id id,
                                                               const uint8_t *input, size_t input_len,
                                                               sleela_byte_buffer *out);

/* Human-readable text for a result code (never NULL). */
const char *sleela_compression_result_text(sleela_compression_result r);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSION_MANAGER_H */
