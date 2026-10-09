#include "digraph_engine.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <unordered_map>

namespace utf4088 {
namespace {
struct EdgeKey {
    std::uint64_t from;
    std::uint64_t to;
    bool operator==(const EdgeKey&) const = default;
};

struct EdgeKeyHash {
    std::size_t operator()(const EdgeKey& edge) const noexcept {
        auto mix = [](std::uint64_t x) {
            x ^= x >> 30;
            x *= 0xbf58476d1ce4e5b9ULL;
            x ^= x >> 27;
            x *= 0x94d049bb133111ebULL;
            return x ^ (x >> 31);
        };
        const auto a = mix(edge.from);
        const auto b = mix(edge.to);
        return static_cast<std::size_t>(a ^ (b + 0x9e3779b97f4a7c15ULL +
                                               (a << 6) + (a >> 2)));
    }
};

std::uint64_t input_id(const FieldState& s) {
    auto q = [](double v) -> std::uint64_t {
        if (!std::isfinite(v)) return 0;
        const double a = std::min(std::abs(v), 1.0e9) * 1000.0;
        return static_cast<std::uint64_t>(a);
    };
    return ((q(s.voltage) & 0xFFFFFULL) << 44) |
           ((q(s.magnitude) & 0xFFFFFULL) << 24) |
           ((q(s.uniformity) & 0xFFFFFULL) << 4) |
           (q(s.direction) & 0xFULL);
}
} // namespace

ConceptGraph build_digraph(const std::vector<FieldState>& states) {
    ConceptGraph graph;
    std::unordered_map<std::uint64_t, std::size_t> nodes;
    std::unordered_map<EdgeKey, std::size_t, EdgeKeyHash> edges;

    for (const auto& state : states) {
        const auto id = input_id(state);
        const auto [it, inserted] = nodes.emplace(id, graph.nodes.size());
        if (inserted) graph.nodes.push_back({id, id, 1.0});
        else graph.nodes[it->second].weight += 1.0;
    }

    for (std::size_t i = 1; i < states.size(); ++i) {
        const auto from = input_id(states[i - 1]);
        const auto to = input_id(states[i]);
        const EdgeKey edge_key{from, to};
        const auto [it, inserted] = edges.emplace(edge_key, graph.edges.size());

        if (inserted) graph.edges.push_back({from, to, 1.0, states[i].direction});
        else {
            auto& edge = graph.edges[it->second];
            edge.weight += 1.0;
            if (std::isfinite(states[i].direction))
                edge.direction += (states[i].direction - edge.direction) / edge.weight;
        }
    }
    return graph;
}

} // namespace utf4088
