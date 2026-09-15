# The Cityscape Model — the general model

`CityscapeModel` (`cityscape_model.hpp/.cpp`) is the **general model Sleela works
from**. It unifies everything City 3D knows — the **Year / IQ / Legislature**
drivers, building quality, and finality — underneath a **graph in the 3rd
graphing dimension** (Z / up), and adds three tunable dimensions with seeded
variability: **radix**, **diameter**, and **randomness**.

## The graph

- **Center of centricity.** A single root node — the basic unit of centricity —
  anchors the graph at the city's center.
- **radix** — the branching *base* of the graph: how many distribution lines
  fan out from each node. Larger radix ⇒ more nodes / a denser distribution.
- **diameter** — the distance a node may reach *out from the center of
  centricity*; the radial extent of the main distribution. No node exceeds it.
- **randomness** — a `0..1` seeded variability that perturbs the branching
  factor, radial reach, angles, and column sizes. Reproducible for a given
  `(seed, radix, diameter, randomness, params)`; higher randomness ⇒ more
  organic variation.

The graph is grown breadth-first from the center outward, one radial step
(≈ `diameter / radix`) per depth level, until the diameter is reached.

### Each node celebrates, then notes its neighbors

When a node is placed it **celebrates** — it activates and is emitted as a live
node of the main distribution (`celebrate()`), and the render emphasizes it.
It then **notes** each adjacent node into an explicit neighbor list
(`note()`), and the adjacency is mutual (the parent notes the child back). So
the model carries who-is-next-to-whom: `edgeCount()` reports the number of noted
undirected edges (a tree, so `edges == nodes − 1`).

## Main distribution: mating cylinder pairs

The main distribution is realized in the 3rd dimension as **mating pairs of
cylinders**. At each node a coupled pair of vertical cylinders (columns) rises
together in Z, offset perpendicular to the node's radial direction so the two
mated columns straddle the distribution line. Column height scales with
modernity (Year) and design quality (IQ); radius tapers with distance from the
center.

### Spheres of known moral symmetry

Each mating pair **graces a sphere of known moral symmetry** that caps the pair
above its midpoint in Z. The sphere's `sphere_symmetry` (0..1) is high and
steadies with more order / design quality — expressing the model's fixed
**positive, benevolent civic orientation** (a good thing, toward the common
good). `moralSymmetry()` reports the mean across the graph, and
`orientation()` states the positive stance. This is a fixed, positive property
of the general model — not a tunable weapon or an adversarial parameter.

## Composing the drivers

The graph is not separate from the Year / IQ / Legislature model — it composes
with it:

- **Legislature** — `axis_bias` tilts the branching fan toward the dominant
  axis and `linearity` narrows the spread, so the distribution graph inherits
  the regime's lines and linear outcomes.
- **IQ (design quality) + regularity** — raise the graph's *order*, reducing the
  jitter applied to branching and reach (a better-planned distribution).
- **Year (modernity)** — lifts the cylinder columns and sphere presence.

The resulting graph is then rasterized onto the City grid through Phraign: the
cylinder pairs and spheres are drawn as columns and caps rising above the
building field, with the noted neighbor links drawn as ground ribbons.

## Configuration

Set from the config file (see [`CONFIG.md`](CONFIG.md)):

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `radix` | int ≥ 1 | `3` | Branching base of the distribution graph |
| `diameter` | float > 0 | `24` | Radial reach from the center of centricity (blocks) |
| `randomness` | float 0..1 | `0.35` | Seeded variability over the graph + drivers |
| `draw_cylinders` | bool | `true` | Render the mating cylinder pairs + neighbor links |

## Serialization

The general dimensions travel with a saved city: PHRAIGN-CITY **v4** records
`radix`, `diameter`, and `randomness` (alongside `year`, `iq`, `legislature`).
The graph — nodes, mating cylinder pairs, and spheres — is **regenerated
deterministically** from `(seed, year, iq, legislature, radix, diameter,
randomness)` on load, so it is reproduced exactly without dumping every node.
See [`MODEL_FORMAT.md`](MODEL_FORMAT.md).

## API sketch

```cpp
sleela::city::CityscapeModel model;
model.build(seed, radix, diameter, randomness, params, cols, rows);

model.nodes();        // graph nodes (each celebrated, with noted neighbors)
model.cylinders();    // mating cylinder pairs, each gracing a sphere
model.edgeCount();    // noted undirected adjacencies
model.moralSymmetry();// mean sphere symmetry (0..1), positive orientation
CityscapeModel::orientation();  // the fixed positive/benevolent stance
```
