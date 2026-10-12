# UTF-4088 Alphabet Inputs and Probe Results

This directory contains a reproducible five-input curation run for the experimental UTF-4088 alphabet definition space.

## Contents

- inputs/input-1.json through input-5.json — the five source inputs.
- reference-terms.json — nine curated English/Latin reference-term anchors.
- run_inputs.py — standard-library Python reference runner.
- results/result-1.json/.xml/.md through result-5.json/.xml/.md — complete per-input results and ranked proximity tables.
- results/summary.json, summary.xml, summary.md — combined results table in all three formats.
- results/symbols-ordered.json, symbols-ordered.xml, symbols-ordered.md — unique candidate glyphs in input order 1–5.
- METHODOLOGY.md — input, feature-space, scoring, wording, and reproducibility contract.

## Run

From the repository root, run:

    python3 alphabet/utf-4088/run_inputs.py

The prototype validates inputs, computes four-dimensional distances, semantic profile proximity, composite rankings, derived 64-bit candidate keys, unique 8×12 candidate glyphs, provisional English/Latin wordings, and emits all per-input and summary documents.

## Important boundaries

This is an executed Python reference prototype, not a claim that the native C++ probe engine has been run or independently matched bit-for-bit. All source inputs and semantic profiles are manually curated demonstration values. The English and Latin wordings are suggestions only. Depth is an abstract iteration level, normalized voltage is synthetic, time is metadata, and the result keys are not Unicode scalar values or published UTF-4088 code points.

The canonical UTF-4088 specification set remains under [../../utf-4088/](../../utf-4088/README.md). The general alphabet documentation links to this run via [../sleela/README.md](../sleela/README.md).

- [RIGOROUS.INPUTS.md](RIGOROUS.INPUTS.md) — exact five-input application coordinates, 4D points and step lengths, pressure/voltage values, logical time, and the trial-specific sudden-pressure rule.
