/*
 * ballum/include/sleela_ballum/ballum_effects.h
 * Ballum transition effects -- a catalog of per-pixel compositing effects a
 * session uses to reveal a logo over a background across a progress timeline.
 *
 * Model: an effect is a pure function. Given the effect id, a progress t in
 * [0,100], the destination geometry, the destination pixel (x,y), the
 * foreground color (e.g. the logo pixel, straight alpha) and the background
 * color, it returns the composited color for that pixel at that moment. No
 * state, no allocation, no device access -- the caller plots the result. This
 * makes every effect trivially testable and deterministic.
 *
 * The set is deliberately broad and fun: fades, wipes, iris, bars/blinds,
 * dissolve, pixelate/mosaic, slides, zoom, spotlight, ripple, checkerboard,
 * diamond, radial clock, and sparkle. All are framed as "reveal fg over bg as
 * t goes 0 -> 100"; at t==100 the foreground is fully shown where it is opaque.
 */
#ifndef SLEELA_BALLUM_EFFECTS_H
#define SLEELA_BALLUM_EFFECTS_H

#include "ballum_manager.h"   /* ballum_color_t */

#ifdef __cplusplus
extern "C" {
#endif

/* The effect catalog. Keep in sync with lib/ballum/SLBallumEffect.sleela. */
typedef enum {
    BALLUM_FX_FADE = 0,        /* global cross-fade bg -> fg                   */
    BALLUM_FX_WIPE_LEFT,       /* reveal sweeping from the left edge           */
    BALLUM_FX_WIPE_RIGHT,
    BALLUM_FX_WIPE_UP,
    BALLUM_FX_WIPE_DOWN,
    BALLUM_FX_IRIS_IN,         /* expanding circle from the center             */
    BALLUM_FX_IRIS_OUT,        /* closing circle toward the center             */
    BALLUM_FX_BARS,            /* vertical blinds opening                      */
    BALLUM_FX_BLINDS,          /* horizontal blinds opening                    */
    BALLUM_FX_DISSOLVE,        /* ordered-dither random-looking dissolve       */
    BALLUM_FX_PIXELATE,        /* fg emerges from coarse mosaic to sharp       */
    BALLUM_FX_SLIDE_LEFT,      /* fg slides in from the right                  */
    BALLUM_FX_SLIDE_UP,        /* fg slides in from the bottom                 */
    BALLUM_FX_ZOOM,            /* fg scales up from center while fading        */
    BALLUM_FX_SPOTLIGHT,       /* soft radial spotlight grows from center      */
    BALLUM_FX_RIPPLE,          /* concentric rings sweep outward               */
    BALLUM_FX_CHECKER,         /* checkerboard squares fill in                 */
    BALLUM_FX_DIAMOND,         /* expanding diamond (Manhattan iris)           */
    BALLUM_FX_CLOCK,           /* radial clock-wipe sweep                      */
    BALLUM_FX_SPARKLE,         /* twinkling reveal with a shimmer over fg      */
    BALLUM_FX__COUNT           /* KEEP LAST                                    */
} ballum_fx_t;

/* The canonical lowercase name of an effect ("fade","wipe-left",...). */
const char *ballum_fx_name(ballum_fx_t fx);

/* Resolve an effect by name; returns BALLUM_FX_FADE for an unknown name. */
ballum_fx_t ballum_fx_by_name(const char *name);

/* Number of effects in the catalog. */
int ballum_fx_count(void);

/* Composite one pixel for effect `fx` at progress `t` (0..100). (w,h) is the
 * destination size; (x,y) the destination pixel; `fg` the foreground/logo color
 * at that pixel (straight alpha; a==0 means "no logo here"); `bg` the
 * background. Returns the color to plot. Pure and deterministic. */
ballum_color_t ballum_fx_pixel(ballum_fx_t fx, int t, int w, int h,
                               int x, int y,
                               ballum_color_t fg, ballum_color_t bg);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_EFFECTS_H */
