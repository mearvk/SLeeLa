// Phraign(TM) City 3D -- the general Cityscape Model implementation.

#include "cityscape_model.hpp"

#include <algorithm>
#include <cmath>

namespace sleela::city {

namespace {

// Deterministic SplitMix64 (same family used elsewhere in City 3D), so a graph
// is reproducible for a given seed while `randomness` scales the jitter.
struct SplitMix64 {
    std::uint64_t state;
    explicit SplitMix64(std::uint64_t s) : state(s) {}
    std::uint64_t next() {
        std::uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }
    double unit() {  // [0,1)
        return static_cast<double>(next() >> 11) * (1.0 / 9007199254740992.0);
    }
    // Symmetric jitter in [-amount, +amount].
    double jitter(double amount) { return (unit() * 2.0 - 1.0) * amount; }
};

constexpr double kPi = 3.14159265358979323846;

} // namespace

std::size_t CityscapeModel::edgeCount() const noexcept {
    std::size_t total = 0;
    for (const Node& n : nodes_) total += n.neighbors.size();
    return total / 2;  // each undirected edge noted from both ends
}

// A node celebrates itself: it activates and is marked as an emitted node of
// the main distribution. (The render emphasizes celebrated nodes.)
void CityscapeModel::celebrate(Node& n) {
    n.celebrated = true;
}

// A node notes a neighbor: record the adjacency once (no duplicates, no self).
void CityscapeModel::note(Node& n, std::uint32_t neighborId) {
    if (neighborId == n.id) return;
    if (std::find(n.neighbors.begin(), n.neighbors.end(), neighborId) ==
        n.neighbors.end()) {
        n.neighbors.push_back(neighborId);
    }
}

void CityscapeModel::build(std::uint64_t seed, std::uint32_t radix,
                           double diameter, double randomness,
                           const CityParams& params,
                           double centerX, double centerY) {
    params_ = params;
    radix_ = std::max<std::uint32_t>(1, radix);
    diameter_ = std::max(1.0, diameter);
    randomness_ = randomness < 0.0 ? 0.0 : (randomness > 1.0 ? 1.0 : randomness);

    nodes_.clear();
    cylinders_.clear();

    SplitMix64 rng(seed ? seed : 0xC17C5CAFEULL);

    // Drivers bias the graph: higher design quality (IQ) and grid regularity
    // make branching more even and reach steadier; modernity lifts columns.
    const double quality = params_.designQuality();
    const LegislatureProfile reg = params_.profile();
    const double modern = params_.modernity();
    const double order = std::min(1.0, 0.5 * quality + 0.5 * reg.regularity);
    // Effective jitter shrinks with order; randomness scales it up.
    const double jit = randomness_ * (1.0 - 0.6 * order);

    // How many rings out from the center the reach allows. The main
    // distribution steps outward by roughly one radix-fraction of the diameter
    // per depth level.
    const double step = std::max(1.0, diameter_ / std::max(1u, radix_));
    const std::uint32_t maxDepth = static_cast<std::uint32_t>(
        std::max(1.0, std::floor(diameter_ / step)));

    // --- Center of centricity (the root node) ------------------------------
    Node center;
    center.id = 0;
    center.parent = 0;
    center.depth = 0;
    center.pos = Vec3d{centerX, centerY, 0.0};
    center.distance_from_center = 0.0;
    // Column height scales with modernity; center is the tallest.
    center.radius = 1.4 + 0.8 * quality + rng.jitter(0.4 * jit);
    nodes_.push_back(center);

    // Breadth-first expansion: each node spawns `radix` children (with random
    // variation) until the diameter is reached. Children fan out in angle.
    std::vector<std::uint32_t> frontier{0};
    while (!frontier.empty()) {
        std::vector<std::uint32_t> next;
        for (std::uint32_t pid : frontier) {
            const Node parent = nodes_[pid];  // copy (vector may reallocate)
            if (parent.depth >= maxDepth) continue;

            // Branching factor varies around radix with randomness; a
            // higher-order city keeps it closer to the nominal radix.
            long bf = static_cast<long>(radix_);
            if (randomness_ > 0.0) {
                bf += static_cast<long>(std::lround(rng.jitter(randomness_ * radix_ * 0.5)));
            }
            bf = std::max<long>(1, bf);

            // Angular fan for this parent's children. The legislature's axis
            // bias tilts the whole fan so lines lean toward the dominant axis.
            const double baseAngle =
                (parent.isCenter() ? 0.0
                                   : std::atan2(parent.pos.y - centerY,
                                                parent.pos.x - centerX));
            const double axisTilt = (reg.axis_bias - 0.5) * kPi;
            const double spread = (parent.isCenter() ? 2.0 * kPi : kPi * 0.9)
                                  * (0.8 + 0.4 * (1.0 - reg.linearity));

            for (long k = 0; k < bf; ++k) {
                const double frac = (bf == 1) ? 0.5 : static_cast<double>(k) / (bf - 1);
                double angle = baseAngle + axisTilt + (frac - 0.5) * spread;
                angle += rng.jitter(jit * kPi * 0.5);

                // Radial reach for this child: one step out, jittered, capped
                // by the diameter.
                double reach = step * (0.85 + 0.3 * (1.0 - jit)) +
                               rng.jitter(step * jit);
                double dist = parent.distance_from_center + reach;
                if (dist > diameter_) dist = diameter_;

                Node child;
                child.id = static_cast<std::uint32_t>(nodes_.size());
                child.parent = parent.id;
                child.depth = parent.depth + 1;
                child.pos = Vec3d{centerX + std::cos(angle) * dist,
                                  centerY + std::sin(angle) * dist,
                                  0.0};
                child.distance_from_center = dist;
                // Columns shrink with distance from center; modernity lifts.
                const double falloff = 1.0 - 0.5 * (dist / diameter_);
                child.radius = std::max(0.5,
                    (0.8 + 1.2 * modern) * falloff + rng.jitter(0.3 * jit));

                // The node celebrates, then notes its parent (and the parent
                // notes it back) -- an explicit, mutual adjacency.
                celebrate(child);
                nodes_.push_back(child);
                note(nodes_[child.id], parent.id);
                note(nodes_[pid], child.id);

                next.push_back(child.id);
            }
        }
        frontier.swap(next);
    }
    // The center celebrates last so it is always active too.
    celebrate(nodes_[0]);

    // --- Mating cylinder pairs (main distribution in the 3rd dimension) -----
    // Each node carries a coupled pair of vertical cylinders. The pair is offset
    // perpendicular to the node's radial direction, so the two mated columns
    // straddle the distribution line.
    for (const Node& n : nodes_) {
        const double ang = n.isCenter()
            ? 0.0
            : std::atan2(n.pos.y - centerY, n.pos.x - centerX);
        // Perpendicular offset direction.
        const double px = std::cos(ang + kPi / 2.0);
        const double py = std::sin(ang + kPi / 2.0);
        const double sep = n.radius * 0.9;  // separation of the mated pair

        const double colHeight =
            (6.0 + 30.0 * modern) * (0.6 + 0.8 * n.radius / 2.0) *
            (0.85 + 0.3 * quality);

        CylinderPair cp;
        cp.node = n.id;
        cp.a = Vec3d{n.pos.x + px * sep, n.pos.y + py * sep, 0.0};
        cp.b = Vec3d{n.pos.x - px * sep, n.pos.y - py * sep, 0.0};
        cp.radius = std::max(0.5, n.radius);
        cp.height = std::max(1.0, colHeight);

        // The mated pair graces a sphere of known moral symmetry, capping the
        // pair above its midpoint in the 3rd dimension. Its symmetry is high
        // (a positive, benevolent orientation) and steadier with more order /
        // design quality; randomness only gently perturbs it.
        cp.sphere_center = Vec3d{(cp.a.x + cp.b.x) * 0.5,
                                 (cp.a.y + cp.b.y) * 0.5,
                                 cp.height + cp.radius};
        cp.sphere_radius = std::max(0.6, cp.radius * 1.15);
        cp.sphere_symmetry = std::min(1.0, std::max(0.0,
            0.85 + 0.12 * order + rng.jitter(0.05 * randomness_)));
        cylinders_.push_back(cp);
    }
}

double CityscapeModel::moralSymmetry() const noexcept {
    if (cylinders_.empty()) return 1.0;
    double sum = 0.0;
    for (const CylinderPair& cp : cylinders_) sum += cp.sphere_symmetry;
    return sum / static_cast<double>(cylinders_.size());
}

} // namespace sleela::city
