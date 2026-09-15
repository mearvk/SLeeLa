// Phraign(TM) City 3D -- build a rendering group from a City and draw it onto a
// Phraign frame.
//
// This layer no longer contains raw projection/scanline math: it uses
// render_math (ObliqueCamera, Quad, Rgba) to build screen-space quads and
// render_group (RenderGroup + PhraignSink) to order and rasterize them.
//
// Projection: an oblique view seen from above and slightly to the side. World
// coordinates are (x east, y depth, z up); a block (bx,by) occupies the unit
// cell [bx,bx+1] x [by,by+1] on the ground, and a building rises to z = height.
//
// Occlusion: no z-buffer -- the RenderGroup draws by ascending painter depth so
// far/lower buildings are covered by nearer/taller ones. Each building becomes
// three quads (lit side, shaded side, roof); the ground plane is one quad drawn
// first.

#include "city_renderer.hpp"

#include "render_group.hpp"
#include "render_math.hpp"

namespace sleela::city {

namespace {

using sleela::render::ObliqueCamera;
using sleela::render::Quad;
using sleela::render::Rgba;
using sleela::render::Vec2;
using sleela::render::Vec3;

Rgba toRgba(const Color& c) { return Rgba{c.r, c.g, c.b, c.a}; }

// Build the oblique camera from the City Viewpoint. block_pitch scales the grid
// step into world "east/depth" units so a change in pitch spreads the city out.
ObliqueCamera cameraFor(const Viewpoint& vp) {
    ObliqueCamera cam;
    // Fold block_pitch into the per-unit scales so one grid cell == one world
    // unit at the camera. This preserves the previous on-screen geometry.
    cam.scale_x = vp.scale_x;
    cam.tilt_x = vp.tilt_x * vp.block_pitch;
    cam.tilt_y = vp.tilt_y * vp.block_pitch;
    cam.scale_height = vp.scale_height;
    cam.origin_x = vp.origin_x;
    cam.origin_y = vp.origin_y;
    return cam;
}

// A quad from four world points, projected through the camera.
Quad worldQuad(const ObliqueCamera& cam,
               const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d) {
    return Quad{{cam.project(a), cam.project(b), cam.project(c), cam.project(d)}};
}

} // namespace

std::size_t Renderer::render(const City& city,
                             sleela::terminal::PixelTerminal& terminal) const {
    // Size the Phraign frame to the configured dimensions.
    terminal.setSize(sleela::terminal::Size{options_.frame_width,
                                            options_.frame_height});
    terminal.begin();

    const Palette pal = Palette::forTheme(options_.theme);
    terminal.fill(sleela::terminal::Pixel{toRgba(pal.background).pack()});

    const ObliqueCamera cam = cameraFor(options_.viewpoint);

    const Rgba ground = toRgba(pal.ground);
    const Rgba roof = toRgba(pal.roof);
    const Rgba lit = toRgba(pal.wall_lit);
    const Rgba shade = toRgba(pal.wall_shade);

    sleela::render::RenderGroup group;

    // Ground plane first (depth pushed to -inf so it is always behind).
    {
        const double W = city.cols();
        const double H = city.rows();
        Quad g = worldQuad(cam, Vec3{0, 0, 0}, Vec3{W, 0, 0},
                           Vec3{W, H, 0}, Vec3{0, H, 0});
        group.addQuad(g, ground, -1.0e18);
    }

    // Each building contributes three quads. The painter depth key is the
    // screen-y of the building's near-base edge, so nearer buildings draw last.
    for (std::uint32_t by = 0; by < city.rows(); ++by) {
        for (std::uint32_t bx = 0; bx < city.cols(); ++bx) {
            const double h = city.at(bx, by).height;
            if (h <= 0.0) continue;

            const double x0 = bx, x1 = bx + 1.0;
            const double y0 = by, y1 = by + 1.0;

            // Depth: nearer (larger by) and taller draw later. Using the near
            // base corner's projected y, nudged up by height, matches the old
            // far-to-near, short-to-tall ordering.
            const double depth =
                cam.project(Vec3{x1, y1, 0}).y + h * 1.0e-3;

            // Lit side (the x1 face), ground -> roof.
            group.addQuad(
                worldQuad(cam, Vec3{x1, y0, 0}, Vec3{x1, y1, 0},
                          Vec3{x1, y1, h}, Vec3{x1, y0, h}),
                lit, depth);

            // Shaded side (the y1 face), ground -> roof.
            group.addQuad(
                worldQuad(cam, Vec3{x0, y1, 0}, Vec3{x1, y1, 0},
                          Vec3{x1, y1, h}, Vec3{x0, y1, h}),
                shade, depth);

            // Roof (top face at z = h). Slightly greater depth so it covers its
            // own walls.
            group.addQuad(
                worldQuad(cam, Vec3{x0, y0, h}, Vec3{x1, y0, h},
                          Vec3{x1, y1, h}, Vec3{x0, y1, h}),
                roof, depth + 0.5);
        }
    }

    sleela::render::PhraignSink sink(terminal);
    const std::size_t drawn = group.draw(sink, /*sortByDepth=*/true);

    terminal.show();
    return drawn;
}

} // namespace sleela::city
