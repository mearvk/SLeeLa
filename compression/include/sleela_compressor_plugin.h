#ifndef SLEELA_COMPRESSOR_PLUGIN_H
#define SLEELA_COMPRESSOR_PLUGIN_H

/*
 * SLeeLa compressor plugin interface.
 *
 * Every individual algorithm under compression/compressors/<name>/ implements
 * this one small vtable. The Compression Manager
 * (sleela_compression_manager.h) loads these plugins, tracks their lifecycle,
 * and dispatches probe/compress/decompress calls to them. SLeeLa never links
 * an individual compressor directly -- it calls the manager, and the manager
 * calls the plugin. This keeps every algorithm interchangeable and lets one be
 * loaded or unloaded at runtime without touching the caller.
 *
 * A compressor here is metadata + an explicit capability contract. A plugin
 * whose backend is not present reports its state (e.g.
 * SLEELA_COMPRESSION_BACKEND) and fails cleanly from compress/decompress rather
 * than faking success.
 */

#include <stddef.h>
#include <stdint.h>

#include "sleela_compression.h"   /* sleela_compressor_id, sleela_compressor_state */

#ifdef __cplusplus
extern "C" {
#endif

/* Result codes shared by every plugin entry point and by the manager. */
typedef enum sleela_compression_result {
    SLEELA_COMPRESSION_OK = 0,              /* success                                        */
    SLEELA_COMPRESSION_ERR_UNSUPPORTED = -1,/* capability not available (e.g. backend absent) */
    SLEELA_COMPRESSION_ERR_INVALID = -2,    /* bad arguments                                  */
    SLEELA_COMPRESSION_ERR_FORMAT = -3,     /* input is not this algorithm's format           */
    SLEELA_COMPRESSION_ERR_IO = -4,         /* read/write/allocation failure                  */
    SLEELA_COMPRESSION_ERR_STATE = -5,      /* plugin not loaded / wrong lifecycle state      */
    SLEELA_COMPRESSION_ERR_BUFFER = -6      /* output buffer too small (see required bytes)   */
} sleela_compression_result;

/*
 * A byte buffer the plugins operate over. Compression and decompression are
 * byte-in / byte-out; `data` points at `len` bytes. For a decompress result the
 * plugin owns `data` until unload() or a plugin-provided free, exactly like the
 * codec PCM boundary owns decoded samples.
 */
typedef struct sleela_byte_buffer {
    uint8_t *data;   /* bytes (ownership documented per entry point)  */
    size_t   len;    /* number of valid bytes                          */
} sleela_byte_buffer;

/* Opaque per-instance state a plugin may allocate in load() and free in unload(). */
typedef struct sleela_compressor_instance sleela_compressor_instance;

/*
 * The compressor plugin vtable. All function pointers are optional except
 * describe(); a NULL compress/decompress means "not provided" and the manager
 * returns SLEELA_COMPRESSION_ERR_UNSUPPORTED for that direction.
 */
typedef struct sleela_compressor_plugin {
    /* Static identity/metadata -- must always be present. */
    const sleela_compressor_handler *(*describe)(void);

    /* Lifecycle. load() is called by the manager when the algorithm is loaded
     * and may allocate instance state; unload() releases it. Either may be NULL. */
    sleela_compression_result (*load)(sleela_compressor_instance **out_instance);
    void (*unload)(sleela_compressor_instance *instance);

    /* Identify whether `data` (first `len` bytes) is this algorithm's format.
     * Returns SLEELA_COMPRESSION_OK when recognized, SLEELA_COMPRESSION_ERR_FORMAT if not. */
    sleela_compression_result (*probe)(sleela_compressor_instance *instance,
                                       const uint8_t *data, size_t len);

    /* Compress `input` into this algorithm's format. On success *out is filled
     * and owned by the plugin until unload() or a plugin-provided free; a plugin
     * that lacks its backend returns SLEELA_COMPRESSION_ERR_UNSUPPORTED. */
    sleela_compression_result (*compress)(sleela_compressor_instance *instance,
                                          const uint8_t *input, size_t input_len,
                                          sleela_byte_buffer *out);

    /* Decompress this algorithm's format back to raw bytes. Ownership of *out as
     * for compress(). */
    sleela_compression_result (*decompress)(sleela_compressor_instance *instance,
                                            const uint8_t *input, size_t input_len,
                                            sleela_byte_buffer *out);
} sleela_compressor_plugin;

/*
 * Runtime-pluggable backend for an algorithm whose state is
 * SLEELA_COMPRESSION_BACKEND.
 *
 * A plugin ships the contract (probe + metadata) but delegates the actual
 * compress/decompress to a backend a host registers at runtime. When an
 * approved library (zlib, libzstd, liblz4, libbz2, liblzma, brotli, ...) is
 * available, the host builds a small adapter exposing these two function
 * pointers and registers it for the algorithm's id; the plugin's
 * compress/decompress then dispatch to it. With no backend registered, the
 * plugin reports SLEELA_COMPRESSION_ERR_UNSUPPORTED -- never a false success.
 *
 * This keeps the in-tree code free of third-party dependencies while making the
 * "wire a real backend" path concrete and testable.
 */
typedef struct sleela_compression_backend {
    const char *name;   /* adapter/library name, for diagnostics          */
    sleela_compression_result (*compress)(const uint8_t *input, size_t input_len,
                                          sleela_byte_buffer *out);
    sleela_compression_result (*decompress)(const uint8_t *input, size_t input_len,
                                            sleela_byte_buffer *out);
} sleela_compression_backend;

/* Register (or clear, with backend==NULL) the backend for an algorithm id.
 * Returns SLEELA_COMPRESSION_OK, or SLEELA_COMPRESSION_ERR_INVALID for an
 * out-of-range id. The registry is process-global; a host wires backends at
 * startup. */
sleela_compression_result sleela_compression_backend_register(sleela_compressor_id id,
                                                              const sleela_compression_backend *backend);

/* The backend currently registered for an algorithm id, or NULL if none. */
const sleela_compression_backend *sleela_compression_backend_get(sleela_compressor_id id);

/*
 * Each individual algorithm exposes exactly one accessor returning its plugin
 * vtable (a stable singleton). The manager calls these to build its table.
 * Declared here; defined in compression/compressors/<name>/<name>.c (+ .cpp).
 */
const sleela_compressor_plugin *sleela_compressor_plugin_store(void);
const sleela_compressor_plugin *sleela_compressor_plugin_rle(void);
const sleela_compressor_plugin *sleela_compressor_plugin_huffman(void);
const sleela_compressor_plugin *sleela_compressor_plugin_lzw(void);
const sleela_compressor_plugin *sleela_compressor_plugin_deflate(void);
const sleela_compressor_plugin *sleela_compressor_plugin_zlib(void);
const sleela_compressor_plugin *sleela_compressor_plugin_gzip(void);
const sleela_compressor_plugin *sleela_compressor_plugin_bzip2(void);
const sleela_compressor_plugin *sleela_compressor_plugin_lzma(void);
const sleela_compressor_plugin *sleela_compressor_plugin_xz(void);
const sleela_compressor_plugin *sleela_compressor_plugin_zstd(void);
const sleela_compressor_plugin *sleela_compressor_plugin_lz4(void);
const sleela_compressor_plugin *sleela_compressor_plugin_brotli(void);
const sleela_compressor_plugin *sleela_compressor_plugin_snappy(void);
const sleela_compressor_plugin *sleela_compressor_plugin_zip(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSOR_PLUGIN_H */
