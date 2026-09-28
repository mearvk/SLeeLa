# `.sst` Semantic Subject Support — NS-SST-SUB-0001

Nordshrift 2.0 `.sst` sheets have a first-class `subject` declaration block. A
subject makes the semantic meaning of a computation explicit: what is modeled,
in what units, under what assumptions, by what relation, with what provenance,
and how completion will be validated. It fills a
`nordshrift::semantic::Subject` (see [`subject_model.h`](subject_model.h)) and is
parsed by the same indentation-aware NS-SST parser as every other section.

A subject is metadata, not a program. It carries the semantic layer shared by
the Math, Physics, Economics, Chemistry, and Financial libraries so a numeric
result is not silently promoted to an empirical fact.

## Canonical chain

```
Subject -> Quantity -> Unit -> Assumption -> Relation -> Formula ->
Transformation -> Result -> ComparativeNorm -> Evidence -> Explanation ->
Validation
```

## Grammar

A subject is `subject <Identity>:` followed by scalar keys and repeatable nested
blocks:

```sst
subject ProjectileRange:
  domain: physics
  depends: [ math ]

  quantity range:
    value:     "v0^2 * sin(2*theta) / g"
    unit:      meter
    dimension: length
    domain:    "0 <= theta <= pi/2"
    status:    modeled

  assumption noDrag:
    statement: "air resistance is neglected"
    scope:     "low-speed, short-range trajectories"
    source:    "modeling choice"
    status:    assumed

  relation rangeFormula:
    formula: "R = v0^2 * sin(2*theta) / g"
    inputs:  [ v0, theta, g ]
    outputs: [ range ]

  transformation deriveRange:
    source:      rangeFormula
    operation:   "evaluate at theta = 45deg"
    parameters:  "theta=pi/4"
    destination: range
    approximate: false

  comparison:
    prior:     ProjectileRangeVacuumBaseline
    current:   ProjectileRange
    reference: ProjectileRangeVacuumBaseline
    norm:      "ratio of modeled range to ideal vacuum range"

  evidence:
    status: modeled
    source: "closed-form kinematics under the noDrag assumption"
    note:   "result is a model output, not a measurement"

  explanation:
    subject: ProjectileRange
    steps: [ "identify subject and domain", "apply the range relation" ]

  todo validateAgainstMeasurement:
    priority:      1
    depends:       [ ProjectileRange ]
    preconditions: "a measured trajectory data set is available"
    action:        "compare modeled range to observed range"
    expected:      "relative error within stated tolerance"
    validation:    "residual analysis over the data set"
    status:        planned
```

Nested blocks (`quantity`, `assumption`, `relation`, `transformation`,
`comparison`, `evidence`, `explanation`, `todo`) may each appear any number of
times. `quantity`, `assumption`, `relation`, `transformation`, `explanation`,
and `todo` accept an optional identity/name before the `:`; `comparison` and
`evidence` are nameless.

## Evidence status

A `status` records epistemic provenance and must be one of:

`observed`, `specified`, `derived`, `modeled`, `inferred`, `assumed`.

Unknown values are rejected with `NSS-E-SUB-002`.

## Work status

A `todo` `status` must be one of:

`planned`, `ready`, `active`, `blocked`, `validating`, `complete`, `deferred`.

Unknown values are rejected with `NSS-E-SUB-003`.

## Validation

`nordshrift check` prints each declared subject, its collection counts, and its
canonical chain, and applies two provenance checks (warnings, so a subject sheet
remains valid):

- `NSS-W-SUB-010` — a subject depends on a subject that is neither declared in
  the sheet nor a foundational domain (e.g. `math`).
- `NSS-W-SUB-011` — a `modeled` / `inferred` / `derived` quantity has no
  supporting relation, assumption, or evidence (a bare claim).

## Build effect

On `nordshrift build`, declared subjects (alongside any `network:` / `finance:`
series) are emitted into a component manifest artifact for the selected target.
See [`SST.NETWORK.md`](SST.NETWORK.md) and [`SST.FINANCE.md`](SST.FINANCE.md).

## Diagnostics summary

| Code | Meaning |
|---|---|
| `NSS-E-SUB-001` | subject block without an identity |
| `NSS-E-SUB-002` | unknown evidence status |
| `NSS-E-SUB-003` | unknown work status |
| `NSS-W-SUB-010` | dependency on an undeclared subject |
| `NSS-W-SUB-011` | modeled/inferred/derived quantity without supporting provenance |
