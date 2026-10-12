# UTF-4088 Probe Result 5

**Input:** Input 5 — next Z-layer, reset to top-left

**Execution:** executed (Python reference prototype)
**Match status:** provisional-leading-match

## Ordered candidate symbol

- **Ordinal:** 5 of 5
- **Symbol ID:** UTF4088-SYMBOL-000005
- **Candidate key:** 0xABE9098D80D3704E
- **Glyph:** 8 × 12 binary bitmap; not a Unicode character.

```text
#.#.#.##
###.#..#
....#..#
#...##.#
#.......
##.#..##
.###....
.#..###.
...#.###
...#.#.#
.##.....
.###.#.#
```

## Input and coordinate values

| Field | Value |
|---|---:|
| x | 0.050000 |
| y | 0.050000 |
| z-layer (traversal metadata) | 1 |
| pressure/depth iteration | 50 / 50 |
| normalized depth | 1.000000 |
| normalized voltage (synthetic) | 0.870000 |
| logical time (seconds) | 5.00 |
| delta from previous input (seconds) | 1.25 |
| 4D feature vector | x, y, normalized depth, normalized voltage |

## Wording and ranking

- **Anglophone wording:** eccentric distance
- **Latin wording:** distantia eccentrica
- **Dominant axes:** eccentricity, distance
- **Prototype merit score:** 0.320

## Proximity to curated reference terms

| Rank | Reference term | English | Latin | 4D distance | Geometry proximity | Semantic proximity | Composite proximity | Time delta (s, informational) |
|---:|---|---|---|---:|---:|---:|---:|---:|
| 1 | eccentricity (TERM-ECCENTRICITAS) | eccentricity | eccentricitas | 0.090554 | 0.696132 | 0.983333 | 0.796653 | 0.20 |
| 2 | distance (TERM-DISTANTIA) | distance | distantia | 1.247357 | 0.006810 | 0.864444 | 0.306982 | 1.00 |
| 3 | space (TERM-SPATIUM) | space | spatium | 1.220778 | 0.007573 | 0.823333 | 0.293089 | 5.00 |
| 4 | importance (TERM-MOMENTUM) | importance | momentum | 0.646452 | 0.075335 | 0.631111 | 0.269857 | 1.20 |
| 5 | relation (TERM-RELATIO) | relation | relatio | 0.753591 | 0.049077 | 0.654444 | 0.260956 | 3.00 |

## Interpretation and limits

Coordinates, weights, and reference anchors are curated demonstration inputs. Proximity is a numeric result of this declared prototype, not proof that a glyph has an established natural-language meaning.

- Depth is an abstract iteration, not physical pressure. Normalized voltage is synthetic, not measured volts.
- Logical time and z-layer are recorded but excluded from the 4D matching metric.
- English/Latin labels are provisional and are not validated word origins.
- Candidate key and bitmap are experimental outputs, not published UTF-4088 code points.
- C++ equivalence and semantic validity have not been verified.

