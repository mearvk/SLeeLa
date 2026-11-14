# SleelaUI™ Draw API

A comprehensive, low-level 2D drawing surface for **developers and agents**.
Where [`sleela_ui.h`](include/sleela_ui.h) gives you widgets,
[`sleela_ui_draw.h`](include/sleela_ui_draw.h) gives you the **pixels** —
rendered by the toolkit's own software rasterizer, so your drawing is
pixel-identical on Windows, macOS, and Linux/Unix.

It is the substrate the animated **Throbber** is built on, and the public API
any custom visualisation uses through the **Canvas View** widget.

## What you get

- **Draw context** — an owned RGBA surface at any size, single- or
  double-buffered.
- **Pixel-level control** — read/write individual pixels, a per-op blend mode,
  a layer opacity, a clip stack, and a raw buffer lock for direct access.
- **A complete primitive set** — clear, points, antialiased lines + polylines,
  rects, rounded rects, circles, ellipses, arcs, triangles, linear and radial
  gradients, text, and surface-to-surface blits.
- **Buffering** — draw into a back buffer and `present()` it so animation never
  tears or flickers.
- **Refresh-rate control** — a frame clock with a target FPS, per-frame delta
  time, a smoothed measured rate, a sleep hint, and a fixed-timestep accumulator
  for deterministic simulation.
- **Predictive-colour helpers** — HSV, colour lerp, and luminance for smooth,
  natural, flowing colour fields.

## Draw context

```c
#include "sleela_ui_draw.h"

SLUIDrawContext *dc = slui_draw_create(640, 360, SLUI_BUFFER_DOUBLE);

slui_draw_clear(dc, slui_rgb(0x0d, 0x0d, 0x0f));
slui_draw_fill_round_rect(dc, (SLUIRect){20, 20, 120, 60}, 8, slui_rgb(0x5e,0x9c,0xff));
slui_draw_line(dc, 0, 0, 640, 360, 2.0, slui_rgb(255,255,255));
slui_draw_fill_circle(dc, 320, 180, 40, slui_color_hsv(205, 0.8, 1.0, 1.0));
slui_draw_present(dc);                 /* publish back -> front */

/* read one pixel, or the whole image as RGBA8 */
SLUIColor p = slui_draw_get_pixel(dc, 320, 180);
uint8_t rgba[640*360*4];
slui_draw_read_rgba(dc, rgba, sizeof rgba);

slui_draw_destroy(dc);
```

### Buffer modes

| Mode | Behaviour |
|---|---|
| `SLUI_BUFFER_SINGLE` | Draw into and read the one surface; `present()` is a no-op. |
| `SLUI_BUFFER_DOUBLE` | Draw into a back buffer; `present()` copies it to the front. Read-backs and widget blits see the front. Use for flicker-free animation. |

### Blend modes

Set with `slui_draw_set_blend`; applies to every primitive and `set_pixel`.

| Mode | Effect |
|---|---|
| `SLUI_BLEND_OVER` | src over dst (straight-alpha, the default) |
| `SLUI_BLEND_COPY` | overwrite dst with src |
| `SLUI_BLEND_ADD` | additive — light accumulation; great for glow/water crests |
| `SLUI_BLEND_MULTIPLY` | darken |
| `SLUI_BLEND_SCREEN` | lighten |
| `SLUI_BLEND_MAX` | per-channel maximum |

`slui_draw_set_opacity(dc, 0..1)` multiplies every source alpha — a layer
opacity. `slui_draw_clip_push/pop/reset` constrain drawing to a rectangle.

## Pixel-level control

```c
slui_draw_set_pixel(dc, x, y, color);           /* honours blend + opacity   */
SLUIColor c = slui_draw_get_pixel(dc, x, y);    /* front buffer, 0xRRGGBBAA  */
slui_draw_blend_pixel(dc, x, y, color, 0.5);    /* sub-pixel AA by hand      */

int stride;                                      /* words per row (== width)  */
uint32_t *raw = slui_draw_lock(dc, &stride);    /* 0xAARRGGBB words           */
/* ... touch raw[y*stride + x] directly ... */
slui_draw_unlock(dc);
```

## Primitives

```c
void slui_draw_fill_rect / fill_round_rect / stroke_rect / stroke_round_rect(...);
void slui_draw_line(dc, x0,y0, x1,y1, thickness, color);      /* AA, round-cap */
void slui_draw_polyline(dc, xy, count, thickness, color);
void slui_draw_fill_circle / stroke_circle / fill_ellipse(...);
void slui_draw_arc(dc, cx,cy, r, start_rad, sweep_rad, thickness, color);
void slui_draw_fill_triangle(dc, x0,y0, x1,y1, x2,y2, color);
void slui_draw_linear_gradient(dc, rect, axis, c0, c1);
void slui_draw_radial_gradient(dc, cx,cy, radius, inner, outer);
void slui_draw_text(dc, utf8, x, baseline, color);
void slui_draw_blit(dst, src, dx, dy, src_rect);              /* ctx -> ctx    */
```

All honour the active clip, blend mode, and layer opacity. Coordinates are
device pixels; thicknesses and radii are doubles (sub-pixel).

## Colour helpers

```c
SLUIColor m = slui_color_lerp(a, b, t);           /* blend, t in [0,1]        */
SLUIColor h = slui_color_hsv(205, 0.8, 1.0, 1.0); /* flowing hues, h in deg   */
double     L = slui_color_luminance(c);           /* perceptual-ish, [0,1]    */
```

## Refresh-rate control (frame clock)

```c
SLUIFrameClock *fc = slui_frame_clock_create(60.0);   /* target FPS           */
while (running) {
    double dt = slui_frame_clock_tick(fc);            /* seconds since last   */
    /* deterministic fixed-step updates regardless of frame rate */
    while (slui_frame_clock_fixed_step(fc, 1.0/120.0)) simulate(1.0/120.0);
    draw(dc);
    slui_draw_present(dc);
    int ms = slui_frame_clock_sleep_hint(fc);          /* idle to hit 60Hz     */
    if (ms > 0) host_sleep_ms(ms);
}
double shown = slui_frame_clock_measured_fps(fc);      /* smoothed actual FPS  */
double t     = slui_frame_clock_elapsed(fc);           /* animation time       */
slui_frame_clock_destroy(fc);
```

`tick()` clamps a long stall so a paused window can't explode a simulation.

## Driving an animation inside a window: the Canvas View

A **Canvas View** widget owns a double-buffered context sized to its bounds and,
each frame while visible, calls your draw function then presents + blits the
result into the window. The window's event loop drives the frames at the view's
requested rate — you do not write a loop yourself.

```c
static void draw(SLUIDrawContext *dc, double time_s, double dt_s, void *user) {
    slui_draw_clear(dc, slui_rgb(0x0d,0x0d,0x0f));
    double x = (0.5 + 0.5*sin(time_s)) * slui_draw_width(dc);
    slui_draw_fill_circle(dc, x, slui_draw_height(dc)/2.0, 10,
                          slui_color_hsv(time_s*60, 0.8, 1.0, 1.0));
}

SLUIWidget *view = slui_canvas_view(root, 400, 80);
slui_canvas_view_set_fps(view, 60.0);
slui_canvas_view_set_draw(view, draw, NULL);
```

## The Throbber

The **Throbber** is a Canvas View whose built-in draw function is a **water-like
flow field** — see [THROBBER.md](THROBBER.md) for the model. It is
width-adjustable, full-motion, and colour-predictive (the hue leads the flow),
tuned to approach the ease of real flowing water without exceeding it.

```c
SLUIWidget *t = slui_throbber(root, 400);     /* initial width in px          */
slui_widget_set_size_request(t, 0, 24);        /* height (a slim seam default) */
slui_throbber_set_width(t, 520);               /* adjustable any time          */
slui_throbber_set_intensity(t, 0.7);           /* 0 calm .. 1 vigorous         */
slui_throbber_set_hue(t, 205);                 /* base hue (deg); 205 ~ water  */
```

## SLeeLa bindings

Every call here has a one-class-per-file SLeeLa binding under
[`/lib/user-interface`](../lib/user-interface/USER-INTERFACE.md): `SLDrawContext`
(the full primitive/pixel/buffer surface), `SLFrameClock` (refresh pacing),
`SLCanvasView` (the hosted draw surface), and `SLThrobber` (the water throbber).

— SleelaUI™ · MEARVK LLC · 2026
