# UTF-4088 Probe Ordering and Match-Driven Alphabet Generation

Status: proposed experimental algorithm, version 1.

## 1. Goal

Define a deterministic process that orders candidate input probes, compares their results with known registry entries, and produces an evidence-aware candidate alphabet. The algorithm must optimize exploration logistics without confusing probe priority with the final published alphabetic order.

UTF-4088 is an experimental catalogue/identifier namespace, not a standard Unicode encoding. A glyph's visual proximity to another glyph does not, by itself, prove semantic or linguistic equivalence.

## 2. Keep four orderings separate

Each candidate may participate in four independent orders:

1. **Spatial traversal order** — the canonical input map order: top-left, left-to-right, top-to-bottom, then next z-layer.
2. **Probe priority order** — which candidate/depth to evaluate next based on expected usefulness and cost.
3. **Match-neighbor order** — candidates ranked by a declared feature-distance metric against known reference entries.
4. **Published alphabetic order** — stable subject collation used by the National Lien Alphabetic Register.

Never use one of these orders as a substitute for another. Permanent catalogue IDs remain immutable; alphabetic rank is a generated index.

## 3. Candidate and probe model

A probe request should identify:

- `candidate_id`: stable ID or deterministic candidate key;
- `x`, `y`, `z`: input-map coordinate;
- `depth`: probing depth/iteration, from 1 through the assumed maximum 50;
- `features`: versioned, measurable glyph/input features;
- `reference_set_version`: exact known-match set used for comparison;
- `seed`: deterministic seed when candidate sampling is stochastic;
- `cost_estimate`: expected CPU, memory, and I/O budget;
- `status`: untested, candidate, provisional, verified, rejected, or deferred.

Depth 1–50 is an algorithmic exploration scale, not physical pressure, electrical voltage, or semantic confidence. A deeper probe is not automatically a better match.

## 4. Match distance and evidence

Define a versioned feature distance (d(c,r)) between candidate (c) and reference record (r). It may combine normalized bitmap difference, structural features, explicitly defined graph relationships, and metadata compatibility. Each component must document its range and weight. Missing features must be marked missing, not silently treated as zero.

For each candidate, record:

- nearest reference ID and measured distance;
- runner-up reference and distance;
- distance margin between the top two matches;
- features used and their version;
- evidence source and confidence/status;
- whether the match is exact, approximate, ambiguous, or unmatched.

A visual/structural match may suggest a candidate relationship but cannot establish the same linguistic meaning without independent annotation or provenance.

## 5. Probe priority and logistics

Use a deterministic priority score only to decide which probe to run next. A suggested baseline is:

`priority(c,d) = (w_m M + w_n N + w_u U) / (1 + w_c C + w_r R)`

where all normalized components are in [0,1]:

- `M`: expected match utility from prior evidence;
- `N`: novelty/coverage contribution;
- `U`: uncertainty reduction expected from the probe;
- `C`: estimated compute cost;
- `R`: resource-risk penalty (memory, I/O, or time budget);
- `w_m, w_n, w_u, w_c, w_r`: explicit, versioned non-negative weights.

This score is a scheduling heuristic, not a truth score. Document the chosen weights in the run manifest. For an initial neutral profile, set all weights to 1 and compare the resulting candidate order against the canonical spatial traversal baseline. Never drop a candidate solely because its heuristic score is low; allow a deterministic coverage pass so that prioritization does not starve regions or depth levels.

Tie-break in this exact order: higher priority score, lower probe depth, lower z, lower y, lower x, then lexicographically smaller canonical candidate ID. Floating-point scores should be quantized to a documented fixed precision before tie-breaking.

## 6. Bounded depth and stopping rules

For each map element, process depth levels 1–50 in ascending order unless an explicitly named adaptive mode is selected. An adaptive mode may prioritize likely useful depths, but must retain a coverage schedule and record skipped/deferred depths. Stop a probe when any configured limit is reached: maximum depth, candidate budget, elapsed time, memory budget, or completed work budget. Limit exhaustion is not a match failure.

The implementation must validate dimensions, overflow, depth bounds, finite numeric values, and the total work budget before starting. A probe run must be reproducible from its manifest and inputs.

## 7. Generation pipeline

1. Load the versioned input map and reference registry without executing untrusted source files.
2. Validate map bounds, candidate IDs, feature schema, depth range, and resource budgets.
3. Generate candidate probes for the declared coordinate/depth domain.
4. Apply the configured priority queue and deterministic tie-break rules.
5. Evaluate candidates against a bounded nearest-match index or exhaustive reference scan for small test fixtures.
6. Record top matches, distance margins, cost, depth, and evidence status.
7. Run a coverage pass over remaining candidate/depth regions.
8. Deduplicate only under an explicit identity rule; do not merge records merely because their bitmaps or distances are similar.
9. Emit an auditable candidate manifest.
10. Generate National Lien alphabetic rank from verified/provisional subject metadata using a separately versioned collation profile.

## 8. Output manifest

Recommended fields:

`run_id,algorithm_version,map_version,reference_set_version,feature_schema_version,seed,traversal_mode,priority_profile,work_budget,probes_completed,candidate_id,x,y,z,depth,nearest_match_id,nearest_distance,runner_up_id,runner_up_distance,match_status,subject_key,alphabetic_rank,cost_units,evidence_ref`

Keep run-level metadata in a manifest header or companion JSON. If a field is unavailable, represent it explicitly as null/unknown; do not fabricate a match or subject.

## 9. Validation and metrics

Tests should cover:

- identical inputs and configuration produce the same ordered probe trace;
- depth 1 and 50 are accepted, while 0 and 51 are rejected;
- each coordinate/depth pair is visited at most once per run unless retries are explicitly logged;
- priority ties use the documented tie-break order;
- exact matches outrank non-exact matches only under the selected metric;
- ambiguous nearest-neighbor results are marked ambiguous;
- a candidate with no match remains unmatched rather than being forced to a known symbol;
- resource-budget exhaustion is reported distinctly from rejection;
- the coverage pass prevents indefinite starvation of low-priority regions;
- final alphabetic rank is stable for a fixed dataset and collation profile;
- changes to weights, feature metrics, reference sets, or collation profiles change version metadata.

Measure candidate yield, exact/approximate match rate, ambiguity rate, probes per accepted candidate, cost per probe, memory peak, coverage by coordinate and depth, and time to first verified candidate. Do not optimize a single match-rate metric at the expense of coverage or reproducibility.

## 10. Relationship to companion documents

- [Input Map and Pressure Iteration Protocol](INPUT-MAP-AND-PRESSURE.md): spatial coordinates and depth iteration.
- [National Lien Alphabetic Register](NATIONAL-LIEN-REGISTER.md): permanent IDs and published subject order.
- [Code-point and Serialization Contract](CODE-POINT-AND-SERIALIZATION.md): ID bounds and serialization.
- [SLeeLa Alphabet Specification](../alphabet/sleela/SPECIFICATION.md): general character-set terminology and implementation caveats.

This document defines a proposed algorithm. It does not claim that a production probe engine, proximity index, semantic validator, or complete 16,606-entry verified alphabet already exists.
