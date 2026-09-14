// Phraign(TM) City 3D -- oblique projection + per-pixel raster onto a Phraign
// frame.
//
// Projection model
// ----------------
// World coordinates: block (bx, by) sits on a ground grid; height h rises along
// +Z. We use an oblique projection so the city is seen from above and slightly
// to the side:
//
//   ground point (wx, wy):
//     sx = origin_x + wx*scale_x + wy*tilt_x*block_pitch
//     sy = origin_y + wy*tilt_y*block_pitch
//   raising by height h lifts the screen point up:
//     sy -= h * scale_height
//
// where wx = bx*block_pitch (in world units scaled by scale_x through the
// pitch) -- in practice we fold block_pitch into the ground step so the code
// walks the grid directly.
//
// Painter's algorithm: draw far rows (small by) first and near rows last, and
// within a row draw left-to-right, so nearer/taller buildings correctly occlude
// those behind them. Each building is drawn as a top face (roof), a lit side,
// and a shaded side; the ground plane fills the gaps.

#include "city_renderer.hpp"

#include <algorithm>
#include <cmath>

namespace sleela::city {

namespace {

using sleela::terminal::Pixel;
using sleela::terminal::PixelTerminal;

struct ScreenPt {
    double x;
    double y;
};

// Project a ground grid cell corner (in block units) at height h to screen.
ScreenPt project(const Viewpoint& vp, double bx, double by, double h) {
    const double gx = bx * vp.block_pitch;
    const double gy = by * vp.block_pitch;
    ScreenPt p;
    p.x = vp.origin_x + gx * (vp.scale_x / vp.block_pitch) + gy * vp.tilt_x;
    p.y = vp.origin_y + gy * vp.tilt_y - h * vp.scale_height;
    return p;
}

// Fill an axis-aligned-ish quad given its 4 screen corners by scanline. The
// quads we produce (roof parallelogram, wall parallelograms) are convex, so a
// simple polygon scanline fill is exact and stays on a per-pixel basis.
void fillQuad(PixelTerminal& t, const ScreenPt q[4], Pixel color,
              std::size_t& drawn) {
    double minY = q[0].y, maxY = q[0].y;
    for (int i = 1; i < 4; ++i) {
        minY = std::min(minY, q[i].y);
        maxY = std::max(maxY, q[i].y);
    }
    int y0 = static_cast<int>(std::floor(minY));
    int y1 = static_cast<int>(std::ceil(maxY));
    const auto H = static_cast<int>(t.pixelSize().height);
    const auto W = static_cast<int>(t.pixelSize().width);
    y0 = std::max(y0, 0);
    y1 = std::min(y1, H - 1);

    for (int y = y0; y <= y1; ++y) {
        const double sy = y + 0.5;
        double xs[8];
        int n = 0;
        for (int i = 0; i < 4; ++i) {
            const ScreenPt& a = q[i];
            const ScreenPt& b = q[(i + 1) % 4];
            const double ay = a.y, by = b.y;
            if ((sy >= ay && sy < by) || (sy >= by && sy < ay)) {
                const double t01 = (sy - ay) / (by - ay);
                xs[n++] = a.x + t01 * (b.x - a.x);
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
                if (t.setPixel(static_cast<std::size_t>(x),
                               static_cast<std::size_t>(y), color)) {
                    ++drawn;
                }
            }
        }
    }
}

Pixel toPixel(const Color& c) { return Pixel{c.rgba()}; }

} // namespace

std::size_t Renderer::render(const City& city, PixelTerminal& terminal) const {
    // Size the Phraign frame to the configured dimensions.
    terminal.setSize(sleela::terminal::Size{options_.frame_width,
                                            options_.frame_height});
    terminal.begin();

    const Palette pal = Palette::forTheme(options_.theme);
    terminal.fill(toPixel(pal.background));

    const Viewpoint& vp = options_.viewpoint;
    std::size_t drawn = 0;

    // Ground plane: draw the whole grid footprint as one quad first, so streets
    // and empty lots read as ground.
    {
        ScreenPt g[4] = {
            project(vp, 0, 0, 0),
            project(vp, city.cols(), 0, 0),
            project(vp, city.cols(), city.rows(), 0),
            project(vp, 0, city.rows(), 0),
        };
        fillQuad(terminal, g, toPixel(pal.ground), drawn);
    }

    const Pixel roof = toPixel(pal.roof);
    const Pixel lit = toPixel(pal.wall_lit);
    const Pixel shade = toPixel(pal.wall_shade);

    // Painter's algorithm: far rows (small by) first, near rows last.
    for (std::uint32_t by = 0; by < city.rows(); ++by) {
        for (std::uint32_t bx = 0; bx < city.cols(); ++bx) {
            const double h = city.at(bx, by).height;
            if (h <= 0.0) continue;

            const double x0 = bx, x1 = bx + 1.0;
            const double y0 = by, y1 = by + 1.0;

            // Right (lit) side wall: from the x1 edge, ground->roof.
            ScreenPt rightWall[4] = {
                project(vp, x1, y0, 0),
                project(vp, x1, y1, 0),
                project(vp, x1, y1, h),
                project(vp, x1, y0, h),
            };
            fillQuad(terminal, rightWall, lit, drawn);

            // Front (shaded) side wall: from the y1 edge, ground->roof.
            ScreenPt frontWall[4] = {
                project(vp, x0, y1, 0),
                project(vp, x1, y1, 0),
                project(vp, x1, y1, h),
                project(vp, x0, y1, h),
            };
            fillQuad(terminal, frontWall, shade, drawn);

            // Roof: the top face at height h.
            ScreenPt top[4] = {
                project(vp, x0, y0, h),
                project(vp, x1, y0, h),
                project(vp, x1, y1, h),
                project(vp, x0, y1, h),
            };
            fillQuad(terminal, top, roof, drawn);
        }
    }

    terminal.show();
    return drawn;
}

} // namespace sleela::city
