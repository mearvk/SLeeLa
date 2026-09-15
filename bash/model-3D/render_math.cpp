// Phraign(TM) City 3D -- rendering math implementation.

#include "render_math.hpp"

#include <algorithm>
#include <cmath>

namespace sleela::render {

// ---------------------------------------------------------------------------
// Vectors
// ---------------------------------------------------------------------------

double dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
    return Vec3{a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x};
}

double length(const Vec3& v) noexcept {
    return std::sqrt(dot(v, v));
}

Vec3 normalize(const Vec3& v) noexcept {
    const double len = length(v);
    if (len <= 0.0) return Vec3{};
    const double inv = 1.0 / len;
    return Vec3{v.x * inv, v.y * inv, v.z * inv};
}

double signedArea2(const Vec2& a, const Vec2& b, const Vec2& c) noexcept {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

double clampd(double v, double lo, double hi) noexcept {
    return v < lo ? lo : (v > hi ? hi : v);
}

double lerpd(double a, double b, double t) noexcept {
    return a + (b - a) * t;
}

// ---------------------------------------------------------------------------
// Color
// ---------------------------------------------------------------------------

namespace {
std::uint8_t toByte(double v) noexcept {
    return static_cast<std::uint8_t>(clampd(v, 0.0, 255.0) + 0.5);
}
}

Rgba shade(const Rgba& c, double factor) noexcept {
    return Rgba{toByte(c.r * factor),
                toByte(c.g * factor),
                toByte(c.b * factor),
                c.a};
}

Rgba lerp(const Rgba& a, const Rgba& b, double t) noexcept {
    return Rgba{toByte(lerpd(a.r, b.r, t)),
                toByte(lerpd(a.g, b.g, t)),
                toByte(lerpd(a.b, b.b, t)),
                toByte(lerpd(a.a, b.a, t))};
}

// ---------------------------------------------------------------------------
// Projection
// ---------------------------------------------------------------------------

Vec2 ObliqueCamera::project(const Vec3& world) const noexcept {
    // x east -> +screen x; y depth -> skews x and advances y; z up -> lifts y.
    Vec2 s;
    s.x = origin_x + world.x * scale_x + world.y * tilt_x;
    s.y = origin_y + world.y * tilt_y - world.z * scale_height;
    return s;
}

// ---------------------------------------------------------------------------
// Screen geometry
// ---------------------------------------------------------------------------

void BBox::add(const Vec2& p) noexcept {
    if (empty) {
        min_x = max_x = p.x;
        min_y = max_y = p.y;
        empty = false;
    } else {
        min_x = std::min(min_x, p.x);
        min_y = std::min(min_y, p.y);
        max_x = std::max(max_x, p.x);
        max_y = std::max(max_y, p.y);
    }
}

BBox Quad::bounds() const noexcept {
    BBox b;
    for (const auto& c : p) b.add(c);
    return b;
}

Vec2 Quad::centroid() const noexcept {
    return Vec2{(p[0].x + p[1].x + p[2].x + p[3].x) * 0.25,
                (p[0].y + p[1].y + p[2].y + p[3].y) * 0.25};
}

// ---------------------------------------------------------------------------
// Scanline fill
// ---------------------------------------------------------------------------

std::size_t fillQuad(RenderSink& sink, const Quad& quad, std::uint32_t rgba) {
    const BBox b = quad.bounds();
    if (b.empty) return 0;

    const int W = static_cast<int>(sink.width());
    const int H = static_cast<int>(sink.height());
    if (W <= 0 || H <= 0) return 0;

    int y0 = static_cast<int>(std::floor(b.min_y));
    int y1 = static_cast<int>(std::ceil(b.max_y));
    y0 = std::max(y0, 0);
    y1 = std::min(y1, H - 1);

    std::size_t drawn = 0;
    for (int y = y0; y <= y1; ++y) {
        const double sy = y + 0.5;
        double xs[8];
        int n = 0;
        for (int i = 0; i < 4; ++i) {
            const Vec2& a = quad.p[i];
            const Vec2& c = quad.p[(i + 1) % 4];
            const double ay = a.y, cy = c.y;
            // Edge crosses this scanline (half-open to avoid double counting).
            if ((sy >= ay && sy < cy) || (sy >= cy && sy < ay)) {
                const double t = (sy - ay) / (cy - ay);
                xs[n++] = a.x + t * (c.x - a.x);
            }
        }
        if (n < 2) continue;
        std::sort(xs, xs + n);
        for (int i = 0; i + 1 < n; i += 2) {
            int xa = static_cast<int>(std::floor(xs[i] + 0.5));
            int xb = static_cast<int>(std::floor(xs[i + 1] - 0.5));
            xa = std::max(xa, 0);
            xb = std::min(xb, W - 1);
            for (int x = xa; x <= xb; ++x) {
                if (sink.plot(static_cast<std::size_t>(x),
                              static_cast<std::size_t>(y), rgba)) {
                    ++drawn;
                }
            }
        }
    }
    return drawn;
}

} // namespace sleela::render
