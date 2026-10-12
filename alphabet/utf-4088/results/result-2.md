# UTF-4088 Probe Result 2

**Input:** Input 2 — layer zero, move right

**Execution:** executed (Python reference prototype)
**Match status:** provisional-leading-match

## Input and coordinate values

| Field | Value |
|---|---:|
| x | 0.300000 |
| y | 0.050000 |
| z-layer (traversal metadata) | 0 |
| pressure/depth iteration | 12 / 50 |
| normalized depth | 0.224490 |
| normalized voltage (synthetic) | 0.300000 |
| logical time (seconds) | 1.25 |
| delta from previous input (seconds) | 1.25 |
| 4D feature vector | x, y, normalized depth, normalized voltage |

## Candidate symbol and wording

- Derived symbol key: 0x3A8AF7C9593A783F (64-bit key; not a Unicode code point)
- Anglophone wording: temporal relation
- Latin wording: relatio temporalis
- Dominant axes: time, relation
- Prototype merit score: 0.360

## Proximity to curated reference terms

| Rank | Reference term | English | Latin | 4D distance | Geometry proximity | Semantic proximity | Composite proximity | Time delta (s, informational) |
|---:|---|---|---|---:|---:|---:|---:|---:|
| 1 | time (TERM-TEMPUS) | time | tempus | 0.053393 | 0.807695 | 0.974444 | 0.866057 | 0.25 |
| 2 | space (TERM-SPATIUM) | space | spatium | 0.340626 | 0.256019 | 0.665556 | 0.399357 | 1.25 |
| 3 | relation (TERM-RELATIO) | relation | relatio | 0.497198 | 0.136861 | 0.796667 | 0.367793 | 0.75 |
| 4 | cause (TERM-CAUSA) | cause | causa | 0.697586 | 0.061400 | 0.764444 | 0.307466 | 1.25 |
| 5 | definability (TERM-DEFINIBILITAS) | definability | definibilitas | 0.775084 | 0.045034 | 0.764444 | 0.296828 | 1.55 |

Nearest geometric anchor: time, 4D distance 0.053393.
Nearest composite anchor: time, margin over runner-up 0.466701.

## Interpretation and limits

Coordinates, weights, and reference anchors are curated demonstration inputs. Proximity is a numeric result of this declared prototype, not proof that a glyph has an established natural-language meaning.

- Depth is an abstract iteration, not physical pressure. Normalized voltage is synthetic, not measured volts.
- Logical time and z-layer are recorded but excluded from the 4D matching metric.
- Latin/English labels are provisional. Definibilitas is a Latinized technical coinage, not claimed as classical Latin.
- The key is not a Unicode scalar or registered UTF-4088 code point.
- C++ equivalence and semantic validity have not been verified.
