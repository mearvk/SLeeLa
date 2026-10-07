#ifndef SLUI_GEOMETRY_HPP
#define SLUI_GEOMETRY_HPP
/* Internal C++ geometry and colour helpers for the SleelaUI toolkit.
 * These sit behind the public C ABI in include/sleela_ui.h and are never part
 * of the installed interface. Max Rupplin -- MEARVK LLC -- 2026. */
#include "sleela_ui.h"

#include <algorithm>
#include <cstdint>

namespace slui {

struct Rect {
    int x = 0, y = 0, w = 0, h = 0;

    int right() const { return x + w; }
    int bottom() const { return y + h; }
    bool empty() const { return w <= 0 || h <= 0; }

    bool contains(int px, int py) const {
        return px >= x && py >= y && px < x + w && py < y + h;
    }

    Rect inset(int top, int right_, int bottom, int left) const {
        return Rect{x + left, y + top, w - left - right_, h - top - bottom};
    }
    Rect inset(int all) const { return inset(all, all, all, all); }

    Rect intersect(const Rect& o) const {
        int nx = std::max(x, o.x);
        int ny = std::max(y, o.y);
        int nr = std::min(right(), o.right());
        int nb = std::min(bottom(), o.bottom());
        return Rect{nx, ny, std::max(0, nr - nx), std::max(0, nb - ny)};
    }
};

inline Rect to_rect(const SLUIRect& r) { return Rect{r.x, r.y, r.w, r.h}; }

/* Straight-alpha colour decoded from the public 0xRRGGBBAA word. */
struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    static Color from_abi(SLUIColor c) {
        return Color{static_cast<uint8_t>((c >> 24) & 0xFF),
                     static_cast<uint8_t>((c >> 16) & 0xFF),
                     static_cast<uint8_t>((c >> 8) & 0xFF),
                     static_cast<uint8_t>(c & 0xFF)};
    }
    SLUIColor to_abi() const { return slui_rgba(r, g, b, a); }

    Color with_alpha(uint8_t na) const { return Color{r, g, b, na}; }
};

/* Linear interpolation of two colours in straight 8-bit space, t in [0,1].
 * Good enough for hairline gradients and hover/active tints; the toolkit's
 * surfaces are near-black so perceptual error is negligible. */
inline Color lerp(const Color& a, const Color& b, double t) {
    t = std::clamp(t, 0.0, 1.0);
    auto mix = [t](uint8_t x, uint8_t y) {
        return static_cast<uint8_t>(x + (y - x) * t + 0.5);
    };
    return Color{mix(a.r, b.r), mix(a.g, b.g), mix(a.b, b.b), mix(a.a, b.a)};
}

/* Composite `src` over `dst` (both straight alpha). Returns the opaque-ish
 * result used by the software rasterizer, which writes into a 32-bit buffer. */
inline Color over(const Color& src, const Color& dst) {
    double sa = src.a / 255.0;
    double da = dst.a / 255.0;
    double oa = sa + da * (1.0 - sa);
    if (oa <= 0.0) return Color{0, 0, 0, 0};
    auto chan = [&](uint8_t s, uint8_t d) {
        double v = (s * sa + d * da * (1.0 - sa)) / oa;
        return static_cast<uint8_t>(std::clamp(v, 0.0, 255.0) + 0.5);
    };
    return Color{chan(src.r, dst.r), chan(src.g, dst.g), chan(src.b, dst.b),
                 static_cast<uint8_t>(std::clamp(oa * 255.0, 0.0, 255.0) + 0.5)};
}

} // namespace slui

#endif /* SLUI_GEOMETRY_HPP */
