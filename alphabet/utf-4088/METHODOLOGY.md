# UTF-4088 Five-Input Alphabet Probe — Methodology

Status: executed reference prototype. The five inputs and scores are curated demonstrations; this is not a verified native UTF-4088 probe engine.

## Input definition

Each input records normalized map location x/y, Z-layer, depth iteration 1–50, normalized synthetic voltage, logical sample time, and nine provisional semantic profile scores. Inputs follow the traversal pattern: left-to-right, next row, then a new Z-layer; pressure iterations are 1, 12, 25, 38, and 50.

The four-dimensional matching vector is exactly (x, y, normalized_depth, normalized_voltage), with normalized_depth = (pressure_iteration - 1) / 49. Z-layer is traversal metadata and logical time is sample-time metadata; neither is silently added as a fifth dimension. The voltage value is normalized prototype data, not an electrical reading. The depth is an algorithmic iteration, not physical pressure.

## Curated reference terms

The nine English/Latin term anchors and their four-dimensional coordinates are stored in reference-terms.json. Their anchors and semantic signatures were hand-set to exercise the protocol; they are not empirical measurements, a trained model, or established UTF-4088 definitions. Latin terms such as definibilitas and eccentricitas are explicitly provisional technical wording where indicated.

## Ranking formulas

Let d be the Euclidean distance from a candidate's four-dimensional feature vector to a reference-term anchor.

- Geometry proximity: g = exp(-d / 0.25).
- Semantic proximity: s = 1 - mean(|candidate_axis - reference_axis|) over the nine declared scores.
- Composite proximity: 0.65g + 0.35s.
- Prototype merit score: (merit score + importance score) / 2.
- Composite ranking sorts by descending composite score, then smaller 4D distance, then stable term ID. A margin under 0.05 is marked ambiguous-provisional; this run does not establish a human meaning.

Time delta from each reference term's logical reference time is displayed as context only and does not affect the 4D or composite ranking. The 64-bit symbol key is calculated by a Python port of the deterministic mixing expression in utf-4088/character_map.cpp; equivalence against a compiled C++ run is not yet verified.

## Reproduce

From the repository root:

    python3 alphabet/utf-4088/run_inputs.py

The program reads inputs/input-1.json through input-5.json and reference-terms.json, then writes results/result-1 through result-5 in JSON, XML, and Markdown plus summary.json, summary.xml, and summary.md. It validates that all five result triplets exist and that the JSON/XML output parses.

## Interpretation limits

- “Known term” means a curated anchor in this test reference set, not an official UTF-4088 dictionary entry.
- Proximity values are scores from the formula above, not factual probabilities or proof of synonymy.
- English/Latin labels are proposed wordings based on the top two semantic-profile axes.
- The result key is not a Unicode scalar value, an established code point, or a published registry assignment.
- No physical voltage/pressure is applied, measured, or inferred.
