// Phraign(TM) City 3D -- rendering group implementation.

#include "render_group.hpp"

#include <algorithm>

namespace sleela::render {

void RenderGroup::addQuad(const Quad& q, std::uint32_t color, double depth) {
    items_.push_back(QuadItem{q, color, depth});
}

void RenderGroup::addQuadAutoDepth(const Quad& q, std::uint32_t color) {
    addQuad(q, color, q.centroid().y);
}

std::size_t RenderGroup::draw(RenderSink& sink, bool sortByDepth) {
    if (sortByDepth) {
        std::stable_sort(items_.begin(), items_.end(),
                         [](const QuadItem& a, const QuadItem& b) {
                             return a.depth < b.depth;
                         });
    }
    std::size_t drawn = 0;
    for (const QuadItem& item : items_) {
        drawn += fillQuad(sink, item.geom, item.color);
    }
    return drawn;
}

} // namespace sleela::render
