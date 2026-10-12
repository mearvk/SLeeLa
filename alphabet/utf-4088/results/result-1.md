# UTF-4088 Probe Result 1

**Input:** Input 1 — layer zero, top-left

**Execution:** executed (Python reference prototype)
**Match status:** provisional-leading-match

## Ordered candidate symbol

- **Ordinal:** 1 of 5
- **Symbol ID:** UTF4088-SYMBOL-000001
- **Candidate key:** 0xAFA5577333931820
- **Glyph:** 8 × 12 binary bitmap; not a Unicode character.

```text
#.#.####
#.#..#.#
.#.#.###
.###..##
..##..##
#..#..##
...##...
..#.....
#..####.
..##.###
.####..#
#.##...#
```

## Input and coordinate values

| Field | Value |
|---|---:|
| x | 0.050000 |
| y | 0.050000 |
| z-layer (traversal metadata) | 0 |
| pressure/depth iteration | 1 / 50 |
| normalized depth | 0.000000 |
| normalized voltage (synthetic) | 0.120000 |
| logical time (seconds) | 0.00 |
| delta from previous input (seconds) | — |
| 4D feature vector | x, y, normalized depth, normalized voltage |

## Wording and ranking

- **Anglophone wording:** spatial distance
- **Latin wording:** distantia spatialis
- **Dominant axes:** space, distance
- **Prototype merit score:** 0.250

## Proximity to curated reference terms

| Rank | Reference term | English | Latin | 4D distance | Geometry proximity | Semantic proximity | Composite proximity | Time delta (s, informational) |
|---:|---|---|---|---:|---:|---:|---:|---:|
| 1 | space (TERM-SPATIUM) | space | spatium | 0.052915 | 0.809240 | 0.973333 | 0.866672 | 0.00 |
| 2 | time (TERM-TEMPUS) | time | tempus | 0.424735 | 0.182877 | 0.648889 | 0.345981 | 1.00 |
| 3 | distance (TERM-DISTANTIA) | distance | distantia | 1.512746 | 0.002356 | 0.861111 | 0.302920 | 4.00 |
| 4 | eccentricity (TERM-ECCENTRICITAS) | eccentricity | eccentricitas | 1.206524 | 0.008018 | 0.813333 | 0.289878 | 5.20 |
| 5 | relation (TERM-RELATIO) | relation | relatio | 0.778075 | 0.044499 | 0.631111 | 0.249813 | 2.00 |

## Interpretation and limits

Coordinates, weights, and reference anchors are curated demonstration inputs. Proximity is a numeric result of this declared prototype, not proof that a glyph has an established natural-language meaning.

- Depth is an abstract iteration, not physical pressure. Normalized voltage is synthetic, not measured volts.
- Logical time and z-layer are recorded but excluded from the 4D matching metric.
- English/Latin labels are provisional and are not validated word origins.
- Candidate key and bitmap are experimental outputs, not published UTF-4088 code points.
- C++ equivalence and semantic validity have not been verified.

