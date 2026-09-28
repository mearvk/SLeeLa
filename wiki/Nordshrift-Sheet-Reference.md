# Nordshrift Sheet Reference

**Nordshrift** reads a `.sst` **control sheet** and drives transpilation of Sleela sources to the selected *triplet* target. The sheet is a **build-control surface, not a program**. This page is the practical reference; the normative documents are `SST.model` (NS-SST-0001, 1.0), `SST-2.0.model` (2.0), and `NORDSHRIFT.md`.

> Current driver: **Nordshrift 2.0**.

---

## CLI

```sh
nordshrift check    <sheet.sst>                    # validate; print diagnostics
nordshrift build    <sheet.sst>                    # compile the sheet's sources to its target
nordshrift objects                                 # list the SHEET.sheet object compatibility list
nordshrift relevance --target=java|sleela|c [Object]   # show direct/model relevance
nordshrift version
```

`build` transpiles each resolved source; for `target-language sleela` it also writes a runnable `.sleela` Core artifact, and (when declared) a **component manifest** — see below.

---

## Sheet syntax basics

- **Indentation-significant**, pragma-first.
- A block opens with `name:` and an indented body.
- Scalars: strings (`"..."` / `"""..."""`), integers (`1_000`), versions (`1.0.0`), identifiers, booleans (`true/false`, `on/off`).
- Lists: `[ a, b, c ]`.

Required pragma:

```sst
#nordshrift 1.0
#sleela     1.0
```

---

## Sections

### `sheet`

```sst
sheet my-build:
  version      1.0.0
  author       "Team"
  description  "..."
  tags  [ "a", "b" ]
```

### `import`

```sst
import "path/to/lib.sleela" as lib only [ Foo, Bar ]
```

Alias uniqueness is enforced (`NSS-E-0021`).

### `source`

```sst
source:
  root     "src"
  glob     "**/*.sleela"
  exclude  [ "build/**", "**/*_stub.sleela" ]
  watch    false
```

Absolute globs are rejected (`NSS-E-0032`); an empty resolved set errors (`NSS-E-0031`); a missing root errors (`NSS-E-0030`).

### `target`

```sst
target:
  root            "out"
  layout          mirror-source        # mirror-source | flat | package-mapped | custom
  java-version    21                    # must be >= 17
  package-root    "com.example.app"
  target-language sleela                # java | sleela | c   (the triplet)
  overwrite       true
  source-map      true
```

`java-version < 17` errors (`NSS-E-0040`).

### `pipeline`

Ordered phases: `lex, parse, desugar, bind, infer, effect-check, rule-eval, ir, java-emit, java-print, diagnostic`.

```sst
pipeline:
  phases  [ lex, parse, bind, infer, java-print ]
  parallel-threshold  4
  verbosity  normal
  fail-fast  false
```

Out-of-order phases error (`NSS-E-0050`); skipping a mandatory phase (`lex`, `parse`, `bind`, `infer`, `java-print`) errors (`NSS-E-0051`).

### `rules`, `effects`, `derive`, `guards`, `interop`

Standard 1.0 sections for lint rules, effect policy, derivation artifacts, guard mode, and Java interop mapping. See `SST.model`. Example:

```sst
effects:
  policy   strict
  declare  [ DatabaseRead, PaymentGateway ]
  default-effect  "Pure"
```

### `profile`

An overlay that overrides sections (single inheritance):

```sst
profile dev:
  pipeline:
    verbosity  verbose
```

---

## Component series blocks

### `network:` (NS-SST-NET-0001)

Closed object series. Objects: `Endpoint, NIC, Link, Packet, Queue, Switch, Router, Fabric, Listener, Connector, Gateway, LoadBalancer, Service, TLS, DNS`.

```sst
network:
  objects: [ Endpoint, Router, Gateway, LoadBalancer, Service, TLS, DNS ]
  transports: [ tcp, udp, tls, quic ]
  address-family: dual        # ipv4 | ipv6 | dual
  tls: true
```

Also accepts singular repeated forms (`object: ...`, `transport: ...`); duplicates coalesce. Unknown members: `NSS-E-NET-001` (object), `NSS-E-NET-002` (transport), `NSS-E-NET-003` (address-family).

### `finance:` (SST.FINANCIAL.md)

Closed object series. Objects: `FutureValue, PresentValue, AnnuityPresent, AnnuityFuture, NetPresentValue, BondPrice, CAPM, WACC, Determinant2x2, LinearSolve2x2, QuadraticEquation, Ratio`.

```sst
finance:
  objects: [ PresentValue, NetPresentValue, BondPrice, CAPM, WACC, Ratio ]
  currency:    USD
  period:      annual         # annual | semi-annual | quarterly | monthly | continuous
  discounting: discrete       # discrete | continuous
```

Unknown members: `NSS-E-FIN-001` (object), `NSS-E-FIN-002` (period), `NSS-E-FIN-003` (discounting).

### `subject:` (Nordshrift 2.0 semantic layer)

Declares explicit semantics. See **[Writing a Subject](Writing-a-Subject)**.

---

## The component manifest (build effect)

When a sheet declares `network:`, `finance:`, or `subject:` blocks, `nordshrift build` emits a **component manifest** artifact so those declarations have a real effect:

| Target | Manifest form |
|---|---|
| `java` | a `ComponentManifest` class of `static final String[]` arrays (printed) |
| `c` | null-terminated `const char*` name arrays with `_count`s (printed) |
| `sleela` | a `ComponentManifest` class written to `<sheetDir>/build/ComponentManifest.sleela` |

Every network/finance object also lives in `SHEET.sheet`, so `nordshrift objects` / `nordshrift relevance` will show it alongside the system objects (as a `model` relevance).

---

## Diagnostics

Codes are `NSS-{E|W}-{area}-{number}`. Highlights:

| Code | Meaning |
|---|---|
| `NSS-E-0003/0004` | missing / unsupported `#nordshrift` pragma |
| `NSS-E-0010` | no (or duplicate) `sheet` block |
| `NSS-E-0030/0031/0032` | source root missing / empty set / absolute glob |
| `NSS-E-0040` | `java-version < 17` |
| `NSS-E-0050/0051` | pipeline phases out of order / mandatory phase skipped |
| `NSS-E-NET-001/002/003` | unknown network object / transport / address-family |
| `NSS-E-FIN-001/002/003` | unknown finance object / period / discounting |
| `NSS-E-SUB-001/002/003` | subject without identity / bad evidence status / bad work status |
| `NSS-W-SUB-010/011` | undeclared subject dependency / unsupported modeled quantity |

`check` prints per-diagnostic detail plus a final error/warning tally, and reports the declared network/finance/subject series.

---

## Worked examples in the repo

- `nordshrift/examples/commerce-engine.sst` — full 1.0 sheet (java target)
- `nordshrift/examples/demo/build.sst` — network + finance + subject, builds to a runnable artifact + manifest
- `nordshrift/examples/finance-desk.sst` — network + finance component series
- `nordshrift/examples/kinematics-subject.sst` — a full semantic subject

---

## See also

- **[Language Reference](Language-Reference)** — the `.sleela` source you point a sheet at
- **[Writing a Subject](Writing-a-Subject)** — the 2.0 semantic model
- `NORDSHRIFT.md`, `SST.model`, `SST-2.0.model`, `impl/nordshrift/SST.NETWORK.md`, `SST.FINANCE.md`, `SST.SUBJECT.md`
