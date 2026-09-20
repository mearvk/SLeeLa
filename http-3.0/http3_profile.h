/* ==========================================================================
 * http3_profile.h -- SLeeLa HTTP 3.0 "colors": named configuration profiles.
 *
 * A SLeeLa program specifies its HTTP behavior by naming a COLOR. A color is a
 * named bundle of protocol knobs -- wire form, advisory flags, and the
 * per-packet integrity profile (MAC / INTACTX tamper threshold / replay NONCE /
 * goods & services basket) -- so source code can say "use color green" instead
 * of setting each switch by hand.
 *
 * This layer is ADDITIVE and does not change the envelope or pipeline wire
 * formats: it only chooses which optional behaviors a connection turns on. The
 * same palette is mirrored in http3_flow.py and documented in HTTP-COLORS.md,
 * and the shipped palette is loaded from the config file http3_colors.conf.
 * ========================================================================== */
#ifndef HTTP3_PROFILE_H
#define HTTP3_PROFILE_H

#include <stddef.h>
#include <stdint.h>

#include "http3_envelope.h"  /* HTTP3_FLAG_*, wire form */

#ifdef __cplusplus
extern "C" {
#endif

/* Wire form a color selects. */
typedef enum {
    HTTP3_WIRE_TEXT   = 0, /* interoperable textual wire   */
    HTTP3_WIRE_BINARY = 1  /* negotiated binary wire       */
} http3_wire_form_t;

/* Max length of a color name. */
#define HTTP3_COLOR_NAME_MAX 24u

/*
 * A resolved HTTP color: the concrete configuration a SLeeLa program gets by
 * naming a color. All fields are advisory selectors over already-supported
 * behavior; none change the wire layout itself.
 */
typedef struct {
    char              name[HTTP3_COLOR_NAME_MAX]; /* color name, e.g. "green"  */
    http3_wire_form_t wire;                       /* text or binary            */
    uint8_t           flags;                      /* default envelope flags    */
    int               mac_required;               /* verify keyed-MAC DIGEST   */
    int               intactx_enabled;            /* apply INTACTX tamper gate */
    uint16_t          intactx_threshold;          /* 0 => library default      */
    int               replay_guard;               /* apply NONCE replay gate   */
    int               basket_required;            /* expect the basket present */
} http3_profile_t;

/* Number of colors in the shipped palette. */
#define HTTP3_COLOR_COUNT 4u

/*
 * Resolve a color by name into `out`. Recognized shipped colors:
 *
 *   "green"  -- textual, core only: no MAC/INTACTX/NONCE gate, no basket
 *               requirement. The lightest, most interoperable profile.
 *   "amber"  -- textual, MAC required + replay guard; INTACTX off. Balanced.
 *   "red"    -- binary, full integrity: MAC + INTACTX tamper gate + replay
 *               guard + basket required. The strictest profile.
 *   "black"  -- binary, full integrity like red AND compressed payload flag.
 *
 * Returns 0 on success, -1 if the name is unknown or an argument is NULL.
 */
int http3_profile_by_color(const char *color, http3_profile_t *out);

/* Return the i-th shipped color (0..HTTP3_COLOR_COUNT-1), or NULL if out of
 * range. Lets callers enumerate the palette. */
const http3_profile_t *http3_profile_palette(size_t i);

/*
 * Load a color palette entry from a config file (http3_colors.conf format:
 * see HTTP-COLORS.md). Looks up `color` in `path`; on success fills `out` and
 * returns 0. Returns -1 if the file/entry is missing or malformed. A NULL
 * `path` uses HTTP3_COLORS_DEFAULT_PATH.
 */
#define HTTP3_COLORS_DEFAULT_PATH "http3_colors.conf"
int http3_profile_load(const char *path, const char *color, http3_profile_t *out);

/* Apply a resolved color to a pipeline: sets the INTACTX threshold (when the
 * color enables INTACTX) so the pipeline's gate matches the color. The MAC key
 * is still installed separately via http3_pipeline_set_mac_key. Returns 0 on
 * success, -1 on a NULL argument. Declared here but defined in http3_profile.c
 * to avoid a hard pipeline dependency in this header. */
struct http3_pipeline; /* fwd decl */
int http3_profile_apply(const http3_profile_t *profile, struct http3_pipeline *pipe);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_PROFILE_H */
