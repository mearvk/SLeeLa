# UTF-4088 Source-Level Symbols: 4D Geometry, Topology, and Coincidence

**Scope:** internal structures and mathematical symbols present in the UTF-4088 C/C++ source  
**Status:** implementation inventory; separates implemented behavior from interfaces, documented concepts, and unimplemented coincidence rules.

This is the source-code catalogue for the 4D study. It is **not** a list of the five trial labels such as “spatial distance” or “temporal relation.” Those five input-derived glyphs remain available in [the ordered trial output](results/symbols-ordered.md). This document instead describes the 4D state, derived geometric features, 8×12 glyph topology, registry identifiers, graph objects, and the different ways repeated states are coalesced in the implementation.

UTF-4088 is an experimental project-specific identifier namespace, not a Unicode encoding. Generated IDs and hashes do not by themselves establish a human-language meaning.

## 1. The four-dimensional state

The main source-level 4D point is:

```cpp
struct SymbolPoint {
    double x;
    double y;
    double pressure;
    double voltage;
};
```

Declared in [symbol_resolution.hpp](../../utf-4088/algebra/symbol_resolution.hpp). The neural relayer also declares `Field4D { x, y, pressure, voltage }` in [relayer.hpp](../../utf-4088/neural/relayer.hpp), while [character_map.hpp](../../utf-4088/character_map.hpp) exposes a remainder-symbol function accepting the same four scalar coordinates.

| Dimension | Source field | Role in this implementation |
|---|---|---|
| 1 | `x` | Horizontal/map coordinate |
| 2 | `y` | Vertical/map coordinate |
| 3 | `pressure` | Abstract pressure/depth parameter in the 4D address |
| 4 | `voltage` | Voltage or normalized field-state parameter, according to the calling layer |

The source-level resolver uses the raw `pressure` and `voltage` values supplied in each point. It does not apply the trial-run transformation `normalized_depth = (pressure_iteration - 1) / 49` itself. Callers should therefore agree on units, normalization, and bounds before comparing points; otherwise one axis may dominate the distance.

The separate map traversal uses `(x, y, z)` for grid location and processes a pressure/depth iteration at each map location. That traversal Z-layer is **metadata outside** the `SymbolPoint` four-vector; it must not silently be treated as one of the vector's four fields. See [input-map protocol](../../utf-4088/INPUT-MAP-AND-PRESSURE.md).

## 2. Geometry and path similarity

In [symbol_resolution.cpp](../../utf-4088/algebra/symbol_resolution.cpp), the Euclidean separation between two points is:

`d(a,b) = sqrt((Δx)^2 + (Δy)^2 + (Δpressure)^2 + (Δvoltage)^2)`

For paths, `path_error` averages the pointwise distances for corresponding list positions over the shorter input length. The shape score is:

`shape_similarity = 1 / (1 + path_error)`

The same resolver computes additional scores:

| Internal quantity | Formula / behavior | Intended role |
|---|---|---|
| `shape_similarity` | `1 / (1 + path_error)` | Agreement of aligned 4D points |
| `graph_continuity` | `1 / (1 + mean XY step length)` | Smoothness of the path in the x/y plane |
| `field_uniformity` | `1 / (1 + sqrt(voltage variance))` | Low variation in path voltage |
| `language_fit` | Currently hard-coded to `0.0` | Neutral placeholder until a language model is supplied |
| `total` | `0.40*shape + 0.25*continuity + 0.20*uniformity + 0.15*language_fit` | Combined resolver score |

**Interpretation limit:** a distance of zero would mean the compared numeric coordinates coincide exactly. The resolver does not define a tolerance for “near coincidence,” a geometric intersection solver, or a clustering algorithm. It also compares path elements by index rather than resampling or aligning paths by a dynamic-time-warping algorithm.

## 3. Four-dimensional address-to-symbol hash

[`derive_remainder_symbol(x, y, pressure, voltage)`](../../utf-4088/character_map.cpp) builds a deterministic 64-bit hash-like symbol identifier from all four coordinates.

- Each finite input is clamped to the implementation's `±1.0e9` bound and quantized at a scale of `1,000,000` using rounding.
- Non-finite values quantize to zero in this helper.
- The four quantized values are mixed with fixed constants and a SplitMix-style finalizer.
- Repeating the same four quantized inputs produces the same output.

This is an address-derived identifier, **not** a reversible encoding, rendered glyph, semantic label, or guarantee of collision-free identity. Different inputs may collide after quantization or hashing. The code does not return the internal intermediate geometry as a public symbol description.

The simpler public `InputState` / `CodePoint` API in [utf4088.hpp](../../utf-4088/utf4088.hpp) accepts an already-created integer input. [utf4088.cpp](../../utf-4088/utf4088.cpp) currently validates the experimental interval `0x110000 <= value <= 0x1FFFFFFFF`; it does not compute the 4D geometry or derive a point from electrical hardware.

## 4. Voltage-field geometry symbols

The voltage-field module derives local structure from 2D samples. It declares:

```cpp
struct VoltageSample {
    double x;
    double y;
    double volts;
    double time;
};

struct FieldState {
    double voltage;
    double dVdx;
    double dVdy;
    double magnitude;
    double direction;
    double uniformity;
};
```

Defined in [voltage_field.hpp](../../utf-4088/voltage_field.hpp), with calculations in [voltage_field.cpp](../../utf-4088/voltage_field.cpp).

| Field symbol | Current computation | Geometric interpretation |
|---|---|---|
| `voltage` | Center sample's voltage value | Field value at the sample |
| `dVdx` | Finite difference using nearest aligned left/right samples where available | Horizontal gradient |
| `dVdy` | Finite difference using nearest aligned down/up samples where available | Vertical gradient |
| `magnitude` | `hypot(dVdx, dVdy)` | Gradient strength |
| `direction` | `atan2(dVdy, dVdx)` | Gradient angle in radians |
| `uniformity` | `1 / (1 + sqrt(population variance))` across valid sample voltages | A field-uniformity score |

The `VoltageSample.time` field must be finite, but the current field analysis does not use it in the gradient, uniformity, or packed-state formulas. The field API turns the other derived values into a packed 64-bit `field_to_input` value. The documented bit allocation is 20 bits each for voltage, magnitude, and uniformity, plus 4 bits for direction. The packer quantizes absolute values and masks each field into its allotted bits. In particular, direction is currently reduced to the low four bits of the quantized absolute angle rather than a carefully defined circular-angle code. Consequently, different field states can produce the same packed input; that is an implementation-level encoding limitation, not evidence of a true geometric coincidence.

These sample values are software data. Any actual hardware sampling and electrical safety contract must remain in a separate adapter.

## 5. 8×12 glyph geometry and topology

The source glyph is a 96-bit bitmap stored as twelve 8-bit rows:

`Glyph8x12 { rows[12] }`

See [glyph8x12.hpp](../../utf-4088/algebra/glyph8x12.hpp) and [glyph8x12.cpp](../../utf-4088/algebra/glyph8x12.cpp). A set pixel is black/stroke; an unset pixel is background. The code analyzes orthogonal (up/down/left/right) adjacency—not diagonal adjacency—for connectivity.

| Metric / symbol | Exact source meaning |
|---|---|
| `black_pixels` | Number of set pixels among the 96 cells |
| `connected_components` | Number of separate 4-neighbor-connected groups of set pixels |
| `connected` | True for an empty bitmap or when there is exactly one component |
| `edge_count` | Number of adjacent set-pixel pairs counted horizontally and vertically (right/down once per pair) |
| `transitions` | Number of horizontal/vertical neighbor pairs with different pixel values |
| `signature` | 32-bit mixed fingerprint derived from row-major pixels, edge count, and transitions |
| `seed_glyph(seed)` | Deterministically creates a bitmap from a text seed |
| `glyph_signature(glyph)` | Returns the analyzer's 32-bit signature |

A `signature` is a fingerprint, not a unique identity guarantee: a 32-bit signature can collide, and the code does not use the signature alone as proof of identical geometry.

### Candidate filter used by the sampling programs

The source programs use explicit engineering filters to study potentially interpretable glyph topology:

- `interpretability_sampler.cpp`: 4-neighbor connected, 8–40 black pixels, and 8–90 transitions.
- `large_sampling_map.cpp`: the same conditions plus `edge_count > 0`.
- `space_sampler.cpp`: reports a separate, looser `connected + 4–48 black pixels` subset.

These are distinct sampling predicates, not one shared semantic definition. Passing a predicate establishes only geometric/topological eligibility; it does not establish linguistic meaning.

The glyph-resolution helper scores connectivity, topology, transitions, and density:

`score = 0.45*connectivity + 0.25*topology + 0.20*complexity + 0.10*(1 - abs(density - 0.30))`

with `density = black_pixels / 96`, `topology = min(1, edge_count / 95)`, and `complexity = min(1, transitions / 120)`. This score favors connected, structured glyphs but is not a validated language-recognition score.

## 6. Coincidence and coalescence symbols in the digraph

The current implemented coincidence behavior lives in [digraph_engine.hpp](../../utf-4088/digraph_engine.hpp) and [digraph_engine.cpp](../../utf-4088/digraph_engine.cpp).

| Graph symbol | Source fields | Behavior |
|---|---|---|
| `GraphNode` | `id`, `symbol_input`, `weight` | Represents a quantized field state in the concept graph |
| `GraphEdge` | `from`, `to`, `weight`, `direction` | Represents one directed transition between adjacent states |
| `ConceptGraph` | `nodes`, `edges` | Holds the node and directed-edge collections |
| `input_id(FieldState)` | Packed voltage, magnitude, uniformity, and direction values | Supplies the graph node identity |
| `EdgeKey(from,to)` | Ordered pair of node IDs | Supplies directed-edge identity |

The implementation processes a sequence of field states as follows:

1. Each state is quantized and packed into an `input_id`.
2. Repeated IDs coalesce into one `GraphNode`; its weight increments once for each occurrence.
3. Each consecutive pair of states contributes a directed `from → to` edge.
4. Repeated instances of the same ordered edge coalesce into one `GraphEdge`; its weight increments and its direction field is updated as a running average.
5. The reverse edge `to → from` is a different key. A repeated state can also produce a self-edge when it occurs consecutively.

In this module, a **coincidence** means identity after that module's quantization/packing step, or repetition of the same ordered transition. It does **not** mean the code found a physical intersection of trajectories or proved two original continuous field states were identical. Quantization and bit masking may merge distinct original values.

The test in [utf4088_tests.cpp](../../utf-4088/tests/utf4088_tests.cpp) verifies that a repeating two-state sequence produces two nodes and two directed edges, each with weight two.

## 7. Registry and language/stage symbols

The front-end registry uses the following C++ types from [character_map.hpp](../../utf-4088/character_map.hpp):

| Type / field | Meaning in the source |
|---|---|
| `CharacterRecord.integer_id` | Sequential record ID |
| `CharacterRecord.codepoint` | Experimental identifier value |
| `CharacterRecord.shape_id` | Deterministically derived shape reference ID |
| `CharacterRecord.meaning_id` | Deterministically derived meaning reference ID |
| `CharacterRecord.stage` | One of `Start`, `Intermediate`, `Final` |
| `CharacterRecord.language` | One of `AmericanEnglish`, `Korean`, `Germanic` |
| `Stage` | Three-step symbolic gradation |
| `Language` | Language-family slot used by this experimental front end |

[`registry_generator.cpp`](../../utf-4088/registry_generator.cpp) deterministically generates 16,606 records with `integer_id = 0..16605` and `codepoint = 0x110000 + integer_id`. It derives `shape_id` and `meaning_id` using salted SplitMix64-style hashes of the record ID. These hashes are stable references, not the actual bitmap, a populated semantic dictionary, or validation that the associated meaning exists. The implementation cycles stage and language enum values by record index.

The separate [semantic seed data](../../utf-4088/semantics/character_meanings.json) currently includes `A`, `한`, and `Ä` as illustrative cross-language seed records. These are language/semantic examples, not labels inferred from the 4D geometry. The [cross-language corpus builder](../../utf-4088/algebra/cross_language_corpus.cpp) derives seed-based bitmaps and stage variants for Latin alphabet seeds and Korean jamo seeds; it explicitly does not claim to reproduce historical type.

## 8. Neural relayer: declared 4D interface

[relayer.hpp](../../utf-4088/neural/relayer.hpp) declares `Field4D`, a 16-value hidden-state record, an `engram_hash`, ranked `CharacterCandidate` values, and a `RelayerResult`. The header declares `synthesize(...)` with caller-supplied weights and candidate IDs. In the inspected directory, this is an interface declaration; this catalogue does not treat the neural synthesis behavior as verified by the header alone.

## 9. Symbol inventory: implemented versus not yet implemented

| Capability | Source status |
|---|---|
| Four-coordinate point type `(x,y,pressure,voltage)` | Declared and used by the symbol resolver |
| Euclidean 4D point distance | Implemented in `symbol_resolution.cpp` |
| Pointwise path-comparison score | Implemented; compares positions by index |
| Deterministic four-coordinate remainder hash | Implemented in `character_map.cpp` |
| Voltage gradients, magnitude, direction, uniformity | Implemented in `voltage_field.cpp` |
| Binary 8×12 glyph topology metrics | Implemented in `glyph8x12.cpp` |
| Coalescing equal packed state IDs | Implemented in `digraph_engine.cpp` |
| Coalescing repeated directed edges | Implemented in `digraph_engine.cpp` |
| Configurable geometric near-coincidence tolerance | Not defined by the inspected C++ |
| Segment/curve/surface intersection solver | Not present in the inspected source set |
| Dedicated clustering of approximate 4D points | Not present in the inspected source set |
| Collision-free 64-bit or 32-bit fingerprint guarantee | Not provided |
| Meaning validation from geometry alone | Explicitly not provided |

## 10. Related source and generated data

- [4D symbol resolver header](../../utf-4088/algebra/symbol_resolution.hpp) and [implementation](../../utf-4088/algebra/symbol_resolution.cpp)
- [4D remainder hash](../../utf-4088/character_map.hpp) / [implementation](../../utf-4088/character_map.cpp)
- [Voltage-field API](../../utf-4088/voltage_field.hpp) / [implementation](../../utf-4088/voltage_field.cpp)
- [Digraph API](../../utf-4088/digraph_engine.hpp) / [implementation](../../utf-4088/digraph_engine.cpp)
- [Glyph geometry API](../../utf-4088/algebra/glyph8x12.hpp) / [implementation](../../utf-4088/algebra/glyph8x12.cpp)
- [Registry generation](../../utf-4088/registry_generator.cpp)
- [C++ regression tests](../../utf-4088/tests/utf4088_tests.cpp)
- [Five-input trial input map](RIGOROUS.INPUTS.md)
- [Generated trial glyphs](results/symbols-ordered.md)

The register is intentionally source-grounded. It describes actual data types, formulas, hash/packing behavior, topology metrics, and the graph coalescence mechanism while keeping unimplemented geometric coincidence detection clearly marked as future work.
