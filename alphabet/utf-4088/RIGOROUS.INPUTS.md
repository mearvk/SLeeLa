# RIGOROUS.INPUTS.md — UTF-4088 Five-Input Trial Mapping

**Status:** experimental, reproducible input-mapping contract for trial inputs 1–5.
**Source fixtures:** `alphabet/utf-4088/inputs/input-1.json` through `input-5.json`.
**Ordered symbols:** [`results/symbols-ordered.json`](results/symbols-ordered.json), [`XML`](results/symbols-ordered.xml), and [`Markdown`](results/symbols-ordered.md).

## 1. Fixed four-dimensional definition

For input i, define its application point in the model as:

`q_i = (x_i, y_i, d_i, v_i)`

- `x` and `y` are normalized application/map coordinates, with x increasing to the right and y increasing downward.
- `p` is the integer pressure/depth iteration, valid from 1 through 50. It is an abstract algorithmic iteration, **not a physical pressure measurement**.
- `d = (p - 1) / 49` converts iteration levels 1–50 to normalized depth 0–1.
- `v` is the normalized voltage/model input in [0,1]. Values in this trial are synthetic, dimensionless model inputs, **not volts or electrical measurements**.

The step vector between consecutive input points is `Δq_i = q_i - q_(i-1) = (Δx, Δy, Δd, Δv)`. The unweighted 4D Euclidean step length is:

`L_i = sqrt(Δx^2 + Δy^2 + Δd^2 + Δv^2)`.

Z-layer and logical time are tracked as traversal/event metadata, but are **not** added as fifth or sixth dimensions to that 4D geometric step length.

## 2. Observed steps and sudden-pressure rule

The trial's observed horizontal step is `Δx = ±0.25`; its row step is `Δy = ±0.50`. One pressure-iteration increment corresponds to a normalized-depth increment of `1/49 ≈ 0.0204081633`. The observed depth steps are `11/49`, `13/49`, `13/49`, and `12/49`. The observed normalized-voltage steps are `+0.18`, `+0.25`, `+0.17`, and `+0.15`. Each adjacent input is separated by `1.25` logical seconds. These are the increments present in this five-input sample, not claims about the finest step resolvable in the entire UTF-4088 space; no global voltage quantization step is declared.

For this trial only, flag a *sudden-pressure candidate* when `abs(Δp) >= 12` iteration levels between adjacent inputs. This is an explicit, configurable test threshold, **not** a calibrated physical law or an intrinsic UTF-4088 constant. The four transitions have Δp of `+11`, `+13`, `+13`, and `+12`, with rates of `+8.8`, `+10.4`, `+10.4`, and `+9.6` iteration levels per logical second. Under this rule, inputs 3, 4, and 5 are flagged; input 1 is the initial baseline, and input 2 is below threshold.

**State identity versus transition identity:** the current `symbol_key()` routine hashes the instantaneous x, y, absolute pressure iteration p, and normalized voltage v. It does not hash Δp, Δt, pressure rate, or the sudden-pressure flag. The jump flags below therefore document a potentially significant event; they do not yet modify the already-generated state key. To guarantee a distinct result when the same state is reached through a different pressure transition, a versioned engine update must add a transition/event fingerprint or emit a separate event-symbol ID. The current key formula should not be silently changed.

## 3. Five input-mapping tables

## Input Mapping 1 — Layer zero, top-left

| Mapping field | Trial value |
|---|---|
| Trial input | 1 — Layer zero, top-left |
| Ordered symbol ID | UTF4088-SYMBOL-000001 |
| Current state key | 0xAFA5577333931820 (64-bit candidate key; not a Unicode code point) |
| Application location x, y | (0.05, 0.05) normalized map coordinates |
| Z-layer | 0 (traversal metadata; excluded from 4D distance) |
| Pressure input | 1 / 50 abstract iteration levels; not physical pressure |
| Normalized depth d = (p - 1) / 49 | 0 |
| Normalized voltage v | 0.12 in [0,1]; synthetic model input, not measured volts |
| Logical sample time | 0.00 seconds (metadata, excluded from 4D distance) |
| 4D application point q = (x, y, d, v) | (0.05, 0.05, 0.000000, 0.12) |
| Step vector Δq from previous input | Baseline — no preceding 4D sample |
| 4D Euclidean step length | — |
| Pressure change Δp | — (baseline) |
| Logical time change Δt | — (baseline) |
| Pressure rate Δp / Δt | — (baseline) |
| Voltage step Δv | — |
| Sudden-pressure candidate | BASELINE — no preceding input |
| Nearest curated reference anchor | space; composite proximity = 0.866672 |
| Provisional English wording | spatial distance |
| Provisional Latin wording | distantia spatialis |

## Input Mapping 2 — Layer zero, move right

| Mapping field | Trial value |
|---|---|
| Trial input | 2 — Layer zero, move right |
| Ordered symbol ID | UTF4088-SYMBOL-000002 |
| Current state key | 0x3A8AF7C9593A783F (64-bit candidate key; not a Unicode code point) |
| Application location x, y | (0.30, 0.05) normalized map coordinates |
| Z-layer | 0 (traversal metadata; excluded from 4D distance) |
| Pressure input | 12 / 50 abstract iteration levels; not physical pressure |
| Normalized depth d = (p - 1) / 49 | 11/49 = 0.224490 |
| Normalized voltage v | 0.30 in [0,1]; synthetic model input, not measured volts |
| Logical sample time | 1.25 seconds (metadata, excluded from 4D distance) |
| 4D application point q = (x, y, d, v) | (0.30, 0.05, 0.224490, 0.30) |
| Step vector Δq from previous input | (+0.250000, 0.000000, +0.224490, +0.180000) |
| 4D Euclidean step length | 0.381176689 |
| Pressure change Δp | +11 iterations |
| Logical time change Δt | 1.25 seconds |
| Pressure rate Δp / Δt | 8.800 iteration levels / logical second |
| Voltage step Δv | +0.180000 |
| Sudden-pressure candidate | NO — Δp = 11 < 12 |
| Nearest curated reference anchor | time; composite proximity = 0.866057 |
| Provisional English wording | temporal relation |
| Provisional Latin wording | relatio temporalis |

## Input Mapping 3 — Layer zero, next row

| Mapping field | Trial value |
|---|---|
| Trial input | 3 — Layer zero, next row |
| Ordered symbol ID | UTF4088-SYMBOL-000003 |
| Current state key | 0xFD6497A9A475933E (64-bit candidate key; not a Unicode code point) |
| Application location x, y | (0.05, 0.55) normalized map coordinates |
| Z-layer | 0 (traversal metadata; excluded from 4D distance) |
| Pressure input | 25 / 50 abstract iteration levels; not physical pressure |
| Normalized depth d = (p - 1) / 49 | 24/49 = 0.489796 |
| Normalized voltage v | 0.55 in [0,1]; synthetic model input, not measured volts |
| Logical sample time | 2.50 seconds (metadata, excluded from 4D distance) |
| 4D application point q = (x, y, d, v) | (0.05, 0.55, 0.489796, 0.55) |
| Step vector Δq from previous input | (-0.250000, +0.500000, +0.265306, +0.250000) |
| 4D Euclidean step length | 0.667373463 |
| Pressure change Δp | +13 iterations |
| Logical time change Δt | 1.25 seconds |
| Pressure rate Δp / Δt | 10.400 iteration levels / logical second |
| Voltage step Δv | +0.250000 |
| Sudden-pressure candidate | YES — Δp = 13 ≥ 12 |
| Nearest curated reference anchor | cause; composite proximity = 0.831129 |
| Provisional English wording | causal definability |
| Provisional Latin wording | definibilitas causalis |

## Input Mapping 4 — Layer zero, continue right

| Mapping field | Trial value |
|---|---|
| Trial input | 4 — Layer zero, continue right |
| Ordered symbol ID | UTF4088-SYMBOL-000004 |
| Current state key | 0x120C1C41A2380D33 (64-bit candidate key; not a Unicode code point) |
| Application location x, y | (0.30, 0.55) normalized map coordinates |
| Z-layer | 0 (traversal metadata; excluded from 4D distance) |
| Pressure input | 38 / 50 abstract iteration levels; not physical pressure |
| Normalized depth d = (p - 1) / 49 | 37/49 = 0.755102 |
| Normalized voltage v | 0.72 in [0,1]; synthetic model input, not measured volts |
| Logical sample time | 3.75 seconds (metadata, excluded from 4D distance) |
| 4D application point q = (x, y, d, v) | (0.30, 0.55, 0.755102, 0.72) |
| Step vector Δq from previous input | (+0.250000, 0.000000, +0.265306, +0.170000) |
| 4D Euclidean step length | 0.402227968 |
| Pressure change Δp | +13 iterations |
| Logical time change Δt | 1.25 seconds |
| Pressure rate Δp / Δt | 10.400 iteration levels / logical second |
| Voltage step Δv | +0.170000 |
| Sudden-pressure candidate | YES — Δp = 13 ≥ 12 |
| Nearest curated reference anchor | merit; composite proximity = 0.835553 |
| Provisional English wording | merit and importance |
| Provisional Latin wording | meritum et momentum |

## Input Mapping 5 — Next Z-layer, reset to top-left

| Mapping field | Trial value |
|---|---|
| Trial input | 5 — Next Z-layer, reset to top-left |
| Ordered symbol ID | UTF4088-SYMBOL-000005 |
| Current state key | 0xABE9098D80D3704E (64-bit candidate key; not a Unicode code point) |
| Application location x, y | (0.05, 0.05) normalized map coordinates |
| Z-layer | 1 (traversal metadata; excluded from 4D distance) |
| Pressure input | 50 / 50 abstract iteration levels; not physical pressure |
| Normalized depth d = (p - 1) / 49 | 49/49 = 1.000000 |
| Normalized voltage v | 0.87 in [0,1]; synthetic model input, not measured volts |
| Logical sample time | 5.00 seconds (metadata, excluded from 4D distance) |
| 4D application point q = (x, y, d, v) | (0.05, 0.05, 1.000000, 0.87) |
| Step vector Δq from previous input | (-0.250000, -0.500000, +0.244898, +0.150000) |
| 4D Euclidean step length | 0.628470374 |
| Pressure change Δp | +12 iterations |
| Logical time change Δt | 1.25 seconds |
| Pressure rate Δp / Δt | 9.600 iteration levels / logical second |
| Voltage step Δv | +0.150000 |
| Sudden-pressure candidate | YES — Δp = 12 ≥ 12 |
| Nearest curated reference anchor | eccentricity; composite proximity = 0.796653 |
| Provisional English wording | eccentric distance |
| Provisional Latin wording | distantia eccentrica |

## 4. Interpretation and implementation rules

1. **Application location:** each requested x/y pair comes directly from `location.x` and `location.y` in the corresponding input JSON. These are logical normalized coordinates, not GPS coordinates, physical placement, or screen pixels.
2. **Pressure and voltage:** use `pressure.iteration` and `field.voltage_normalized` as defined. No physical pressure unit (Pa, kPa, psi) or physical voltage unit (V) is supplied by these fixtures, so none is inferred.
3. **Z-layer traversal:** input 5 moves from Z-layer 0 to Z-layer 1. Z-layer changes the traversal address but is not part of the four-dimensional distance. The 4D point still differs from input 1 because normalized depth and normalized voltage differ.
4. **Time:** logical time and Δt help describe temporal transitions and pressure rates, but are not part of the current geometric 4D metric.
5. **Symbol uniqueness:** the existing five candidate keys and 8×12 bitmaps are unique within this trial. English/Latin labels and reference-term proximity are provisional annotations, not verified meanings, probabilities, or published Unicode characters.
6. **Reproduction:** run `python3 alphabet/utf-4088/run_inputs.py` from the repository root for the existing state-derived trial outputs. The runner does not yet use the sudden-pressure flag as an additional key input.
