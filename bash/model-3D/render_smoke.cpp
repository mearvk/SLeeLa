// Phraign(TM) City 3D -- rendering math + rendering group smoke test.

#include "render_group.hpp"
#include "render_math.hpp"
#include "pixel_terminal.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace sleela::render;

namespace {

bool approx(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

// A simple in-memory RenderSink for testing fillQuad without Phraign.
class BufferSink : public RenderSink {
public:
    BufferSink(std::size_t w, std::size_t h) : w_(w), h_(h), px_(w * h, 0) {}
    std::size_t width() const noexcept override { return w_; }
    std::size_t height() const noexcept override { return h_; }
    bool plot(std::size_t x, std::size_t y, std::uint32_t rgba) override {
        if (x >= w_ || y >= h_) return false;
        px_[y * w_ + x] = rgba;
        return true;
    }
    std::uint32_t at(std::size_t x, std::size_t y) const { return px_[y * w_ + x]; }
private:
    std::size_t w_, h_;
    std::vector<std::uint32_t> px_;
};

} // namespace

int main() {
    // --- Vector math --------------------------------------------------------
    Vec3 a{1, 0, 0}, b{0, 1, 0};
    assert(approx(dot(a, b), 0.0));
    Vec3 c = cross(a, b);
    assert(approx(c.x, 0) && approx(c.y, 0) && approx(c.z, 1));
    assert(approx(length(Vec3{3, 4, 0}), 5.0));
    Vec3 n = normalize(Vec3{0, 5, 0});
    assert(approx(n.y, 1.0));
    Vec3 z = normalize(Vec3{0, 0, 0});  // zero-safe
    assert(approx(length(z), 0.0));

    // signedArea2: CCW triangle is positive.
    assert(signedArea2(Vec2{0, 0}, Vec2{1, 0}, Vec2{0, 1}) > 0.0);

    // --- Color math ---------------------------------------------------------
    Rgba red{200, 0, 0, 255};
    assert(red.pack() == 0xC80000FFu);
    assert(Rgba::unpack(0xC80000FFu).r == 200);
    Rgba dark = shade(red, 0.5);
    assert(dark.r == 100 && dark.a == 255);
    Rgba bright = shade(red, 10.0);  // clamps
    assert(bright.r == 255);
    Rgba mid = lerp(Rgba{0, 0, 0, 255}, Rgba{255, 255, 255, 255}, 0.5);
    assert(mid.r >= 127 && mid.r <= 128);

    // --- Projection ---------------------------------------------------------
    ObliqueCamera cam;  // defaults
    Vec2 origin = cam.project(Vec3{0, 0, 0});
    assert(approx(origin.x, cam.origin_x) && approx(origin.y, cam.origin_y));
    // Raising z lifts screen-y upward (smaller y).
    Vec2 lifted = cam.project(Vec3{0, 0, 10});
    assert(lifted.y < origin.y);
    // Moving east increases screen-x.
    assert(cam.project(Vec3{5, 0, 0}).x > origin.x);
    // Moving in depth advances screen-y and skews x.
    Vec2 deep = cam.project(Vec3{0, 5, 0});
    assert(deep.y > origin.y && deep.x > origin.x);

    // --- Quad geometry ------------------------------------------------------
    Quad q{{Vec2{2, 2}, Vec2{6, 2}, Vec2{6, 6}, Vec2{2, 6}}};
    BBox bb = q.bounds();
    assert(!bb.empty && approx(bb.min_x, 2) && approx(bb.max_x, 6));
    Vec2 ctr = q.centroid();
    assert(approx(ctr.x, 4) && approx(ctr.y, 4));

    // --- fillQuad against a buffer -----------------------------------------
    BufferSink buf(16, 16);
    const std::uint32_t COL = 0x11223344u;
    std::size_t drawn = fillQuad(buf, q, COL);
    // A 4x4 axis-aligned square (x in [2,6), y in [2,6)) => 16 pixels.
    assert(drawn == 16);
    assert(buf.at(3, 3) == COL);
    assert(buf.at(0, 0) == 0);  // outside the quad

    // Clipping: a quad partly off the left edge still fills only in-bounds.
    Quad off{{Vec2{-4, 2}, Vec2{2, 2}, Vec2{2, 6}, Vec2{-4, 6}}};
    BufferSink buf2(16, 16);
    std::size_t d2 = fillQuad(buf2, off, COL);
    assert(d2 > 0);
    assert(buf2.at(0, 3) == COL);

    // --- RenderGroup ordering ----------------------------------------------
    // Two overlapping full-cell quads; the higher-depth one must win.
    Quad full{{Vec2{0, 0}, Vec2{16, 0}, Vec2{16, 16}, Vec2{0, 16}}};
    RenderGroup group;
    const std::uint32_t FAR = 0x0000FFFFu;   // added first, lower depth
    const std::uint32_t NEAR = 0xFF0000FFu;  // added second, higher depth
    group.addQuad(full, NEAR, 10.0);  // insert near first...
    group.addQuad(full, FAR, 1.0);    // ...then far, to prove sorting reorders
    assert(group.size() == 2);
    BufferSink buf3(16, 16);
    std::size_t d3 = group.draw(buf3, /*sortByDepth=*/true);
    assert(d3 == 16u * 16u * 2u);      // both quads fully drawn
    assert(buf3.at(8, 8) == NEAR);     // near (higher depth) drawn last -> wins

    // Insertion-order draw (no sort): last inserted wins, regardless of depth.
    // Use a fresh group so the earlier sorted draw() (which reorders in place)
    // does not affect insertion order here.
    RenderGroup group2;
    group2.addQuad(full, NEAR, 10.0);  // inserted first
    group2.addQuad(full, FAR, 1.0);    // inserted last -> should win unsorted
    BufferSink buf4(16, 16);
    group2.draw(buf4, /*sortByDepth=*/false);
    assert(buf4.at(8, 8) == FAR);

    // --- PhraignSink over a real Phraign frame ------------------------------
    sleela::terminal::PixelTerminal term(sleela::terminal::Size{32, 24});
    PhraignSink sink(term);
    assert(sink.width() == 32 && sink.height() == 24);
    assert(sink.plot(5, 5, COL));
    assert(!sink.plot(100, 100, COL));  // out of bounds
    RenderGroup g2;
    g2.addQuadAutoDepth(Quad{{Vec2{0, 0}, Vec2{32, 0}, Vec2{32, 24}, Vec2{0, 24}}},
                        0x00FF00FFu);
    std::size_t d5 = g2.draw(sink);
    assert(d5 == 32u * 24u);

    std::cout << "phraign render math+group smoke: OK\n";
    return 0;
}
