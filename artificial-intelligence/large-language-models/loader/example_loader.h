#ifndef SLEELA_AI_LLM_EXAMPLE_LOADER_H
#define SLEELA_AI_LLM_EXAMPLE_LOADER_H
/*
 * SLeeLa example loader — a loader-handler idiom for user classes.
 *
 * The idiom, in one sentence: a user writes one small HANDLER that knows how to
 * turn an instance of THEIR type (a word, a math fact, an object/record) into a
 * fixed-width feature vector and target vector; the FRAMEWORK owns the careful
 * machinery — bounded buffers, deterministic ordering, a known feature width,
 * fail-closed validation, and the feed into a real slnn_* network.
 *
 * It is deliberately:
 *   - subtle: one tiny vtable (encode + describe); nothing else to implement.
 *     The user never touches a network, a buffer, or an allocator.
 *   - performant: a single caller-owned scratch buffer is reused across every
 *     item (zero per-item heap allocation in the hot loop); the handler writes
 *     straight into framework-owned feature/target slices.
 *   - clear: one width for features, one for targets, validated once; every item
 *     is encoded the same way; an item that does not fit fails closed with a
 *     precise reason rather than silently truncating.
 *
 * Careful-by-construction (a "samewise careful" posture): the loader never
 * fabricates data. A handler that cannot encode an item returns SLLME_SKIP (the
 * item is counted and omitted, honestly) or SLLME_ERROR (the load stops). A
 * word not in the known vocabulary is the handler's decision (map to <unk>, or
 * skip) — the framework does not guess.
 *
 * No weights or data are bundled; the user supplies the items and the handler,
 * and (for word handlers) a known vocabulary to encode against.
 */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLME_API __declspec(dllexport)
# else
#  define SLLME_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLME_API __attribute__((visibility("default")))
#else
# define SLLME_API
#endif

#define SLLME_ABI_VERSION 1u

typedef enum sllme_status {
 SLLME_OK=0, SLLME_INVALID_ARGUMENT=1, SLLME_WIDTH_MISMATCH=2,
 SLLME_HANDLER_ERROR=3, SLLME_EMPTY=4
} sllme_status;

/* Per-item outcome the handler returns from encode(). */
typedef enum sllme_item_result {
 SLLME_ITEM_OK=0,    /* feature+target written; use this item */
 SLLME_ITEM_SKIP=1,  /* cannot/should-not encode; omit honestly (counted) */
 SLLME_ITEM_ERROR=2  /* unrecoverable; stop the load */
} sllme_item_result;

/*
 * The user HANDLER for their type. All pointers are borrowed for the call only.
 *
 *   describe   (optional) a short, stable name of what this handler encodes.
 *   feature_width / target_width: the fixed vector widths this handler writes.
 *   encode: write exactly feature_width doubles into 'features' and exactly
 *           target_width doubles into 'target' for the item at 'item'
 *           (user_data carries the user's context, e.g. a vocabulary handle).
 *           It must not write past the given widths. Returning SLLME_ITEM_OK
 *           means both slices were fully written.
 *
 * The idiom: implement encode() for YOUR class; the framework calls it once per
 * item with framework-owned, correctly sized slices.
 */
typedef struct sllme_handler {
 uint32_t abi_version;
 const char *(*describe)(void *user_data);           /* may be NULL */
 size_t feature_width;
 size_t target_width;
 sllme_item_result (*encode)(void *user_data,
                             const void *item,
                             double *features, size_t feature_width,
                             double *target, size_t target_width);
 void *user_data;
} sllme_handler;

/* A caller-provided, reusable scratch region the loader encodes into, item by
   item. Sized once for the handler's widths; reused with no per-item alloc. */
typedef struct sllme_scratch {
 double *features;    /* at least feature_width doubles */
 double *target;      /* at least target_width doubles  */
 size_t  feature_cap;
 size_t  target_cap;
} sllme_scratch;

/* A sink the loader calls with each successfully encoded example. Returning
   non-zero stops the load (reported as SLLME_OK with the count so far). The
   slices are borrowed and valid only for the call. */
typedef int (*sllme_sink_fn)(void *sink_user_data,
                             const double *features, size_t feature_width,
                             const double *target, size_t target_width);

/* Running counts for one load. */
typedef struct sllme_report {
 size_t seen;      /* items presented */
 size_t loaded;    /* encoded OK and delivered to the sink */
 size_t skipped;   /* handler returned SKIP */
} sllme_report;

SLLME_API const char *sllme_status_string(sllme_status status);

/* Validate a handler + scratch pairing once (widths agree, encode present,
   scratch large enough). Call before a hot loop; returns SLLME_OK if usable. */
SLLME_API sllme_status sllme_validate(const sllme_handler *handler,
                                      const sllme_scratch *scratch);

/* Encode a single item through the handler into scratch, then deliver it to the
   sink. Returns SLLME_OK (loaded or skipped — see *result), or a hard error. */
SLLME_API sllme_status sllme_load_one(const sllme_handler *handler,
                                      sllme_scratch *scratch,
                                      const void *item,
                                      sllme_sink_fn sink, void *sink_user_data,
                                      sllme_item_result *result);

/* Encode an ARRAY of items (items is a byte buffer of 'count' records each of
   'stride' bytes; item i is at items + i*stride). Each encoded example is
   delivered to the sink. Fail-closed on the first SLLME_ITEM_ERROR. Fills
   *report. A single caller-owned scratch is reused for all items. */
SLLME_API sllme_status sllme_load_array(const sllme_handler *handler,
                                        sllme_scratch *scratch,
                                        const void *items, size_t count, size_t stride,
                                        sllme_sink_fn sink, void *sink_user_data,
                                        sllme_report *report);

/* ---- A ready-made sink that trains a real slnn_* network ----
   Pass an slnn_sink as sink_user_data with sllme_slnn_sink as the sink_fn to
   feed every encoded example straight into slnn_network_train_sample at the
   given learning rate. Declared here; defined in example_loader_slnn.c so the
   base loader stays dependency-free. */
typedef struct sllme_slnn_sink sllme_slnn_sink;
SLLME_API int sllme_slnn_sink_train(void *sink_user_data,
                                    const double *features, size_t feature_width,
                                    const double *target, size_t target_width);

#ifdef __cplusplus
}
#endif
#endif
