/*
 * ballum/include/sleela_ballum/ballum_transition.h
 * Program entry / exit transitions for the Ballum program host.
 *
 * ballum_effects.h reveals a foreground over a background with PER-PIXEL MASKS:
 * a pixel is either shown or hidden, the foreground never moves. A hosted
 * program needs more: the whole program frame must SLIDE in, break into a
 * MOSAIC, or ASSEMBLE from drifting particles. Those transitions sample the
 * source frame at displaced positions, so they work on a whole frame rather than
 * one pixel at a time.
 *
 * A transition is a pure function of (kind, progress t, source frame, background):
 * integer-only, deterministic, no allocation, no device access. The same call
 * serves entry (t: 0 -> 100) and exit (call with 100 - t), so every effect
 * reverses for free. Contract for every kind:
 *
 *   t == 100  ->  dst equals src (composited over bg by src alpha)
 *   t ==   0  ->  dst is the background (fade/slide/pixelize/disintegrate)
 *
 * Kinds:
 *   none          the program appears at once
 *   fx            any mask effect from ballum_effects.h (fade, iris-in, ...)
 *   slide-left    the frame slides in from the right edge (moves left)
 *   slide-right   ... from the left edge
 *   slide-up      ... from the bottom edge
 *   slide-down    ... from the top edge
 *   pixelize      a coarse mosaic of the frame sharpens to full resolution
 *   disintegrate  every pixel flies in from a scattered position and settles
 *                 (the reverse of dust blowing away); played backwards it is
 *                 the disintegration itself
 */
#ifndef SLEELA_BALLUM_TRANSITION_H
#define SLEELA_BALLUM_TRANSITION_H

#include "ballum_effects.h"
#include "ballum_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BALLUM_TRANSITION_NONE = 0,
    BALLUM_TRANSITION_FX,            /* uses ballum_transition_t.fx */
    BALLUM_TRANSITION_SLIDE_LEFT,
    BALLUM_TRANSITION_SLIDE_RIGHT,
    BALLUM_TRANSITION_SLIDE_UP,
    BALLUM_TRANSITION_SLIDE_DOWN,
    BALLUM_TRANSITION_PIXELIZE,
    BALLUM_TRANSITION_DISINTEGRATE,
    BALLUM_TRANSITION__COUNT         /* KEEP LAST */
} ballum_transition_kind_t;

typedef struct {
    ballum_transition_kind_t kind;
    ballum_fx_t              fx;     /* read only when kind == FX */
} ballum_transition_t;

/* Canonical name: "none", "slide-left", ..., "disintegrate", or, for kind FX,
 * the catalog name ("fade", "iris-in", ...). Never NULL. */
const char *ballum_transition_name(const ballum_transition_t *tr);

/* Resolve `name` into *out. Host kinds win over catalog names, so "slide-left"
 * is the true slide. Returns 1 on success and 0 for an unknown name (*out is
 * then set to NONE, which fails closed to "appear at once"). */
int ballum_transition_by_name(const char *name, ballum_transition_t *out);

/* Number of selectable transition names (host kinds + the effect catalog). */
int ballum_transition_count(void);

/* Compose `src` (w*h pixels, row-major, tightly packed) at progress t (0..100,
 * clamped) over the solid background `bg` into `dst` (w*h pixels). `dst` must
 * not alias `src`. w and h must be non-zero and w*h must not overflow size_t;
 * otherwise nothing is written. Returns 1 when dst was written, else 0. */
int ballum_transition_frame(const ballum_transition_t *tr, int t,
                            uint32_t w, uint32_t h,
                            const ballum_color_t *src, ballum_color_t bg,
                            ballum_color_t *dst);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_TRANSITION_H */
