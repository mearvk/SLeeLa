#pragma once

// SleelaTerminal(TM) -- SLeeLa's terminal product, built on GNU Bash
// (GPL, (C) Free Software Foundation). SLeeLa-authored addition; the
// vendored GNU Bash sources keep their own GPL headers and copyright.
// Phraign(TM) City 3D -- rendering group.
//
// A rendering group is an ordered collection of drawable items (currently
// filled quads) that are submitted to a render target and drawn in a controlled
// order. Because the city view has no z-buffer, correct occlusion comes from
// draw order: items carry a painter `depth` key and the group draws
// low-depth (far) items first, high-depth (near) items last.
//
// Layers separate cleanly:
//   render_math   -- pure geometry/color/fill, no Phraign dependency.
//   render_group  -- this: a RenderSink over Phraign + the item container.
//   city_renderer -- builds a group of quads from a City and draws it.

#include "render_math.hpp"

// Phraign native pixel-terminal layer (sibling in bash/).
#include "pixel_terminal.hpp"

#include <cstdint>
#include <vector>

namespace sleela::render {

// RenderSink implemented over a Phraign PixelTerminal frame. Each plot() writes
// one pixel of the Phraign frame, keeping rendering on a per-pixel basis.
class PhraignSink : public RenderSink {
public:
    explicit PhraignSink(sleela::terminal::PixelTerminal& terminal) noexcept
        : terminal_(terminal) {}

    std::size_t width() const noexcept override {
        return terminal_.pixelSize().width;
    }
    std::size_t height() const noexcept override {
        return terminal_.pixelSize().height;
    }
    bool plot(std::size_t x, std::size_t y, std::uint32_t rgba) override {
        return terminal_.setPixel(x, y, sleela::terminal::Pixel{rgba});
    }

private:
    sleela::terminal::PixelTerminal& terminal_;
};

// One drawable in a rendering group: a convex screen-space quad plus a packed
// RGBA color and a painter depth key (larger = nearer = drawn later).
struct QuadItem {
    Quad geom{};
    std::uint32_t color = 0;
    double depth = 0.0;
};

// An ordered collection of drawable items drawn to a RenderSink.
class RenderGroup {
public:
    // Add a quad with an explicit color and depth key.
    void addQuad(const Quad& q, std::uint32_t color, double depth);
    void addQuad(const Quad& q, Rgba color, double depth) {
        addQuad(q, color.pack(), depth);
    }

    // Add a quad using its centroid's screen-y as the depth key (a convenient
    // default: things lower on screen are nearer and drawn on top).
    void addQuadAutoDepth(const Quad& q, std::uint32_t color);

    std::size_t size() const noexcept { return items_.size(); }
    bool empty() const noexcept { return items_.empty(); }
    void clear() noexcept { items_.clear(); }
    const std::vector<QuadItem>& items() const noexcept { return items_; }

    // Draw every item into the sink. If `sortByDepth` is true the group is
    // stably sorted by ascending depth first (painter's algorithm); otherwise
    // items draw in insertion order. Returns total pixels written.
    //
    // Note: with sortByDepth == true the internal item order is reordered
    // in place, so a subsequent unsorted draw() observes the sorted order.
    std::size_t draw(RenderSink& sink, bool sortByDepth = true);

private:
    std::vector<QuadItem> items_;
};

} // namespace sleela::render
