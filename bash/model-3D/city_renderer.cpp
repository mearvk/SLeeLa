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

#include <algorithm>
#include <array>
#include <cmath>

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

// Map a building's finality (quality of condition, 0..1) to a brightness
// factor. Well-conditioned/modern buildings render brighter and cleaner; poor
// (unfinished/aged) ones render darker and duller. A finality of ~0.55 renders
// at the palette's nominal tone (factor 1.0).
double conditionFactor(double finality) {
    // 0 -> 0.6 (dull), 0.55 -> 1.0 (nominal), 1 -> ~1.32 (bright/clean).
    const double f = finality < 0.0 ? 0.0 : (finality > 1.0 ? 1.0 : finality);
    return 0.6 + 0.72 * f;
}

// Append the mating cylinder pairs, the spheres of known moral symmetry they
// grace, and the noted neighbor links to a render group. Cylinders are drawn as
// octagonal prisms rising in the 3rd dimension; the sphere is an octagon cap.
void addCityscapeGraph(const ObliqueCamera& cam,
                       const CityscapeModel& model, const Palette& pal,
                       sleela::render::RenderGroup& group) {
    using sleela::render::shade;
    const Rgba column = sleela::render::lerp(toRgba(pal.wall_lit),
                                             toRgba(pal.roof), 0.35);
    const Rgba columnShade = shade(column, 0.7);
    const Rgba sphereC = sleela::render::lerp(toRgba(pal.roof),
                                              Rgba{255, 255, 255, 255}, 0.35);
    const Rgba linkC = shade(toRgba(pal.ground), 1.6);

    auto centroidY = [&](double x, double y, double z) {
        return cam.project(Vec3{x, y, z}).y;
    };

    // Neighbor links: a thin ground ribbon between noted neighbors (each edge
    // once). Drawn low so columns rise above them.
    for (const Node& n : model.nodes()) {
        for (std::uint32_t m : n.neighbors) {
            if (m <= n.id) continue;  // once per undirected edge
            const Node& o = model.nodes()[m];
            const double w = 0.18;
            const double depth = centroidY((n.pos.x + o.pos.x) * 0.5,
                                           (n.pos.y + o.pos.y) * 0.5, 0.0) - 0.2;
            group.addQuad(
                worldQuad(cam, Vec3{n.pos.x - w, n.pos.y - w, 0.1},
                          Vec3{n.pos.x + w, n.pos.y + w, 0.1},
                          Vec3{o.pos.x + w, o.pos.y + w, 0.1},
                          Vec3{o.pos.x - w, o.pos.y - w, 0.1}),
                linkC, depth);
        }
    }

    // A single mated cylinder drawn as an octagonal prism at (cx,cy) rising to
    // height h with radius r.
    auto drawColumn = [&](double cx, double cy, double r, double h) {
        const int seg = 8;
        const double depth = centroidY(cx, cy, 0.0) + h * 1.0e-3;
        for (int i = 0; i < seg; ++i) {
            const double a0 = (2.0 * 3.14159265358979 * i) / seg;
            const double a1 = (2.0 * 3.14159265358979 * (i + 1)) / seg;
            const double x0 = cx + std::cos(a0) * r, y0 = cy + std::sin(a0) * r;
            const double x1 = cx + std::cos(a1) * r, y1 = cy + std::sin(a1) * r;
            // Facets facing +x/+y are lit; others shaded (cheap shading).
            const bool litFace = (std::cos((a0 + a1) * 0.5) + std::sin((a0 + a1) * 0.5)) > 0;
            group.addQuad(
                worldQuad(cam, Vec3{x0, y0, 0}, Vec3{x1, y1, 0},
                          Vec3{x1, y1, h}, Vec3{x0, y0, h}),
                litFace ? column : columnShade, depth);
        }
        // Cap (fan of octagon triangles as quads).
        for (int i = 0; i < seg; i += 2) {
            const double a0 = (2.0 * 3.14159265358979 * i) / seg;
            const double a1 = (2.0 * 3.14159265358979 * (i + 1)) / seg;
            const double a2 = (2.0 * 3.14159265358979 * (i + 2)) / seg;
            group.addQuad(
                worldQuad(cam, Vec3{cx, cy, h},
                          Vec3{cx + std::cos(a0) * r, cy + std::sin(a0) * r, h},
                          Vec3{cx + std::cos(a1) * r, cy + std::sin(a1) * r, h},
                          Vec3{cx + std::cos(a2) * r, cy + std::sin(a2) * r, h}),
                column, depth + 0.5);
        }
    };

    // A sphere of known moral symmetry, drawn as an octagon disc facing the
    // camera, capping the mated pair.
    auto drawSphere = [&](const Vec3d& c, double r, double symmetry) {
        const int seg = 8;
        const double depth = centroidY(c.x, c.y, c.z) + 0.8;
        const Rgba s = shade(sphereC, 0.7 + 0.3 * symmetry);
        for (int i = 0; i < seg; i += 2) {
            const double a0 = (2.0 * 3.14159265358979 * i) / seg;
            const double a1 = (2.0 * 3.14159265358979 * (i + 1)) / seg;
            const double a2 = (2.0 * 3.14159265358979 * (i + 2)) / seg;
            // Draw in the x-z plane (a vertical disc facing the viewer).
            group.addQuad(
                worldQuad(cam, Vec3{c.x, c.y, c.z},
                          Vec3{c.x + std::cos(a0) * r, c.y, c.z + std::sin(a0) * r},
                          Vec3{c.x + std::cos(a1) * r, c.y, c.z + std::sin(a1) * r},
                          Vec3{c.x + std::cos(a2) * r, c.y, c.z + std::sin(a2) * r}),
                s, depth);
        }
    };

    for (const CylinderPair& cp : model.cylinders()) {
        drawColumn(cp.a.x, cp.a.y, cp.radius, cp.height);
        drawColumn(cp.b.x, cp.b.y, cp.radius, cp.height);
        drawSphere(cp.sphere_center, cp.sphere_radius, cp.sphere_symmetry);
    }
}

// Build the base city (ground + buildings + bridges + windows) into a group.
void buildCityGroup(const ObliqueCamera& cam, const RenderOptions& options_,
                    const City& city, const Palette& pal,
                    sleela::render::RenderGroup& group) {
    const Rgba ground = toRgba(pal.ground);
    const Rgba roof = toRgba(pal.roof);
    const Rgba lit = toRgba(pal.wall_lit);
    const Rgba shade = toRgba(pal.wall_shade);

    // Ground plane first (depth pushed to -inf so it is always behind).
    {
        const double W = city.cols();
        const double H = city.rows();
        Quad g = worldQuad(cam, Vec3{0, 0, 0}, Vec3{W, 0, 0},
                           Vec3{W, H, 0}, Vec3{0, H, 0});
        group.addQuad(g, ground, -1.0e18);
    }

    // Bridge decking: a distinct, slightly-raised road color.
    const Rgba bridgeDeck = sleela::render::lerp(ground, roof, 0.45);
    // Window colors: darker "glass" pattern on lit faces, brighter on shaded.
    const Rgba glassLit = sleela::render::shade(lit, 0.72);
    const Rgba glassShade = sleela::render::shade(shade, 0.72);

    // Each building contributes three quads. The painter depth key is the
    // screen-y of the building's near-base edge, so nearer buildings draw last.
    for (std::uint32_t by = 0; by < city.rows(); ++by) {
        for (std::uint32_t bx = 0; bx < city.cols(); ++bx) {
            const Block& blk = city.at(bx, by);
            const double x0 = bx, x1 = bx + 1.0;
            const double y0 = by, y1 = by + 1.0;

            // Bridges: a raised deck spanning a road corridor (a real feature).
            if (blk.cell == Cell::Bridge && options_.draw_bridges) {
                const double bz = 1.2;  // slight elevation of the deck
                const double depth = cam.project(Vec3{x1, y1, 0}).y;
                // Deck top.
                group.addQuad(
                    worldQuad(cam, Vec3{x0, y0, bz}, Vec3{x1, y0, bz},
                              Vec3{x1, y1, bz}, Vec3{x0, y1, bz}),
                    bridgeDeck, depth + 0.4);
                // Near edge (a thin side so the deck reads as elevated).
                group.addQuad(
                    worldQuad(cam, Vec3{x0, y1, 0}, Vec3{x1, y1, 0},
                              Vec3{x1, y1, bz}, Vec3{x0, y1, bz}),
                    sleela::render::shade(bridgeDeck, 0.7), depth);
                continue;
            }

            if (blk.cell != Cell::Building) continue;
            const double h = blk.height;
            if (h <= 0.0) continue;

            // Condition/finality drives brightness: cleaner when well finished.
            const double cf = conditionFactor(blk.finality);
            const Rgba litC = sleela::render::shade(lit, cf);
            const Rgba shadeC = sleela::render::shade(shade, cf);
            const Rgba roofC = sleela::render::shade(roof, cf);

            // Depth: nearer (larger by) and taller draw later.
            const double depth = cam.project(Vec3{x1, y1, 0}).y + h * 1.0e-3;

            // Lit side (the x1 face), ground -> roof.
            group.addQuad(
                worldQuad(cam, Vec3{x1, y0, 0}, Vec3{x1, y1, 0},
                          Vec3{x1, y1, h}, Vec3{x1, y0, h}),
                litC, depth);

            // Shaded side (the y1 face), ground -> roof.
            group.addQuad(
                worldQuad(cam, Vec3{x0, y1, 0}, Vec3{x1, y1, 0},
                          Vec3{x1, y1, h}, Vec3{x0, y1, h}),
                shadeC, depth);

            // Window detailing: draw a grid of window bands on the two visible
            // faces. The row count follows the building's floors; the column
            // count follows windows-per-floor (derived from the windows total).
            if (options_.draw_windows && blk.floors > 0 && blk.windows > 0) {
                const int floors = blk.floors;
                const int wpf = std::max(1, static_cast<int>(
                    blk.windows / static_cast<unsigned>(floors)));
                // Cap detailing so huge towers stay cheap to draw.
                const int rowsN = std::min(floors, 20);
                const int colsN = std::min(wpf, 6);
                const Rgba gL = sleela::render::shade(glassLit, cf);
                const Rgba gS = sleela::render::shade(glassShade, cf);

                for (int r = 0; r < rowsN; ++r) {
                    // Vertical band center at fraction fz of the height.
                    const double fz0 = (r + 0.30) / rowsN * h;
                    const double fz1 = (r + 0.70) / rowsN * h;
                    for (int c = 0; c < colsN; ++c) {
                        const double u0 = (c + 0.30) / colsN;
                        const double u1 = (c + 0.70) / colsN;
                        // Lit face (x1 plane), varying along y.
                        group.addQuad(
                            worldQuad(cam,
                                Vec3{x1, y0 + u0, fz0}, Vec3{x1, y0 + u1, fz0},
                                Vec3{x1, y0 + u1, fz1}, Vec3{x1, y0 + u0, fz1}),
                            gL, depth + 0.25);
                        // Shaded face (y1 plane), varying along x.
                        group.addQuad(
                            worldQuad(cam,
                                Vec3{x0 + u0, y1, fz0}, Vec3{x0 + u1, y1, fz0},
                                Vec3{x0 + u1, y1, fz1}, Vec3{x0 + u0, y1, fz1}),
                            gS, depth + 0.25);
                    }
                }
            }

            // Roof (top face at z = h). Slightly greater depth so it covers its
            // own walls and windows.
            group.addQuad(
                worldQuad(cam, Vec3{x0, y0, h}, Vec3{x1, y0, h},
                          Vec3{x1, y1, h}, Vec3{x0, y1, h}),
                roofC, depth + 0.5);
        }
    }

}

} // namespace

std::size_t Renderer::render(const City& city,
                             sleela::terminal::PixelTerminal& terminal) const {
    terminal.setSize(sleela::terminal::Size{options_.frame_width,
                                            options_.frame_height});
    terminal.begin();

    const Palette pal = Palette::forTheme(options_.theme);
    terminal.fill(sleela::terminal::Pixel{toRgba(pal.background).pack()});

    const ObliqueCamera cam = cameraFor(options_.viewpoint);
    sleela::render::RenderGroup group;
    buildCityGroup(cam, options_, city, pal, group);

    sleela::render::PhraignSink sink(terminal);
    const std::size_t drawn = group.draw(sink, /*sortByDepth=*/true);
    terminal.show();
    return drawn;
}

std::size_t Renderer::render(const City& city, const CityscapeModel& model,
                             sleela::terminal::PixelTerminal& terminal) const {
    terminal.setSize(sleela::terminal::Size{options_.frame_width,
                                            options_.frame_height});
    terminal.begin();

    const Palette pal = Palette::forTheme(options_.theme);
    terminal.fill(sleela::terminal::Pixel{toRgba(pal.background).pack()});

    const ObliqueCamera cam = cameraFor(options_.viewpoint);
    sleela::render::RenderGroup group;
    buildCityGroup(cam, options_, city, pal, group);
    if (options_.draw_cylinders) {
        addCityscapeGraph(cam, model, pal, group);
    }

    sleela::render::PhraignSink sink(terminal);
    const std::size_t drawn = group.draw(sink, /*sortByDepth=*/true);
    terminal.show();
    return drawn;
}

} // namespace sleela::city
