#ifndef SLUI_RENDER_HPP
#define SLUI_RENDER_HPP
/* =============================================================================
 * SleelaUI software rasterizer.
 *
 * The toolkit owns its pixels. Every window keeps one Canvas -- a 32-bit
 * premultiplied-into-opaque framebuffer -- that widgets paint into, and the
 * platform backend blits that buffer to the screen. Drawing identical pixels on
 * every OS is what makes the "Slick Black" look pixel-identical on Windows,
 * macOS, and Linux/Unix instead of inheriting three native widget themes.
 *
 * The rasterizer is deliberately small and original: scanline fills with
 * analytic edge antialiasing for rounded rectangles, Wu-style AA for hairlines,
 * vertical gradients, and an 8-bit coverage glyph blitter. Text SHAPING and
 * glyph COVERAGE come from the backend (it owns the real font files); this
 * module only composites the coverage the backend hands back, so the geometry
 * and colour of the UI stay backend-independent.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "slui_geometry.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

namespace slui {

/* A single-channel coverage bitmap for one laid-out glyph, in device pixels.
 * The backend's font rasterizer fills `coverage` (8-bit alpha, row-major) and
 * reports the pen advance so the Canvas can place the next glyph. */
struct GlyphBitmap {
    int left = 0;    /* bearing from pen origin to bitmap left edge   */
    int top = 0;     /* bearing from baseline up to bitmap top edge   */
    int width = 0;
    int height = 0;
    double advance = 0.0; /* horizontal pen advance in pixels         */
    std::vector<uint8_t> coverage;
};

class Canvas {
public:
    Canvas() = default;

    void resize(int w, int h) {
        if (w < 1) w = 1;
        if (h < 1) h = 1;
        if (w == width_ && h == height_) return;
        width_ = w;
        height_ = h;
        pixels_.assign(static_cast<size_t>(w) * h, 0);
        clip_ = Rect{0, 0, w, h};
    }

    int width() const { return width_; }
    int height() const { return height_; }
    const uint32_t* pixels() const { return pixels_.data(); }
    uint32_t* pixels() { return pixels_.data(); }

    /* Stored as 0xAARRGGBB so Win32 (BGRA little-endian), Cocoa, and X11
     * (which we feed ZPixmap BGRA) all read the same word order after the
     * backend's final channel adjust. */
    static uint32_t pack(const Color& c) {
        return (static_cast<uint32_t>(c.a) << 24) |
               (static_cast<uint32_t>(c.r) << 16) |
               (static_cast<uint32_t>(c.g) << 8) | static_cast<uint32_t>(c.b);
    }
    static Color unpack(uint32_t p) {
        return Color{static_cast<uint8_t>((p >> 16) & 0xFF),
                     static_cast<uint8_t>((p >> 8) & 0xFF),
                     static_cast<uint8_t>(p & 0xFF),
                     static_cast<uint8_t>((p >> 24) & 0xFF)};
    }

    void push_clip(const Rect& r) {
        clip_stack_.push_back(clip_);
        clip_ = clip_.intersect(r);
    }
    void pop_clip() {
        if (!clip_stack_.empty()) {
            clip_ = clip_stack_.back();
            clip_stack_.pop_back();
        }
    }

    void clear(const Color& c) {
        uint32_t p = pack(c.with_alpha(255));
        std::fill(pixels_.begin(), pixels_.end(), p);
    }

    /* Blend one pixel with coverage [0..1] applied to the source alpha. */
    void blend(int x, int y, const Color& src, double coverage) {
        if (coverage <= 0.0) return;
        if (!clip_.contains(x, y)) return;
        Color s = src;
        s.a = static_cast<uint8_t>(std::clamp(src.a * coverage, 0.0, 255.0) + 0.5);
        if (s.a == 0) return;
        size_t idx = static_cast<size_t>(y) * width_ + x;
        Color dst = unpack(pixels_[idx]);
        Color out = over(s, dst);
        out.a = 255; /* window buffer is opaque */
        pixels_[idx] = pack(out);
    }

    void fill_rect(const Rect& r, const Color& c) {
        Rect a = r.intersect(clip_);
        for (int y = a.y; y < a.bottom(); ++y)
            for (int x = a.x; x < a.right(); ++x)
                blend(x, y, c, 1.0);
    }

    /* Antialiased rounded-rectangle fill. Coverage comes from a signed-distance
     * test against the rounded boundary, sampled at pixel centres. radius is
     * clamped to half the smaller side. */
    void fill_round_rect(const Rect& r, double radius, const Color& c) {
        if (r.empty()) return;
        radius = std::clamp(radius, 0.0, std::min(r.w, r.h) / 2.0);
        Rect a = r.intersect(clip_);
        double rl = r.x + radius, rt = r.y + radius;
        double rr = r.right() - radius, rb = r.bottom() - radius;
        for (int y = a.y; y < a.bottom(); ++y) {
            double py = y + 0.5;
            for (int x = a.x; x < a.right(); ++x) {
                double px = x + 0.5;
                double cov = round_rect_coverage(px, py, r, radius, rl, rt, rr, rb);
                blend(x, y, c, cov);
            }
        }
    }

    /* A 1px (or `thickness`px) hairline border following the rounded outline. */
    void stroke_round_rect(const Rect& r, double radius, double thickness,
                           const Color& c) {
        if (r.empty() || thickness <= 0.0) return;
        radius = std::clamp(radius, 0.0, std::min(r.w, r.h) / 2.0);
        Rect a = r.intersect(clip_);
        double rl = r.x + radius, rt = r.y + radius;
        double rr = r.right() - radius, rb = r.bottom() - radius;
        for (int y = a.y; y < a.bottom(); ++y) {
            double py = y + 0.5;
            for (int x = a.x; x < a.right(); ++x) {
                double px = x + 0.5;
                double d = round_rect_distance(px, py, r, radius, rl, rt, rr, rb);
                /* d is signed: <0 inside, >0 outside. The stroke is a band
                 * centred on the boundary. */
                double half = thickness / 2.0;
                double cov = 1.0 - smooth_edge(std::fabs(d) - half);
                blend(x, y, c, cov);
            }
        }
    }

    /* Vertical top->bottom gradient fill of a rounded rectangle. */
    void fill_round_rect_vgrad(const Rect& r, double radius, const Color& top,
                               const Color& bottom) {
        if (r.empty()) return;
        radius = std::clamp(radius, 0.0, std::min(r.w, r.h) / 2.0);
        Rect a = r.intersect(clip_);
        double rl = r.x + radius, rt = r.y + radius;
        double rr = r.right() - radius, rb = r.bottom() - radius;
        for (int y = a.y; y < a.bottom(); ++y) {
            double py = y + 0.5;
            double t = r.h > 1 ? (py - r.y) / static_cast<double>(r.h) : 0.0;
            Color row = lerp(top, bottom, t);
            for (int x = a.x; x < a.right(); ++x) {
                double px = x + 0.5;
                double cov = round_rect_coverage(px, py, r, radius, rl, rt, rr, rb);
                blend(x, y, row, cov);
            }
        }
    }

    /* A horizontal or vertical 1px hairline (used for separators). */
    void hline(int x0, int x1, int y, const Color& c) {
        if (x1 < x0) std::swap(x0, x1);
        for (int x = x0; x <= x1; ++x) blend(x, y, c, 1.0);
    }
    void vline(int x, int y0, int y1, const Color& c) {
        if (y1 < y0) std::swap(y0, y1);
        for (int y = y0; y <= y1; ++y) blend(x, y, c, 1.0);
    }

    /* Composite a glyph coverage bitmap at device position (pen_x, baseline). */
    void blit_glyph(const GlyphBitmap& g, int pen_x, int baseline,
                    const Color& c) {
        int gx = pen_x + g.left;
        int gy = baseline - g.top;
        for (int row = 0; row < g.height; ++row) {
            int y = gy + row;
            for (int col = 0; col < g.width; ++col) {
                int x = gx + col;
                uint8_t a = g.coverage[static_cast<size_t>(row) * g.width + col];
                if (a) blend(x, y, c, a / 255.0);
            }
        }
    }

private:
    /* Smoothstep edge over a 1px band: 0 well inside, 1 well outside. */
    static double smooth_edge(double d) {
        if (d <= -0.5) return 0.0;
        if (d >= 0.5) return 1.0;
        double t = d + 0.5; /* 0..1 */
        return t * t * (3.0 - 2.0 * t);
    }

    /* Signed distance from (px,py) to the rounded-rect boundary. */
    static double round_rect_distance(double px, double py, const Rect& r,
                                      double radius, double rl, double rt,
                                      double rr, double rb) {
        if (radius <= 0.0) {
            double dx = std::max(r.x - px, px - r.right());
            double dy = std::max(r.y - py, py - r.bottom());
            return std::max(dx, dy);
        }
        double cx = std::clamp(px, rl, rr);
        double cy = std::clamp(py, rt, rb);
        double dx = px - cx, dy = py - cy;
        return std::sqrt(dx * dx + dy * dy) - radius;
    }

    static double round_rect_coverage(double px, double py, const Rect& r,
                                      double radius, double rl, double rt,
                                      double rr, double rb) {
        double d = round_rect_distance(px, py, r, radius, rl, rt, rr, rb);
        return 1.0 - smooth_edge(d);
    }

    int width_ = 1;
    int height_ = 1;
    std::vector<uint32_t> pixels_{0};
    Rect clip_{0, 0, 1, 1};
    std::vector<Rect> clip_stack_;
};

} // namespace slui

#endif /* SLUI_RENDER_HPP */
