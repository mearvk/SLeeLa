# Writing a Subject

Nordshrift 2.0 adds a **semantic subject** layer to the `.sst` sheet. A `subject:` block makes the *meaning* of a computation explicit — what is modeled, in what units, under what assumptions, by what relation, with what provenance, and how completion will be validated. It fills the shared `nordshrift::semantic::Subject` model used across all five subject libraries.

The normative document is `impl/nordshrift/SST.SUBJECT.md`; the C++ model is `impl/nordshrift/subject_model.h`.

---

## The canonical chain

> **Subject → Quantity → Unit → Assumption → Relation → Formula → Transformation → Result → ComparativeNorm → Evidence → Explanation → Validation**

The purpose is discipline, expressed by the domain ideals (see `NORDSHRIFT.md`):

1. Identity before calculation.
2. Quantity before formula.
3. Unit and dimension before interpretation.
4. Assumption before extrapolation.
5. Provenance before trust.
6. Validation before completion.
7. Computation remains distinguishable from observation.

A numeric result is **not** silently promoted to an observation.

---

## Minimal subject

```sst
#nordshrift 1.0
#sleela     1.0

sheet demo-subject:
  version 1.0.0

source:
  root "src"
  glob "**/*.sleela"

subject Throughput:
  domain: economics
  depends: [ math ]
  quantity rate:
    value:  "completed / window"
    unit:   "ops/second"
    status: derived
  relation formula:
    formula: "rate = completed / window"
    inputs:  [ completed, window ]
    outputs: [ rate ]
  evidence:
    status: derived
    source: "counter over a fixed window"

target:
  root "out"
  target-language sleela
```

`nordshrift check` prints the subject, its collection counts, and its chain.

---

## Anatomy of a `subject` block

`subject <Identity>:` followed by scalar keys and repeatable nested blocks.

**Scalar keys:**

| Key | Meaning |
|---|---|
| `domain` | the subject's domain (e.g. `physics`, `economics`) |
| `depends` / `dependencies` | list of subjects (or `math`) this one depends on |

**Nested blocks** (each may repeat):

### `quantity <name>:`
| Key | Meaning |
|---|---|
| `value` | value or expression |
| `unit` | unit identity |
| `dimension` | dimensional identity |
| `domain` | validity domain of the quantity |
| `status` | evidence status (see below) |

### `assumption <name>:`
`statement`, `scope`, `source`, `status`.

### `relation <name>:`
`formula`, `inputs` (list), `outputs` (list).

### `transformation <name>:`
`source`, `operation`, `parameters`, `destination`, `approximate` (bool).

### `comparison:` (nameless)
`prior`, `current`, `reference`, `norm`. Comparative reasoning follows **PriorSubject → CurrentSubject → ReferenceSubject → ComparativeNorm**.

### `evidence:` (nameless)
`status`, `source`, `note`.

### `explanation:`
`subject`, `steps` (list) — or repeated `step`.

### `todo <name>:`
`priority`, `depends`, `preconditions`, `action`, `expected` (`expected-result`), `validation`, `status` (work status, see below).

---

## Status vocabularies

**Evidence status** (`status` on quantities/assumptions/evidence):

`observed` · `specified` · `derived` · `modeled` · `inferred` · `assumed`

**Work status** (`status` on a `todo`):

`planned` · `ready` · `active` · `blocked` · `validating` · `complete` · `deferred`

Unknown values are rejected (`NSS-E-SUB-002` evidence, `NSS-E-SUB-003` work).

---

## Full example

From `nordshrift/examples/kinematics-subject.sst`:

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
    steps: [ "identify subject and domain",
             "state quantities and units",
             "declare the noDrag assumption",
             "apply the range relation",
             "evaluate the transformation",
             "record evidence status as modeled" ]

  todo validateAgainstMeasurement:
    priority:      1
    depends:       [ ProjectileRange ]
    preconditions: "a measured trajectory data set is available"
    action:        "compare modeled range to observed range"
    expected:      "relative error within stated tolerance"
    validation:    "residual analysis over the data set"
    status:        planned
```

`check` output:

```
subject 'ProjectileRange' (domain=physics): 2 quantity, 1 assumption, 1 relation, 1 transformation, 1 comparison, 1 evidence, 1 explanation, 1 todo
  chain: Subject -> Quantity/Unit -> Assumption -> Relation/Formula -> Transformation -> Result -> ComparativeNorm -> Evidence -> Explanation -> Validation
```

---

## Validation rules

`nordshrift check` applies provenance checks (warnings — a subject sheet stays valid):

- **`NSS-W-SUB-010`** — a subject depends on a subject that is neither declared in the sheet nor a foundational domain (`math`).
- **`NSS-W-SUB-011`** — a `modeled` / `inferred` / `derived` quantity has **no** supporting relation, assumption, or evidence (a bare claim).

Errors:

- **`NSS-E-SUB-001`** — subject block without an identity.
- **`NSS-E-SUB-002` / `NSS-E-SUB-003`** — unknown evidence / work status.

---

## Build effect

On `nordshrift build`, declared subjects (with any `network:` / `finance:` series) are emitted into a **component manifest** artifact for the selected target. See **[Nordshrift Sheet Reference](Nordshrift-Sheet-Reference#the-component-manifest-build-effect)**.

---

## See also

- **[Nordshrift Sheet Reference](Nordshrift-Sheet-Reference)** — all `.sst` sections
- **[Language Reference](Language-Reference)** — the `.sleela` sources
- `SST.SUBJECT.md`, `SST-2.0.model`, `subject_model.h`
