# UTF-4088 Five-Input Probe Results

Status: executed by Python reference prototype; native C++ equivalence is not verified and semantics have not been validated.

## Summary table

| # | Location (z, x, y) | Depth | Norm. voltage | Logical time (s) | English wording | Latin wording | Nearest 4D anchor | 4D distance | Nearest composite anchor | Composite score | Margin | Match status | Merit score |
|---:|---|---:|---:|---:|---|---|---|---:|---|---:|---:|---|---:|
| 1 | (0, 0.05, 0.05) | 1 | 0.12 | 0.00 | spatial distance | distantia spatialis | space | 0.052915 | space | 0.866672 | 0.520691 | provisional-leading-match | 0.250 |
| 2 | (0, 0.30, 0.05) | 12 | 0.30 | 1.25 | temporal relation | relatio temporalis | time | 0.053393 | time | 0.866057 | 0.466701 | provisional-leading-match | 0.360 |
| 3 | (0, 0.05, 0.55) | 25 | 0.55 | 2.50 | causal definability | definibilitas causalis | cause | 0.072196 | cause | 0.831129 | 0.351731 | provisional-leading-match | 0.450 |
| 4 | (0, 0.30, 0.55) | 38 | 0.72 | 3.75 | merit and importance | meritum et momentum | merit | 0.060233 | merit | 0.835553 | 0.149070 | provisional-leading-match | 0.910 |
| 5 | (1, 0.05, 0.05) | 50 | 0.87 | 5.00 | eccentric distance | distantia eccentrica | eccentricity | 0.090554 | eccentricity | 0.796653 | 0.489671 | provisional-leading-match | 0.320 |

## Algorithm

- Normalized depth: (iteration - 1) / 49.
- Four matching dimensions: x, y, normalized depth, normalized voltage. Z-layer and logical time are metadata, not matching dimensions.
- Geometry proximity: exp(-4D Euclidean distance / 0.25).
- Semantic proximity: one minus mean absolute difference across nine curated axes.
- Composite proximity: 0.65 × geometry proximity + 0.35 × semantic proximity.
- Time deltas to term anchors are informational only and do not alter rankings.

## Semantic axes

| Axis | Provisional interpretation |
|---|---|
| merit | merit relevance |
| time | temporal relevance |
| space | spatial relevance |
| importance | importance relevance |
| eccentricity | eccentricity/distinctiveness |
| cause | causal relevance |
| distance | spatial/conceptual separation |
| definability | how readily a concept can be specified |
| relation | relational relevance |

Scores are curated prototype weights, not measured properties, trained probabilities, or objective judgments of value.

## Limitations
- Reference-term anchors and semantic signatures are curated prototype values.
- Latin and English labels are provisional wordings, not validated philological output.
- Proximity scores are model outputs, not factual probabilities.
- Native C++ engine equivalence is not verified.

See result-1 through result-5 in JSON, XML, and Markdown for full ranked lists.
