# UTF-4088 Experimental Symbol Catalogue — 4D Study

**Source-code realm:** `alphabet/utf-4088`  
**Status:** editable review catalogue; five generated candidates currently recorded.

This is the human-review surface for the symbols produced by the five-input four-dimensional (4D) trial. Each record brings together stable identity, glyph shape, input context, nearest curated reference anchor, and provisional wording. The labels are not validated linguistic meanings.

## Model and status

The matching vector is `(x, y, normalized_depth, normalized_voltage)`, with `normalized_depth = (pressure_iteration - 1) / 49`. Z-layer and logical sample time are metadata, not extra dimensions. Pressure iteration (1–50) is an algorithmic depth value, not physical pressure; voltage is synthetic normalized data, not a hardware measurement.

UTF-4088 is experimental and is not a Unicode encoding. The candidate keys below are internal 64-bit identifiers, not Unicode scalar values or code points. Curated anchor scores are prototype scores, not probabilities or proof of meaning.

## Current symbol register

| Order | Input | Stable symbol ID | Candidate key | 4D vector `(x,y,d,v)` | Nearest anchor | Score | Provisional English | Provisional Latin |
|---:|---|---|---|---|---|---:|---|---|
| 1 | `input-1.json` | `UTF4088-SYMBOL-000001` | `0xAFA5577333931820` | `(0.05, 0.05, 0.000000, 0.12)` | space | 0.866672 | spatial distance | distantia spatialis |
| 2 | `input-2.json` | `UTF4088-SYMBOL-000002` | `0x3A8AF7C9593A783F` | `(0.30, 0.05, 0.224490, 0.30)` | time | 0.866057 | temporal relation | relatio temporalis |
| 3 | `input-3.json` | `UTF4088-SYMBOL-000003` | `0xFD6497A9A475933E` | `(0.05, 0.55, 0.489796, 0.55)` | cause | 0.831129 | causal definability | definibilitas causalis |
| 4 | `input-4.json` | `UTF4088-SYMBOL-000004` | `0x120C1C41A2380D33` | `(0.30, 0.55, 0.755102, 0.72)` | merit | 0.835553 | merit and importance | meritum et momentum |
| 5 | `input-5.json` | `UTF4088-SYMBOL-000005` | `0xABE9098D80D3704E` | `(0.05, 0.05, 1.000000, 0.87)` | eccentricity | 0.796653 | eccentric distance | distantia eccentrica |

## Candidate glyphs

Each glyph is an experimental 8-column × 12-row bitmap. `#` means set; `.` means unset. These are generated bitmaps, not Unicode characters.

### 1. UTF4088-SYMBOL-000001 — spatial distance

Candidate key: `0xAFA5577333931820`

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


### 2. UTF4088-SYMBOL-000002 — temporal relation

Candidate key: `0x3A8AF7C9593A783F`

```text
..###.#.
#...#.#.
####.###
##..#..#
.#.##..#
..###.#.
.####...
..######
..####..
.##.###.
####..##
.##...#.
```


### 3. UTF4088-SYMBOL-000003 — causal definability

Candidate key: `0xFD6497A9A475933E`

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


### 4. UTF4088-SYMBOL-000004 — merit and importance

Candidate key: `0x120C1C41A2380D33`

```text
...#..#.
....##..
...###..
.#.....#
#.#...#.
..###...
....##.#
..##..##
.####...
##.###.#
###..##.
##...#..
```


### 5. UTF4088-SYMBOL-000005 — eccentric distance

Candidate key: `0xABE9098D80D3704E`

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

## Source-code realm: editing rules

This document is an annotation and review layer; the generated machine-readable files remain the record of what the runner emitted.

- **Editable here:** proposed English/Latin wording, curator notes, review status, source citations, and semantic relations. Mark uncertain wording as provisional.
- **Change an input:** edit the corresponding file under `inputs/`, run `python3 alphabet/utf-4088/run_inputs.py` from the repository root, then review all per-input results, summaries, and ordered symbols.
- **Change glyph/key generation:** modify and test the runner as a versioned behavior change. Do not hand-edit a generated bitmap while leaving the source key or input unchanged.
- **Add a symbol:** add an input fixture, rerun the generator, verify that symbol IDs, candidate keys, and 8×12 bitmaps remain unique, then add the resulting identity and glyph here.
- **Change identity:** document why the identity changes and cross-reference its prior ID/key. Do not recycle a retired ID for an unrelated symbol.
- **Promote a label:** record its evidence and review status. A high nearest-anchor score is not sufficient by itself.

## Pressure transitions and identity

The existing candidate key does not independently fingerprint pressure change, pressure rate, logical-time delta, or the sudden-pressure flag. Different paths to similar states may therefore fail to receive distinct identities solely because of their path. If transition history must affect identity, add a separately versioned event/transition fingerprint and tests rather than silently changing the existing key contract.

The `abs(Δp) >= 12` sudden-pressure rule is configurable for this trial only—not a physical law or universal UTF-4088 constant. See [RIGOROUS.INPUTS.md](RIGOROUS.INPUTS.md) for the full input mapping and [METHODOLOGY.md](METHODOLOGY.md) for scoring limitations.

## Machine-readable sources

- [Ordered symbol register — JSON](results/symbols-ordered.json)
- [Ordered symbol register — XML](results/symbols-ordered.xml)
- [Ordered symbol register — Markdown](results/symbols-ordered.md)
- [Per-input results](results/)
- [Five-input mapping](RIGOROUS.INPUTS.md)

The register preserves input order, not proximity-score order. Semantic labels remain proposals until independently reviewed and documented.
