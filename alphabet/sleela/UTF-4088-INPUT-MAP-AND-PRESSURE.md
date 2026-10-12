# UTF-4088 Input Map and Pressure Iteration Protocol

Status: experimental protocol, version 1.

## 1. Purpose

This document defines a reproducible traversal order for UTF-4088 input maps. It is referenced by the UTF-4088 README, code-point/serialization contract, alphabet integration notes, and the SLeeLa alphabet documentation series.

The map is a logical data structure. Its pressure values are abstract algorithmic parameters, not physical pressure, voltage, force, or instructions to drive electrical hardware.

## 2. Coordinate system

Each map element has the stable coordinate `(x, y, z)`:

- `x`: horizontal position, increasing from left to right.
- `y`: row position, increasing from top to bottom.
- `z`: layer/index, increasing to the next layer after the current layer has been completely processed.

The origin is `(0, 0, 0)`, at the top-left element of the first layer. Coordinates are zero-based in software. Printed indexes may additionally show one-based ordinal positions.

## 3. Canonical traversal

The canonical order is **top-left, left-to-right, top-to-bottom, then next z-layer**:

1. Start at the top-left element of layer `z=0`.
2. Process its pressure iterations from 1 through 50 in ascending order.
3. Move right to the next `x` element and repeat the pressure iterations.
4. When a row is complete, return to the leftmost `x` position on the next `y` row.
5. When every row of the current layer is complete, increment `z` and restart at the top-left element of that layer.
6. Stop when the declared map dimensions or explicit work budget is exhausted.

Pseudocode:

```text
for z in 0 .. depth-1:
    for y in 0 .. height-1:
        for x in 0 .. width-1:
            for pressure in 1 .. 50:
                process(map[x, y, z], pressure)
```

This ordering makes a run reproducible and suitable for tests, logs, and printed indexes. Implementations must not reorder these loops unless a separately named traversal mode is selected.

## 4. Pressure scale: 1–50

Pressure is an integer iteration level with an assumed inclusive range of `1..50`. Level 50 is the current assumed maximum, not a calibrated physical quantity.

- Level 1 is the first/lowest defined iteration.
- Levels 2–49 are intermediate iteration levels.
- Level 50 is the maximum defined iteration.
- Values below 1, above 50, non-integers, NaN, and infinity are invalid.
- If a future release changes the range or meaning, it must version the protocol and document migration behavior.

The value must not be silently interpreted as a probability, voltage, force, semantic confidence, or glyph intensity. Any such mapping requires a separate named, versioned function.

## 5. Stable identifiers and National Lien ordering

Every input-map element should retain its coordinate identity independently of its display order. The National Lien Alphabetic Register uses a permanent catalogue ID for identity and a separate alphabetic rank for sorted presentation. Map traversal order, catalogue ID, alphabetic rank, and pressure iteration are four different fields and must not be conflated.

Recommended record fields:

| Field | Meaning |
|---|---|
| `catalogue_id` | Immutable UTF-4088 registry identifier |
| `x`, `y`, `z` | Logical map coordinate |
| `pressure_level` | Iteration level, 1–50 |
| `subject_key` | Normalized subject/category key, if assigned |
| `alphabetic_rank` | Generated presentation rank, if assigned |
| `status` | Verified, provisional, or experimental |
| `source_ref` | Provenance/reference for the record |

Alphabetic ranking is generated from documented subject-sorting rules; it must not change the permanent ID or the spatial traversal contract.

## 6. Bounds, determinism, and validation

Implementations must validate nonzero dimensions and ensure that width × height × depth and the total number of pressure operations cannot overflow the chosen integer type. Apply a configurable work budget before processing large maps. Reject coordinates outside the declared dimensions and pressure values outside 1–50. A run should record protocol version, dimensions, start/end coordinates, traversal mode, pressure range, and deterministic seed where sampling is used.

A generated bitmap or input record does not acquire a verified meaning solely from its position or pressure level. Semantic labels require explicit annotation and provenance.

## 7. Tests

Conformance tests should verify:

- The first visited coordinate is `(0,0,0)`.
- `x` increments before `y`; `y` increments before `z`.
- Every coordinate is visited once per traversal.
- Pressure levels for each coordinate are exactly 1, 2, …, 50.
- The next `z` layer is entered only after all elements in the previous layer finish.
- Invalid pressure values and dimension/work-budget overflow are rejected.
- Repeated runs with the same input and seed produce the same ordered trace.

This protocol defines ordering only. It does not claim that all 16,606 current registry records have verified meanings or that UTF-4088 is a standard Unicode encoding.


## Adaptive probe scheduling

The canonical coordinate/depth traversal remains the reproducible baseline. An optional adaptive scheduling policy may choose the next probe using expected match utility, coverage, uncertainty reduction, and compute/resource cost, as specified in [UTF-4088-PROBE-ORDERING-AND-MATCHING.md](UTF-4088-PROBE-ORDERING-AND-MATCHING.md). Adaptive runs must log their policy and maintain a coverage pass; they must not silently change the meaning of depth 1–50.


## Canonical source and relay

This is a synchronized mirror in the alphabet/sleela directory. Its canonical source is [utf-4088/INPUT-MAP-AND-PRESSURE.md](../../utf-4088/INPUT-MAP-AND-PRESSURE.md). Use [UTF-4088 relay index](UTF-4088-RELAY.md) to navigate the complete mirrored series. Update the canonical source and its mirror together when changing the protocol.
