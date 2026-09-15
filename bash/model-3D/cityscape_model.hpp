#pragma once

// Phraign(TM) City 3D -- the general Cityscape Model.
//
// This is the general model Sleela works from. It unifies everything City 3D
// knows -- the Year / IQ / Legislature drivers, building quality, and finality
// -- underneath a graph in the *3rd graphing dimension* (Z / up):
//
//   * A single "center of centricity" is the root node.
//   * The graph branches with a base of `radix` (branching factor).
//   * `diameter` is the distance a node may reach out from the center of
//     centricity -- the radial extent of the main distribution.
//   * `randomness` (0..1) introduces controlled, seeded variability so two runs
//     with the same seed match, while randomness > 0 varies radix, diameter,
//     node placement, and the driver values.
//
// The main distribution is realized as **mating pairs of cylinders**: at each
// distribution node a coupled pair of vertical cylinders (columns) rises in the
// 3rd dimension. Every node **celebrates** itself (activates / emits a record)
// and then **notes** each of its neighbors (records an explicit adjacency),
// so the graph carries who-is-next-to-whom.
//
// The model then composes onto the City grid (cylinders as the distribution
// skeleton, buildings filling around) and is rendered through Phraign.

#include "city_model.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::city {

// A point in the 3rd graphing dimension (x east, y depth, z up).
struct Vec3d {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

// A node in the cityscape graph. Nodes are placed relative to the center of
// centricity. A node's `radius` sizes the cylinders of its mating pair.
struct Node {
    std::uint32_t id = 0;
    std::uint32_t parent = 0;          // id of the parent (self for the root)
    std::uint32_t depth = 0;           // graph depth from the center (0 = center)
    Vec3d pos{};                       // position (blocks; z is column height)
    double radius = 1.0;               // cylinder radius (blocks)
    double distance_from_center = 0.0; // radial distance from centricity
    bool celebrated = false;           // has this node celebrated itself?
    std::vector<std::uint32_t> neighbors;  // noted neighbor ids (adjacency)

    bool isCenter() const noexcept { return id == parent; }
};

// A mating pair of cylinders forming one unit of main distribution. The two
// cylinders are coupled (mated) at a node and rise together in Z, gracing a
// sphere of known moral symmetry that caps the pair.
struct CylinderPair {
    std::uint32_t node = 0;   // the node this pair belongs to
    Vec3d a{};                // base of cylinder A
    Vec3d b{};                // base of cylinder B (mated to A)
    double radius = 1.0;      // shared cylinder radius
    double height = 1.0;      // shared cylinder height (Z extent)

    // The sphere of known moral symmetry the pair rises toward and graces. It
    // is centered above the mated pair; its symmetry (1 = perfectly symmetric)
    // reflects the model's positive, benevolent civic orientation.
    Vec3d sphere_center{};    // center of the sphere (above the pair, in Z)
    double sphere_radius = 0.0;
    double sphere_symmetry = 1.0;  // 0..1; higher = more morally symmetric
};

// The general cityscape graph.
class CityscapeModel {
public:
    CityscapeModel() = default;

    // Radix / diameter / randomness -- the three general dimensions.
    std::uint32_t radix() const noexcept { return radix_; }
    double diameter() const noexcept { return diameter_; }
    double randomness() const noexcept { return randomness_; }

    // The city-design parameters (Year / IQ / Legislature / quality / finality)
    // this model composes with.
    const CityParams& params() const noexcept { return params_; }

    const std::vector<Node>& nodes() const noexcept { return nodes_; }
    const std::vector<CylinderPair>& cylinders() const noexcept { return cylinders_; }

    // The model's civic orientation is positive/benevolent: the cylinders grace
    // spheres of known moral symmetry, oriented toward the common good. This is
    // a fixed, positive stance of the general model (a good thing), not a
    // tunable weapon or an adversarial parameter.
    static constexpr const char* orientation() noexcept {
        return "positive: cylinders grace spheres of known moral symmetry "
               "(benevolent, toward the common good)";
    }
    // Mean sphere symmetry across the graph (0..1); higher = more symmetric.
    double moralSymmetry() const noexcept;

    // Total edges (adjacencies) noted across the graph (each undirected edge is
    // counted once).
    std::size_t edgeCount() const noexcept;

    // Build the graph from the center of centricity outward. `radix` is the
    // branching base, `diameter` the radial reach, `randomness` (0..1) the
    // seeded variability. `params` supplies the Year/IQ/Legislature drivers,
    // which (with randomness) bias branching regularity, reach, and column
    // heights. Reproducible for a given (seed, radix, diameter, randomness,
    // params). `centerX/centerY` locate the center of centricity in block
    // coordinates.
    void build(std::uint64_t seed, std::uint32_t radix, double diameter,
               double randomness, const CityParams& params,
               double centerX, double centerY);

    // Convenience: build using a Config's params + the model dimensions and a
    // grid centre derived from cols/rows.
    void build(std::uint64_t seed, std::uint32_t radix, double diameter,
               double randomness, const CityParams& params,
               std::uint32_t cols, std::uint32_t rows) {
        build(seed, radix, diameter, randomness, params,
              (cols - 1) / 2.0, (rows - 1) / 2.0);
    }

private:
    // A node celebrates (activates + emits) then notes its neighbors.
    void celebrate(Node& n);
    void note(Node& n, std::uint32_t neighborId);

    std::uint32_t radix_ = 3;
    double diameter_ = 24.0;
    double randomness_ = 0.35;
    CityParams params_{};
    std::vector<Node> nodes_;
    std::vector<CylinderPair> cylinders_;
};

} // namespace sleela::city
