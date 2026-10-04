#ifndef SLEELA_CODEC_PLUGIN_H
#define SLEELA_CODEC_PLUGIN_H

/*
 * SLeeLa codec plugin interface.
 *
 * Every individual codec under codecs/codecs/<name>/ implements this one small
 * vtable. The Codec Manager (sleela_codec_manager.h) loads these plugins,
 * tracks their lifecycle, and dispatches probe/decode/encode calls to them.
 * SLeeLa never links an individual codec directly -- it calls the manager, and
 * the manager calls the plugin. This keeps every codec interchangeable and lets
 * a codec be loaded or unloaded at runtime without touching the caller.
 *
 * A codec here is metadata + an explicit capability contract. A plugin whose
 * backend is not present reports its state (e.g. SLEELA_CODEC_BACKEND) and
 * fails cleanly from decode/encode rather than faking success.
 */

#include <stddef.h>
#include <stdint.h>

#include "sleela_codecs.h"   /* sleela_codec_id, sleela_codec_state */

#ifdef __cplusplus
extern "C" {
#endif

/* Result codes shared by every plugin entry point and by the manager. */
typedef enum sleela_codec_result {
    SLEELA_CODEC_OK = 0,            /* success                                   */
    SLEELA_CODEC_ERR_UNSUPPORTED = -1, /* capability not available (e.g. backend absent) */
    SLEELA_CODEC_ERR_INVALID = -2,  /* bad arguments                             */
    SLEELA_CODEC_ERR_FORMAT = -3,   /* input is not this codec's format          */
    SLEELA_CODEC_ERR_IO = -4,       /* read/write/allocation failure             */
    SLEELA_CODEC_ERR_STATE = -5     /* plugin not loaded / wrong lifecycle state */
} sleela_codec_result;

/* A decoded or to-be-encoded PCM buffer. The PCM boundary is the contract
 * between codecs and the Audio API: codecs produce/consume interleaved 16-bit
 * signed PCM here; the manager and Audio layer own conversion beyond that. */
typedef struct sleela_pcm_buffer {
    int16_t *samples;      /* interleaved signed 16-bit samples (owned by caller unless noted) */
    size_t   frame_count;  /* frames (one frame = channels samples)             */
    uint32_t sample_rate;  /* Hz                                                */
    uint16_t channels;     /* 1 = mono, 2 = stereo, ...                         */
} sleela_pcm_buffer;

/* Opaque per-instance state a plugin may allocate in load() and free in unload(). */
typedef struct sleela_codec_instance sleela_codec_instance;

/*
 * The codec plugin vtable. All function pointers are optional except describe();
 * a NULL decode/encode means "not provided" and the manager returns
 * SLEELA_CODEC_ERR_UNSUPPORTED for that direction.
 */
typedef struct sleela_codec_plugin {
    /* Static identity/metadata -- must always be present. */
    const sleela_codec_handler *(*describe)(void);

    /* Lifecycle. load() is called by the manager when the codec is loaded and
     * may allocate instance state; unload() releases it. Either may be NULL. */
    sleela_codec_result (*load)(sleela_codec_instance **out_instance);
    void (*unload)(sleela_codec_instance *instance);

    /* Identify whether `data` (first `len` bytes) is this codec's format.
     * Returns SLEELA_CODEC_OK when recognized, SLEELA_CODEC_ERR_FORMAT if not. */
    sleela_codec_result (*probe)(sleela_codec_instance *instance,
                                 const uint8_t *data, size_t len);

    /* Decode compressed/container input to PCM. On success *out is filled and
     * owned by the plugin until unload() or a plugin-provided free; a plugin
     * that lacks its backend returns SLEELA_CODEC_ERR_UNSUPPORTED. */
    sleela_codec_result (*decode)(sleela_codec_instance *instance,
                                  const uint8_t *input, size_t input_len,
                                  sleela_pcm_buffer *out);

    /* Encode PCM to this codec, writing up to *inout_len bytes into output and
     * setting *inout_len to the bytes produced. */
    sleela_codec_result (*encode)(sleela_codec_instance *instance,
                                  const sleela_pcm_buffer *pcm,
                                  uint8_t *output, size_t *inout_len);
} sleela_codec_plugin;

/*
 * Runtime-pluggable backend for a codec whose state is SLEELA_CODEC_BACKEND.
 *
 * A codec plugin ships the contract (probe + metadata) but delegates the actual
 * encode/decode to a backend that a host registers at runtime. When an approved
 * library (libFLAC, libopus, ...) is available, the host builds a small adapter
 * exposing these two function pointers and registers it for the codec's id; the
 * plugin's decode/encode then dispatch to it. With no backend registered, the
 * plugin reports SLEELA_CODEC_ERR_UNSUPPORTED -- never a false success.
 *
 * This keeps the in-tree code free of third-party dependencies while making the
 * "wire a real backend" path concrete and testable.
 */
typedef struct sleela_codec_backend {
    const char *name;   /* adapter/library name, for diagnostics          */
    sleela_codec_result (*decode)(const uint8_t *input, size_t input_len,
                                  sleela_pcm_buffer *out);
    sleela_codec_result (*encode)(const sleela_pcm_buffer *pcm,
                                  uint8_t *output, size_t *inout_len);
} sleela_codec_backend;

/* Register (or clear, with backend==NULL) the backend for a codec id. Returns
 * SLEELA_CODEC_OK, or SLEELA_CODEC_ERR_INVALID for an out-of-range id. The
 * registry is process-global and simple; a host wires backends at startup. */
sleela_codec_result sleela_codec_backend_register(sleela_codec_id id,
                                                  const sleela_codec_backend *backend);

/* The backend currently registered for a codec id, or NULL if none. */
const sleela_codec_backend *sleela_codec_backend_get(sleela_codec_id id);

/*
 * Each individual codec exposes exactly one accessor returning its plugin
 * vtable (a stable singleton). The manager calls these to build its table.
 * Declared here; defined in codecs/codecs/<name>/<name>.c (+ optional .cpp).
 */
const sleela_codec_plugin *sleela_codec_plugin_pcm_wav(void);
const sleela_codec_plugin *sleela_codec_plugin_aiff(void);
const sleela_codec_plugin *sleela_codec_plugin_flac(void);
const sleela_codec_plugin *sleela_codec_plugin_alac(void);
const sleela_codec_plugin *sleela_codec_plugin_mp3(void);
const sleela_codec_plugin *sleela_codec_plugin_aac(void);
const sleela_codec_plugin *sleela_codec_plugin_he_aac(void);
const sleela_codec_plugin *sleela_codec_plugin_vorbis(void);
const sleela_codec_plugin *sleela_codec_plugin_opus(void);
const sleela_codec_plugin *sleela_codec_plugin_speex(void);
const sleela_codec_plugin *sleela_codec_plugin_wma(void);
const sleela_codec_plugin *sleela_codec_plugin_ac3(void);
const sleela_codec_plugin *sleela_codec_plugin_eac3(void);
const sleela_codec_plugin *sleela_codec_plugin_amr_nb(void);
const sleela_codec_plugin *sleela_codec_plugin_amr_wb(void);
const sleela_codec_plugin *sleela_codec_plugin_g711_mulaw(void);
const sleela_codec_plugin *sleela_codec_plugin_g711_alaw(void);
const sleela_codec_plugin *sleela_codec_plugin_midi(void);
const sleela_codec_plugin *sleela_codec_plugin_matroska_audio(void);
const sleela_codec_plugin *sleela_codec_plugin_webm_audio(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_PLUGIN_H */
