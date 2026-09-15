#pragma once

// SleelaTerminal(TM) -- SLeeLa's terminal product, built on GNU Bash
// (GPL, (C) Free Software Foundation). SLeeLa-authored addition; the
// vendored GNU Bash sources keep their own GPL headers and copyright.
// Phraign(TM) City 3D -- rendering math.
//
// Basic, dependency-free rendering primitives shared by the renderer and the
// rendering group: 2D/3D vectors, RGBA color math, the oblique world->screen
// projection, an axis-aligned bounding box, a screen-space quad, and a
// scanline polygon fill that writes into any RenderSink.
//
// The math here is intentionally small and exact for the convex quads the city
// renderer produces (roofs and walls), and keeps rendering "per pixel" onto a
// Phraign frame.

#include <array>
#include <cstddef>
#include <cstdint>

namespace sleela::render {

// ---------------------------------------------------------------------------
// Vectors
// ---------------------------------------------------------------------------

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    Vec2 operator+(const Vec2& o) const noexcept { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const noexcept { return {x - o.x, y - o.y}; }
    Vec2 operator*(double s) const noexcept { return {x * s, y * s}; }
};

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vec3 operator+(const Vec3& o) const noexcept {
        return {x + o.x, y + o.y, z + o.z};
    }
    Vec3 operator-(const Vec3& o) const noexcept {
        return {x - o.x, y - o.y, z - o.z};
    }
    Vec3 operator*(double s) const noexcept { return {x * s, y * s, z * s}; }
};

double dot(const Vec3& a, const Vec3& b) noexcept;
Vec3 cross(const Vec3& a, const Vec3& b) noexcept;
double length(const Vec3& v) noexcept;
Vec3 normalize(const Vec3& v) noexcept;  // returns {0,0,0} for a zero vector

// Signed area * 2 of the 2D triangle (a,b,c). Positive => counter-clockwise.
double signedArea2(const Vec2& a, const Vec2& b, const Vec2& c) noexcept;

double clampd(double v, double lo, double hi) noexcept;
double lerpd(double a, double b, double t) noexcept;

// ---------------------------------------------------------------------------
// Color (RGBA, packed 0xRRGGBBAA to match the Phraign pixel encoding)
// ---------------------------------------------------------------------------

struct Rgba {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0xFF;

    constexpr std::uint32_t pack() const noexcept {
        return (static_cast<std::uint32_t>(r) << 24) |
               (static_cast<std::uint32_t>(g) << 16) |
               (static_cast<std::uint32_t>(b) << 8) |
               static_cast<std::uint32_t>(a);
    }
    static constexpr Rgba unpack(std::uint32_t v) noexcept {
        return Rgba{static_cast<std::uint8_t>((v >> 24) & 0xFF),
                    static_cast<std::uint8_t>((v >> 16) & 0xFF),
                    static_cast<std::uint8_t>((v >> 8) & 0xFF),
                    static_cast<std::uint8_t>(v & 0xFF)};
    }
};

// Scale RGB by a factor (alpha preserved), clamped to [0,255]. Useful for
// shading a base tone lighter (>1) or darker (<1).
Rgba shade(const Rgba& c, double factor) noexcept;

// Linear interpolation between two colors, t in [0,1] (per channel).
Rgba lerp(const Rgba& a, const Rgba& b, double t) noexcept;

// ---------------------------------------------------------------------------
// Camera / projection
// ---------------------------------------------------------------------------

// Oblique/dimetric projection parameters: the scene is seen from above and
// slightly to the side. These map a world point (x east, y depth, z up) to a
// screen point. This mirrors the City 3D Viewpoint but lives in the math layer
// so it is reusable and testable on its own.
struct ObliqueCamera {
    double tilt_x = 0.55;       // screen-x skew per unit depth
    double tilt_y = 0.42;       // screen-y advance per unit depth
    double scale_x = 9.0;       // screen px per unit east
    double scale_height = 3.0;  // screen px per unit up (z)
    double origin_x = 150.0;    // screen px for world (0,0,0)
    double origin_y = 210.0;

    Vec2 project(const Vec3& world) const noexcept;
};

// ---------------------------------------------------------------------------
// Screen geometry
// ---------------------------------------------------------------------------

struct BBox {
    double min_x = 0.0, min_y = 0.0, max_x = 0.0, max_y = 0.0;
    bool empty = true;

    void add(const Vec2& p) noexcept;
};

// A convex screen-space quad (4 corners, in order around the perimeter).
struct Quad {
    std::array<Vec2, 4> p{};

    BBox bounds() const noexcept;
    Vec2 centroid() const noexcept;
};

// ---------------------------------------------------------------------------
// Raster sink + scanline fill
// ---------------------------------------------------------------------------

// Minimal target the fill writes into. Implemented over a Phraign frame in the
// rendering-group layer, but kept abstract here so the math has no Phraign
// dependency and can be unit-tested against a plain buffer.
class RenderSink {
public:
    virtual ~RenderSink() = default;
    virtual std::size_t width() const noexcept = 0;
    virtual std::size_t height() const noexcept = 0;
    // Write one pixel; return true if it landed inside the target.
    virtual bool plot(std::size_t x, std::size_t y, std::uint32_t rgba) = 0;
};

// Fill a convex quad into the sink by scanline. Pixel centers at (x+0.5,y+0.5)
// are tested. Returns the number of pixels written. Clipped to the sink bounds.
std::size_t fillQuad(RenderSink& sink, const Quad& quad, std::uint32_t rgba);

} // namespace sleela::render
