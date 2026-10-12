# UTF-4088 Probe Result 4

**Input:** Input 4 — layer zero, continue right

**Execution:** executed (Python reference prototype)
**Match status:** provisional-leading-match

## Input and coordinate values

| Field | Value |
|---|---:|
| x | 0.300000 |
| y | 0.550000 |
| z-layer (traversal metadata) | 0 |
| pressure/depth iteration | 38 / 50 |
| normalized depth | 0.755102 |
| normalized voltage (synthetic) | 0.720000 |
| logical time (seconds) | 3.75 |
| delta from previous input (seconds) | 1.25 |
| 4D feature vector | x, y, normalized depth, normalized voltage |

## Candidate symbol and wording

- Derived symbol key: 0x120C1C41A2380D33 (64-bit key; not a Unicode code point)
- Anglophone wording: merit and importance
- Latin wording: meritum et momentum
- Dominant axes: merit, importance
- Prototype merit score: 0.910

## Proximity to curated reference terms

| Rank | Reference term | English | Latin | 4D distance | Geometry proximity | Semantic proximity | Composite proximity | Time delta (s, informational) |
|---:|---|---|---|---:|---:|---:|---:|---:|
| 1 | merit (TERM-MERITUM) | merit | meritum | 0.060233 | 0.785894 | 0.927778 | 0.835553 | 0.25 |
| 2 | importance (TERM-MOMENTUM) | importance | momentum | 0.144623 | 0.560743 | 0.920000 | 0.686483 | 0.05 |
| 3 | cause (TERM-CAUSA) | cause | causa | 0.340254 | 0.256400 | 0.766667 | 0.434993 | 1.25 |
| 4 | relation (TERM-RELATIO) | relation | relatio | 0.357739 | 0.239080 | 0.732222 | 0.411680 | 1.75 |
| 5 | definability (TERM-DEFINIBILITAS) | definability | definibilitas | 0.397728 | 0.203740 | 0.771111 | 0.402320 | 0.95 |

Nearest geometric anchor: merit, 4D distance 0.060233.
Nearest composite anchor: merit, margin over runner-up 0.149070.

## Interpretation and limits

Coordinates, weights, and reference anchors are curated demonstration inputs. Proximity is a numeric result of this declared prototype, not proof that a glyph has an established natural-language meaning.

- Depth is an abstract iteration, not physical pressure. Normalized voltage is synthetic, not measured volts.
- Logical time and z-layer are recorded but excluded from the 4D matching metric.
- Latin/English labels are provisional. Definibilitas is a Latinized technical coinage, not claimed as classical Latin.
- The key is not a Unicode scalar or registered UTF-4088 code point.
- C++ equivalence and semantic validity have not been verified.
