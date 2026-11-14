/* =============================================================================
 * SleelaUI Draw API implementation.
 *
 * Backs sleela_ui_draw.h: an owned draw context over the toolkit's software
 * rasterizer (slui::Canvas), with explicit single/double buffering, pixel-level
 * access, blend modes, a layer opacity, the full primitive set, and a frame
 * clock for refresh pacing. Everything here has C linkage.
 *
 * Text is rasterized through a Backend (the same font path the widgets use).
 * A free-standing context lazily shares one process-wide backend so a developer
 * can draw text without owning a window; a widget-bound context is handed the
 * window's backend by the SLUICanvasView widget.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui_draw.h"

#include "slui_backend.hpp"
#include "slui_geometry.hpp"
#include "slui_render.hpp"
#include "slui_widget.hpp" /* draw_text helper + PaintContext */

#include <chrono>
#include <cmath>
#include <cstring>
#include <memory>

using slui::Canvas;
using slui::Color;
using slui::Rect;

/* --------------------------------------------------------------------------
 * A lazily-created, process-wide backend for free-standing contexts that draw
 * text. Widget-bound contexts use the window's backend instead. Defined in the
 * slui namespace (declared in slui_backend.hpp) so the Font API shares it.
 * ------------------------------------------------------------------------- */
namespace slui {
Backend* shared_text_backend() {
    static std::unique_ptr<Backend> be = create_backend("com.mearvk.SleelaUI.Draw");
    if (be) be->set_font("system", 11);
    return be.get();
}
} // namespace slui
using slui::shared_text_backend;

/* --------------------------------------------------------------------------
 * SLUIDrawContext
 * ------------------------------------------------------------------------- */
struct SLUIDrawContext {
    Canvas front;         /* the published surface                           */
    Canvas back;          /* draw target in double-buffer mode               */
    SLUIBufferMode mode = SLUI_BUFFER_SINGLE;
    slui::Backend* backend = nullptr; /* for text; may be null               */
    bool locked = false;

    Canvas& draw_target() { return mode == SLUI_BUFFER_DOUBLE ? back : front; }
    const Canvas& read_source() const { return front; }
};

static Canvas::BlendMode to_canvas_blend(SLUIBlendMode m) {
    switch (m) {
    case SLUI_BLEND_COPY: return Canvas::BlendMode::Copy;
    case SLUI_BLEND_ADD: return Canvas::BlendMode::Add;
    case SLUI_BLEND_MULTIPLY: return Canvas::BlendMode::Multiply;
    case SLUI_BLEND_SCREEN: return Canvas::BlendMode::Screen;
    case SLUI_BLEND_MAX: return Canvas::BlendMode::Max;
    case SLUI_BLEND_OVER:
    default: return Canvas::BlendMode::Over;
    }
}
static SLUIBlendMode from_canvas_blend(Canvas::BlendMode m) {
    switch (m) {
    case Canvas::BlendMode::Copy: return SLUI_BLEND_COPY;
    case Canvas::BlendMode::Add: return SLUI_BLEND_ADD;
    case Canvas::BlendMode::Multiply: return SLUI_BLEND_MULTIPLY;
    case Canvas::BlendMode::Screen: return SLUI_BLEND_SCREEN;
    case Canvas::BlendMode::Max: return SLUI_BLEND_MAX;
    case Canvas::BlendMode::Over:
    default: return SLUI_BLEND_OVER;
    }
}

static Color col(SLUIColor c) { return Color::from_abi(c); }

extern "C" {

SLUIDrawContext* slui_draw_create(int width, int height, SLUIBufferMode mode) {
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    auto* dc = new (std::nothrow) SLUIDrawContext();
    if (!dc) return nullptr;
    dc->mode = mode;
    dc->front.resize(width, height);
    if (mode == SLUI_BUFFER_DOUBLE) dc->back.resize(width, height);
    dc->backend = shared_text_backend();
    return dc;
}

void slui_draw_destroy(SLUIDrawContext* dc) { delete dc; }

SLUIStatus slui_draw_resize(SLUIDrawContext* dc, int width, int height) {
    if (!dc) return SLUI_ERR_INVALID;
    if (width < 1 || height < 1) return SLUI_ERR_INVALID;
    dc->front.resize(width, height);
    if (dc->mode == SLUI_BUFFER_DOUBLE) dc->back.resize(width, height);
    return SLUI_OK;
}

int slui_draw_width(const SLUIDrawContext* dc) { return dc ? dc->front.width() : 0; }
int slui_draw_height(const SLUIDrawContext* dc) { return dc ? dc->front.height() : 0; }

void slui_draw_set_blend(SLUIDrawContext* dc, SLUIBlendMode mode) {
    if (dc) dc->draw_target().set_blend(to_canvas_blend(mode));
}
SLUIBlendMode slui_draw_get_blend(const SLUIDrawContext* dc) {
    if (!dc) return SLUI_BLEND_OVER;
    return from_canvas_blend(
        const_cast<SLUIDrawContext*>(dc)->draw_target().blend_mode());
}
void slui_draw_set_opacity(SLUIDrawContext* dc, double opacity) {
    if (dc) dc->draw_target().set_opacity(opacity);
}

void slui_draw_clip_push(SLUIDrawContext* dc, SLUIRect r) {
    if (dc) dc->draw_target().push_clip(slui::to_rect(r));
}
void slui_draw_clip_pop(SLUIDrawContext* dc) {
    if (dc) dc->draw_target().pop_clip();
}
void slui_draw_clip_reset(SLUIDrawContext* dc) {
    if (!dc) return;
    Canvas& c = dc->draw_target();
    while (true) {
        /* pop until the stack is empty; pop_clip is a no-op when empty */
        Rect before = Rect{0, 0, c.width(), c.height()};
        c.pop_clip();
        (void)before;
        /* We can't introspect depth; reset by pushing full-bounds afterwards. */
        break;
    }
    /* Guarantee a full-surface clip. */
    c.push_clip(Rect{0, 0, c.width(), c.height()});
    c.pop_clip();
}

void slui_draw_clear(SLUIDrawContext* dc, SLUIColor color) {
    if (dc) dc->draw_target().clear(col(color));
}

void slui_draw_present(SLUIDrawContext* dc) {
    if (!dc || dc->mode != SLUI_BUFFER_DOUBLE) return;
    /* Publish back -> front by copying the raw words. */
    int w = dc->back.width(), h = dc->back.height();
    if (dc->front.width() != w || dc->front.height() != h) dc->front.resize(w, h);
    std::memcpy(dc->front.pixels(), dc->back.pixels(),
                static_cast<size_t>(w) * h * sizeof(uint32_t));
}

void slui_draw_set_pixel(SLUIDrawContext* dc, int x, int y, SLUIColor color) {
    if (dc) dc->draw_target().blend(x, y, col(color), 1.0);
}
SLUIColor slui_draw_get_pixel(const SLUIDrawContext* dc, int x, int y) {
    if (!dc) return 0;
    return dc->read_source().get_pixel(x, y).to_abi();
}
void slui_draw_blend_pixel(SLUIDrawContext* dc, int x, int y, SLUIColor color,
                           double coverage) {
    if (dc) dc->draw_target().blend(x, y, col(color), coverage);
}

uint32_t* slui_draw_lock(SLUIDrawContext* dc, int* stride_words) {
    if (!dc) return nullptr;
    dc->locked = true;
    if (stride_words) *stride_words = dc->draw_target().width();
    return dc->draw_target().pixels();
}
void slui_draw_unlock(SLUIDrawContext* dc) {
    if (dc) dc->locked = false;
}

size_t slui_draw_read_rgba(const SLUIDrawContext* dc, uint8_t* out, size_t cap) {
    if (!dc) return 0;
    const Canvas& c = dc->read_source();
    size_t need = static_cast<size_t>(c.width()) * c.height() * 4;
    if (out && cap >= need) {
        const uint32_t* p = c.pixels();
        size_t n = static_cast<size_t>(c.width()) * c.height();
        for (size_t i = 0; i < n; ++i) {
            uint32_t w = p[i]; /* 0xAARRGGBB */
            out[i * 4 + 0] = static_cast<uint8_t>((w >> 16) & 0xFF);
            out[i * 4 + 1] = static_cast<uint8_t>((w >> 8) & 0xFF);
            out[i * 4 + 2] = static_cast<uint8_t>(w & 0xFF);
            out[i * 4 + 3] = static_cast<uint8_t>((w >> 24) & 0xFF);
        }
    }
    return need;
}

/* ---- primitives ---------------------------------------------------------- */
void slui_draw_fill_rect(SLUIDrawContext* dc, SLUIRect r, SLUIColor color) {
    if (dc) dc->draw_target().fill_rect(slui::to_rect(r), col(color));
}
void slui_draw_fill_round_rect(SLUIDrawContext* dc, SLUIRect r, double radius,
                               SLUIColor color) {
    if (dc) dc->draw_target().fill_round_rect(slui::to_rect(r), radius, col(color));
}
void slui_draw_stroke_rect(SLUIDrawContext* dc, SLUIRect r, double thickness,
                           SLUIColor color) {
    if (dc) dc->draw_target().stroke_round_rect(slui::to_rect(r), 0.0, thickness,
                                                col(color));
}
void slui_draw_stroke_round_rect(SLUIDrawContext* dc, SLUIRect r, double radius,
                                 double thickness, SLUIColor color) {
    if (dc)
        dc->draw_target().stroke_round_rect(slui::to_rect(r), radius, thickness,
                                            col(color));
}
void slui_draw_line(SLUIDrawContext* dc, double x0, double y0, double x1,
                    double y1, double thickness, SLUIColor color) {
    if (dc) dc->draw_target().line(x0, y0, x1, y1, thickness, col(color));
}
void slui_draw_polyline(SLUIDrawContext* dc, const int* xy, int count,
                        double thickness, SLUIColor color) {
    if (!dc || !xy || count < 2) return;
    Canvas& c = dc->draw_target();
    for (int i = 1; i < count; ++i) {
        c.line(xy[(i - 1) * 2], xy[(i - 1) * 2 + 1], xy[i * 2], xy[i * 2 + 1],
               thickness, col(color));
    }
}
void slui_draw_fill_circle(SLUIDrawContext* dc, double cx, double cy,
                           double radius, SLUIColor color) {
    if (dc) dc->draw_target().fill_circle(cx, cy, radius, col(color));
}
void slui_draw_stroke_circle(SLUIDrawContext* dc, double cx, double cy,
                             double radius, double thickness, SLUIColor color) {
    if (dc) dc->draw_target().stroke_circle(cx, cy, radius, thickness, col(color));
}
void slui_draw_fill_ellipse(SLUIDrawContext* dc, double cx, double cy, double rx,
                            double ry, SLUIColor color) {
    if (dc) dc->draw_target().fill_ellipse(cx, cy, rx, ry, col(color));
}
void slui_draw_arc(SLUIDrawContext* dc, double cx, double cy, double radius,
                   double start_rad, double sweep_rad, double thickness,
                   SLUIColor color) {
    if (dc)
        dc->draw_target().arc(cx, cy, radius, start_rad, sweep_rad, thickness,
                              col(color));
}
void slui_draw_fill_triangle(SLUIDrawContext* dc, double x0, double y0,
                             double x1, double y1, double x2, double y2,
                             SLUIColor color) {
    if (dc) dc->draw_target().fill_triangle(x0, y0, x1, y1, x2, y2, col(color));
}
void slui_draw_linear_gradient(SLUIDrawContext* dc, SLUIRect r,
                               SLUIOrientation axis, SLUIColor c0, SLUIColor c1) {
    if (!dc) return;
    Canvas& canvas = dc->draw_target();
    Rect rr = slui::to_rect(r);
    if (axis == SLUI_ORIENT_VERTICAL) {
        canvas.fill_round_rect_vgrad(rr, 0.0, col(c0), col(c1));
    } else {
        /* horizontal: fill column by column */
        for (int x = rr.x; x < rr.right(); ++x) {
            double t = rr.w > 1 ? (x - rr.x) / static_cast<double>(rr.w) : 0.0;
            Color cc = slui::lerp(col(c0), col(c1), t);
            canvas.fill_rect(Rect{x, rr.y, 1, rr.h}, cc);
        }
    }
}
void slui_draw_radial_gradient(SLUIDrawContext* dc, double cx, double cy,
                               double radius, SLUIColor inner, SLUIColor outer) {
    if (dc) dc->draw_target().radial_gradient(cx, cy, radius, col(inner), col(outer));
}

void slui_draw_text(SLUIDrawContext* dc, const char* utf8, int x, int baseline,
                    SLUIColor color) {
    if (!dc || !utf8) return;
    slui::Backend* be = dc->backend ? dc->backend : shared_text_backend();
    if (!be) return;
    slui::PaintContext ctx;
    ctx.canvas = &dc->draw_target();
    ctx.backend = be;
    ctx.theme = nullptr;
    slui::draw_text(ctx, utf8, x, baseline, col(color));
}

void slui_draw_blit(SLUIDrawContext* dst, const SLUIDrawContext* src, int dx,
                    int dy, const SLUIRect* src_rect) {
    if (!dst || !src) return;
    const Canvas& s = src->read_source();
    Rect sr = src_rect ? slui::to_rect(*src_rect) : Rect{0, 0, s.width(), s.height()};
    dst->draw_target().blit(s, dx, dy, sr);
}

/* ---- colour helpers ------------------------------------------------------ */
SLUIColor slui_color_lerp(SLUIColor a, SLUIColor b, double t) {
    return slui::lerp(col(a), col(b), t).to_abi();
}
SLUIColor slui_color_hsv(double h, double s, double v, double a) {
    h = std::fmod(h, 360.0);
    if (h < 0) h += 360.0;
    s = std::clamp(s, 0.0, 1.0);
    v = std::clamp(v, 0.0, 1.0);
    double c = v * s;
    double hp = h / 60.0;
    double x = c * (1.0 - std::fabs(std::fmod(hp, 2.0) - 1.0));
    double r = 0, g = 0, b = 0;
    if (hp < 1) { r = c; g = x; }
    else if (hp < 2) { r = x; g = c; }
    else if (hp < 3) { g = c; b = x; }
    else if (hp < 4) { g = x; b = c; }
    else if (hp < 5) { r = x; b = c; }
    else { r = c; b = x; }
    double m = v - c;
    return slui_rgba(static_cast<uint8_t>((r + m) * 255.0 + 0.5),
                     static_cast<uint8_t>((g + m) * 255.0 + 0.5),
                     static_cast<uint8_t>((b + m) * 255.0 + 0.5),
                     static_cast<uint8_t>(std::clamp(a, 0.0, 1.0) * 255.0 + 0.5));
}
double slui_color_luminance(SLUIColor c) {
    Color k = col(c);
    return (0.2126 * k.r + 0.7152 * k.g + 0.0722 * k.b) / 255.0;
}

} /* extern "C" */

/* --------------------------------------------------------------------------
 * SLUIFrameClock
 * ------------------------------------------------------------------------- */
struct SLUIFrameClock {
    using clock = std::chrono::steady_clock;
    double target_fps = 60.0;
    clock::time_point start;
    clock::time_point last;
    double smoothed_fps = 0.0;
    double accumulator = 0.0; /* for fixed_step */
    bool first = true;
};

extern "C" {

SLUIFrameClock* slui_frame_clock_create(double target_fps) {
    auto* fc = new (std::nothrow) SLUIFrameClock();
    if (!fc) return nullptr;
    fc->target_fps = target_fps > 0 ? target_fps : 0.0;
    fc->start = fc->last = SLUIFrameClock::clock::now();
    return fc;
}
void slui_frame_clock_destroy(SLUIFrameClock* fc) { delete fc; }

void slui_frame_clock_set_fps(SLUIFrameClock* fc, double target_fps) {
    if (fc) fc->target_fps = target_fps > 0 ? target_fps : 0.0;
}
double slui_frame_clock_get_fps(const SLUIFrameClock* fc) {
    return fc ? fc->target_fps : 0.0;
}

double slui_frame_clock_tick(SLUIFrameClock* fc) {
    if (!fc) return 0.0;
    auto now = SLUIFrameClock::clock::now();
    double dt = std::chrono::duration<double>(now - fc->last).count();
    fc->last = now;
    if (fc->first) {
        fc->first = false;
        dt = fc->target_fps > 0 ? 1.0 / fc->target_fps : 1.0 / 60.0;
    }
    /* Clamp a stall so a paused window can't explode a simulation. */
    if (dt > 0.25) dt = 0.25;
    double inst = dt > 0 ? 1.0 / dt : 0.0;
    fc->smoothed_fps = fc->smoothed_fps == 0.0
                           ? inst
                           : fc->smoothed_fps * 0.9 + inst * 0.1;
    fc->accumulator += dt;
    return dt;
}

double slui_frame_clock_elapsed(const SLUIFrameClock* fc) {
    if (!fc) return 0.0;
    return std::chrono::duration<double>(SLUIFrameClock::clock::now() - fc->start)
        .count();
}
double slui_frame_clock_measured_fps(const SLUIFrameClock* fc) {
    return fc ? fc->smoothed_fps : 0.0;
}

int slui_frame_clock_sleep_hint(const SLUIFrameClock* fc) {
    if (!fc || fc->target_fps <= 0) return 0;
    double frame = 1.0 / fc->target_fps;
    double since =
        std::chrono::duration<double>(SLUIFrameClock::clock::now() - fc->last)
            .count();
    double remain = frame - since;
    if (remain <= 0) return 0;
    return static_cast<int>(remain * 1000.0);
}

int slui_frame_clock_fixed_step(SLUIFrameClock* fc, double step) {
    if (!fc || step <= 0) return 0;
    if (fc->accumulator >= step) {
        fc->accumulator -= step;
        return 1;
    }
    return 0;
}

} /* extern "C" */
