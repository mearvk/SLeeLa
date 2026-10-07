#ifndef SLEELA_UI_DRAW_H
#define SLEELA_UI_DRAW_H
/* =============================================================================
 * SleelaUI(TM) Draw API -- the comprehensive, low-level 2D drawing surface for
 * developers and agents.
 *
 * Where sleela_ui.h gives you widgets, this header gives you the PIXELS. It
 * exposes the toolkit's own software rasterizer as a stable C ABI so you can:
 *
 *   * own an off-screen pixel surface (a draw context) at any size;
 *   * draw with a complete primitive set -- clear, points, lines (AA), rects,
 *     rounded rects, circles/ellipses/arcs, polylines, triangles, gradients
 *     (linear + radial), text, and surface-to-surface blits;
 *   * work at the PIXEL LEVEL -- read and write individual pixels, choose a
 *     blend mode, lock the raw buffer for direct access, apply the clip;
 *   * control BUFFERING -- single or double buffered, with an explicit
 *     back-buffer you draw into and a present()/swap() that publishes it, so
 *     animation never tears or flickers;
 *   * pace REFRESH -- a frame clock with a target rate (FPS), per-frame delta
 *     time, an accumulator for fixed-step simulation, and a sleep hint so a
 *     render loop runs smoothly without busy-spinning.
 *
 * A draw context can be free-standing (an image you render and read back) or
 * BOUND TO A WIDGET -- the SLUICanvasView widget (see sleela_ui.h) hands its
 * per-frame surface to a developer draw callback, which is exactly how the
 * animated Throbber and any custom visualisation are built.
 *
 * All colours are the same 0xRRGGBBAA words as sleela_ui.h (SLUIColor), and all
 * geometry uses SLUIRect/SLUIPoint, so the two headers compose cleanly.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Draw context: an owned RGBA pixel surface with an optional back buffer.
 * ------------------------------------------------------------------------- */
typedef struct SLUIDrawContext SLUIDrawContext;

typedef enum {
    SLUI_BUFFER_SINGLE = 0, /* draw and read the one surface directly       */
    SLUI_BUFFER_DOUBLE = 1  /* draw into a back buffer, present() to publish */
} SLUIBufferMode;

/* Pixel compositing mode for every primitive and set_pixel. */
typedef enum {
    SLUI_BLEND_OVER = 0,     /* src OVER dst (straight-alpha, the default)   */
    SLUI_BLEND_COPY = 1,     /* overwrite dst with src (ignores dst alpha)   */
    SLUI_BLEND_ADD = 2,      /* additive (light accumulation; great for glow)*/
    SLUI_BLEND_MULTIPLY = 3, /* multiplicative (darken)                      */
    SLUI_BLEND_SCREEN = 4,   /* screen (lighten)                             */
    SLUI_BLEND_MAX = 5       /* per-channel maximum                          */
} SLUIBlendMode;

/* Create a free-standing context of `width` x `height` device pixels. */
SLUIDrawContext *slui_draw_create(int width, int height, SLUIBufferMode mode);
void slui_draw_destroy(SLUIDrawContext *dc);

/* Resize the surface(s); contents are undefined afterward (clear to repaint). */
SLUIStatus slui_draw_resize(SLUIDrawContext *dc, int width, int height);
int slui_draw_width(const SLUIDrawContext *dc);
int slui_draw_height(const SLUIDrawContext *dc);

/* The active blend mode applied by subsequent primitives. */
void slui_draw_set_blend(SLUIDrawContext *dc, SLUIBlendMode mode);
SLUIBlendMode slui_draw_get_blend(const SLUIDrawContext *dc);

/* A global multiplier (0..1) applied to every source alpha -- a layer opacity. */
void slui_draw_set_opacity(SLUIDrawContext *dc, double opacity);

/* Clip subsequent drawing to a rectangle; push/pop nest; reset clears all. */
void slui_draw_clip_push(SLUIDrawContext *dc, SLUIRect r);
void slui_draw_clip_pop(SLUIDrawContext *dc);
void slui_draw_clip_reset(SLUIDrawContext *dc);

/* --------------------------------------------------------------------------
 * Buffering. In SLUI_BUFFER_DOUBLE mode you draw into the back buffer and call
 * present() to copy it to the front; read-backs and widget blits see the front.
 * In SLUI_BUFFER_SINGLE mode present() is a no-op. clear() wipes the active
 * (draw) buffer to a solid colour.
 * ------------------------------------------------------------------------- */
void slui_draw_clear(SLUIDrawContext *dc, SLUIColor color);
void slui_draw_present(SLUIDrawContext *dc); /* publish back -> front         */

/* --------------------------------------------------------------------------
 * Pixel-level control.
 * ------------------------------------------------------------------------- */
/* Set/get one pixel. set honours the active blend mode and opacity; get reads
 * the front buffer and returns straight-alpha 0xRRGGBBAA (0 if out of range). */
void slui_draw_set_pixel(SLUIDrawContext *dc, int x, int y, SLUIColor color);
SLUIColor slui_draw_get_pixel(const SLUIDrawContext *dc, int x, int y);

/* Blend one pixel with explicit coverage [0..1] (sub-pixel AA by hand). */
void slui_draw_blend_pixel(SLUIDrawContext *dc, int x, int y, SLUIColor color,
                           double coverage);

/* Lock the raw back-buffer words for direct read/write. The layout is tightly
 * packed 0xAARRGGBB, `*stride_words` words per row (== width). Call unlock when
 * done; the pointer is invalidated by resize/destroy. Returns NULL on error. */
uint32_t *slui_draw_lock(SLUIDrawContext *dc, int *stride_words);
void slui_draw_unlock(SLUIDrawContext *dc);

/* Copy the front buffer out as tightly-packed RGBA8 (4 bytes/pixel, top-down),
 * for screenshots, encoders, or tests. `cap` is the byte capacity of `out`;
 * returns the number of bytes the full image needs (write happens if it fits). */
size_t slui_draw_read_rgba(const SLUIDrawContext *dc, uint8_t *out, size_t cap);

/* --------------------------------------------------------------------------
 * Primitives. All honour the clip, blend mode, and layer opacity. Coordinates
 * are device pixels; thickness and radii are in pixels (doubles for sub-pixel).
 * ------------------------------------------------------------------------- */
void slui_draw_fill_rect(SLUIDrawContext *dc, SLUIRect r, SLUIColor color);
void slui_draw_fill_round_rect(SLUIDrawContext *dc, SLUIRect r, double radius,
                               SLUIColor color);
void slui_draw_stroke_rect(SLUIDrawContext *dc, SLUIRect r, double thickness,
                           SLUIColor color);
void slui_draw_stroke_round_rect(SLUIDrawContext *dc, SLUIRect r, double radius,
                                 double thickness, SLUIColor color);

/* Antialiased line of a given thickness (round-capped). */
void slui_draw_line(SLUIDrawContext *dc, double x0, double y0, double x1,
                    double y1, double thickness, SLUIColor color);
/* A connected polyline through `count` points (2 ints each: x,y). */
void slui_draw_polyline(SLUIDrawContext *dc, const int *xy, int count,
                        double thickness, SLUIColor color);

void slui_draw_fill_circle(SLUIDrawContext *dc, double cx, double cy,
                           double radius, SLUIColor color);
void slui_draw_stroke_circle(SLUIDrawContext *dc, double cx, double cy,
                             double radius, double thickness, SLUIColor color);
void slui_draw_fill_ellipse(SLUIDrawContext *dc, double cx, double cy, double rx,
                            double ry, SLUIColor color);
/* An arc (stroked) from `start_rad` sweeping `sweep_rad` radians, 0 = +x axis. */
void slui_draw_arc(SLUIDrawContext *dc, double cx, double cy, double radius,
                   double start_rad, double sweep_rad, double thickness,
                   SLUIColor color);

/* A filled triangle (flat-shaded). */
void slui_draw_fill_triangle(SLUIDrawContext *dc, double x0, double y0,
                             double x1, double y1, double x2, double y2,
                             SLUIColor color);

/* Gradients filling a rect. Linear interpolates c0->c1 along the axis; radial
 * interpolates centre->edge within the rect's inscribed radius. */
void slui_draw_linear_gradient(SLUIDrawContext *dc, SLUIRect r,
                               SLUIOrientation axis, SLUIColor c0, SLUIColor c1);
void slui_draw_radial_gradient(SLUIDrawContext *dc, double cx, double cy,
                               double radius, SLUIColor inner, SLUIColor outer);

/* Text, using the window/application font the host last selected (or a default
 * system face). Draws left-to-right from the baseline at (x, baseline). */
void slui_draw_text(SLUIDrawContext *dc, const char *utf8, int x, int baseline,
                    SLUIColor color);

/* Blit another context's front buffer at (dx, dy) with the active blend/opacity.
 * `src_rect` may be NULL to copy the whole source. */
void slui_draw_blit(SLUIDrawContext *dst, const SLUIDrawContext *src, int dx,
                    int dy, const SLUIRect *src_rect);

/* --------------------------------------------------------------------------
 * Colour helpers for predictive / generative drawing. These are pure value
 * utilities (no context needed) that make smooth, natural colour fields easy.
 * ------------------------------------------------------------------------- */
/* Linear blend of two colours, t in [0,1]. */
SLUIColor slui_color_lerp(SLUIColor a, SLUIColor b, double t);
/* HSV -> packed colour. h in [0,360), s,v,a in [0,1]. Ideal for flowing hues. */
SLUIColor slui_color_hsv(double h, double s, double v, double a);
/* A perceptual-ish luminance of a colour in [0,1]. */
double slui_color_luminance(SLUIColor c);

/* --------------------------------------------------------------------------
 * Frame clock: refresh-rate control for a render loop.
 *
 * Typical loop:
 *     SLUIFrameClock *fc = slui_frame_clock_create(60.0);
 *     while (running) {
 *         double dt = slui_frame_clock_tick(fc);   // seconds since last tick
 *         update(dt);
 *         draw(dc);
 *         slui_draw_present(dc);
 *         int ms = slui_frame_clock_sleep_hint(fc);// how long to idle for 60Hz
 *         if (ms > 0) host_sleep_ms(ms);
 *     }
 * ------------------------------------------------------------------------- */
typedef struct SLUIFrameClock SLUIFrameClock;

SLUIFrameClock *slui_frame_clock_create(double target_fps);
void slui_frame_clock_destroy(SLUIFrameClock *fc);

/* Set/get the target refresh rate in frames per second (0 = unthrottled). */
void slui_frame_clock_set_fps(SLUIFrameClock *fc, double target_fps);
double slui_frame_clock_get_fps(const SLUIFrameClock *fc);

/* Advance the clock one frame; returns the delta time in seconds since the
 * previous tick (clamped to a sane max so a stall can't explode a simulation). */
double slui_frame_clock_tick(SLUIFrameClock *fc);

/* Seconds since the clock was created (monotonic animation time). */
double slui_frame_clock_elapsed(const SLUIFrameClock *fc);

/* The measured, smoothed frames-per-second actually being achieved. */
double slui_frame_clock_measured_fps(const SLUIFrameClock *fc);

/* Milliseconds the caller should idle to hit the target rate (0 if behind). */
int slui_frame_clock_sleep_hint(const SLUIFrameClock *fc);

/* Fixed-timestep helper: accumulate real time and drain it in `step` chunks so
 * physics/field updates stay deterministic regardless of frame rate. Call tick
 * first, then repeatedly call this until it returns 0, running one fixed step
 * each time it returns 1. */
int slui_frame_clock_fixed_step(SLUIFrameClock *fc, double step);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_UI_DRAW_H */
