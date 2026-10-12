# UTF-4088 Probe Result 3

**Input:** Input 3 — layer zero, next row

**Execution:** executed (Python reference prototype)
**Match status:** provisional-leading-match

## Ordered candidate symbol

- **Ordinal:** 3 of 5
- **Symbol ID:** UTF4088-SYMBOL-000003
- **Candidate key:** 0xFD6497A9A475933E
- **Glyph:** 8 × 12 binary bitmap; not a Unicode character.

```text
######.#
.##..#..
#..#.###
#.#.#..#
#.#..#..
.###.#.#
#..#..##
..#####.
##.##.#.
#.#..##.
.##.##.#
...#..##
```

## Input and coordinate values

| Field | Value |
|---|---:|
| x | 0.050000 |
| y | 0.550000 |
| z-layer (traversal metadata) | 0 |
| pressure/depth iteration | 25 / 50 |
| normalized depth | 0.489796 |
| normalized voltage (synthetic) | 0.550000 |
| logical time (seconds) | 2.50 |
| delta from previous input (seconds) | 1.25 |
| 4D feature vector | x, y, normalized depth, normalized voltage |

## Wording and ranking

- **Anglophone wording:** causal definability
- **Latin wording:** definibilitas causalis
- **Dominant axes:** cause, definability
- **Prototype merit score:** 0.450

## Proximity to curated reference terms

| Rank | Reference term | English | Latin | 4D distance | Geometry proximity | Semantic proximity | Composite proximity | Time delta (s, informational) |
|---:|---|---|---|---:|---:|---:|---:|---:|
| 1 | cause (TERM-CAUSA) | cause | causa | 0.072196 | 0.749174 | 0.983333 | 0.831129 | 0.00 |
| 2 | relation (TERM-RELATIO) | relation | relatio | 0.310490 | 0.288817 | 0.833333 | 0.479398 | 0.50 |
| 3 | definability (TERM-DEFINIBILITAS) | definability | definibilitas | 0.535242 | 0.117541 | 0.923333 | 0.399568 | 0.30 |
| 4 | merit (TERM-MERITUM) | merit | meritum | 0.427086 | 0.181166 | 0.802222 | 0.398536 | 1.00 |
| 5 | importance (TERM-MOMENTUM) | importance | momentum | 0.526618 | 0.121666 | 0.812222 | 0.363361 | 1.30 |

## Interpretation and limits

Coordinates, weights, and reference anchors are curated demonstration inputs. Proximity is a numeric result of this declared prototype, not proof that a glyph has an established natural-language meaning.

- Depth is an abstract iteration, not physical pressure. Normalized voltage is synthetic, not measured volts.
- Logical time and z-layer are recorded but excluded from the 4D matching metric.
- English/Latin labels are provisional and are not validated word origins.
- Candidate key and bitmap are experimental outputs, not published UTF-4088 code points.
- C++ equivalence and semantic validity have not been verified.

