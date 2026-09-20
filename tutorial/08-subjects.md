# Lesson 08 — Subjects & the Sheet Model

> Series: [index](README.md) · prev: [07](07-conducted-methods.md) · next: [09 — Verification & the SHA-256 gate](09-verification.md)

A `.sst` sheet can declare more than sources and a target. It can declare
**subjects** — typed domains of quantities and relations (math, physics,
economics, finance, inference, chemistry) — plus **network** and **finance**
component models. Declaring them has a real, inspectable build effect
(Nordshrift emits a component manifest).

## A subject block, annotated

From the demo sheet (`impl/nordshrift/examples/demo/build.sst`):

```sst
subject DemoThroughput:
  domain: economics
  depends: [ math ]
  quantity throughput:
    value:  "completed / window"
    unit:   "ops/second"
    status: derived
  relation rate:
    formula: "throughput = completed / window"
    inputs:  [ completed, window ]
    outputs: [ throughput ]
  evidence:
    status: derived
    source: "counter over a fixed window"
```

- **`domain`** — which subject library governs it (`economics`, `math`, …).
- **`quantity`** — a named measurable with a `unit` and `status`.
- **`relation`** — a formula tying inputs to outputs.
- **`evidence`** — where the quantity's value comes from (`derived`, etc.).

## The network & finance models

The same sheet declares component models:

```sst
network:
  objects: [Endpoint, NIC, Link, Packet, Queue, Switch, Router, Fabric, ...]
  transports: [tcp, udp, tls, quic]
  address-family: dual
  tls: true

finance:
  objects: [ PresentValue, NetPresentValue, Ratio ]
  currency:    USD
  period:      annual
  discounting: discrete
```

## What you get

Building a sheet that declares these emits a **component manifest** artifact
(`ComponentManifest.sleela` for the sleela target), so declaring a subject or a
network isn't decorative — it produces something you can inspect:

```sh
impl/build/nordshrift build impl/nordshrift/examples/demo/build.sst
# ... nordshrift: manifest -> .../build/ComponentManifest.sleela ...
```

## Discipline: evidence over assertion

Subjects carry an `evidence`/`status` field for a reason. Quantities are marked
`derived`, `measured`, or `assumed` — the model states *where a number comes
from* rather than asserting it. This is the same honesty the subject libraries
enforce with real numeric tests (see `ARCHITECTURE.md`), and it keeps
declared quantities auditable.

**Next:** [Lesson 09 — Verification & the SHA-256 gate](09-verification.md)
