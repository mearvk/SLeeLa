# Sleela

**Sleela** is a Java-like programming language that runs on a Turing-complete, thread-friendly **C/C++ execution core**, with **Nordshrift** — a `.sst` transpiler driver (spec **NS-SST-0001**) — layered on top to drive a *triplet* of targets: **Java**, **Sleela**, and **C**.

A Sleela source file (`.sleela`) is a **Wrapper™** — the program unit governed by the Sleela Language Metadocument (**SL-META-0001**).

> Current versions: **Sleelvac™ 1.4** compiler (supported `.sleela` syntax **1.0 – 1.1**) · **Nordshrift 2.0** driver.

---

## Quick start

```sh
cd impl && make
./build/sleela run examples/hello.sleela
./build/nordshrift build nordshrift/examples/demo/build.sst
```

`make` produces two binaries: `build/sleela` (compiler/runtime) and `build/nordshrift` (the `.sst` driver).

---

## Wiki contents

- **[Language Reference](Language-Reference)** — syntax, types, the `.sleela` Wrapper™, conducted methods, native modules
- **[Nordshrift Sheet Reference](Nordshrift-Sheet-Reference)** — every `.sst` section, the triplet, the component manifest, diagnostics
- **[Writing a Subject](Writing-a-Subject)** — the 2.0 semantic subject model, end to end

Below is the map of the rest of the system.

---

## The Sleela Language

Java-like: a program is one or more `class` declarations; execution begins at a `main()` method. Top-level statements are **not** allowed — everything lives inside a class.

```sleela
class Hello {
    void main() {
        print("Hello, Sleela!");
    }
}
```

```sh
./build/sleela run    examples/hello.sleela          # compile + run on the core
./build/sleela check  examples/hello.sleela          # parse/validate only
./build/sleela compile examples/hello.sleela -o hello.sleela   # persistent Core artifact
```

**Bundled examples:** `hello`, `fizzbuzz`, `factorial`, `fibonacci`, `conduct`, `threads`, `network_echo`, `chemistry_presumed`, `financial`, `versioned`.

See **[Language Reference](Language-Reference)**, plus `COMPILER.md`, `SOURCE.md`, `VERSION.md`.

---

## The Execution Core

A Turing-complete C/C++ VM (`impl/core/`) with a tagged value model, threading, and networking. Sleela lowers to core opcodes; the `sleela` target compiles to a **persistent, runnable `.sleela` Core artifact**. Networking exposes `LISTEN / ACCEPT / CONNECT / SOCKREAD / SOCKWRITE / SOCKCLOSE`. The core can also ingest `.xclass` (SecureJDK 28 structural class metadata).

---

## Nordshrift & the `.sst` Sheet

**Nordshrift** reads a `.sst` **control sheet** (indentation-significant, pragma-first) and drives transpilation to the selected *triplet* target. The `.sst` file is a **build-control surface, not a program**.

```sh
nordshrift check    <sheet.sst>     # validate; print diagnostics
nordshrift build    <sheet.sst>     # compile the sheet's sources to its target
nordshrift objects                  # list the SHEET.sheet object compatibility list
nordshrift relevance --target=c [Object]
nordshrift version
```

See **[Nordshrift Sheet Reference](Nordshrift-Sheet-Reference)**, plus `NORDSHRIFT.md`, `SST.model` (1.0), `SST-2.0.model` (2.0 normative).

---

## Component Series: Network & Finance

An `.sst` sheet can declare two **closed** component series that become a build artifact (a *component manifest* — Java class / C arrays / Sleela class):

```sst
network:
  objects: [ Endpoint, NIC, Link, Router, Gateway, LoadBalancer, Service, TLS, DNS ]
  transports: [ tcp, tls, quic ]
  address-family: dual
  tls: true

finance:
  objects: [ PresentValue, NetPresentValue, BondPrice, CAPM, WACC, Ratio ]
  currency: USD
  period: annual
  discounting: discrete
```

Unknown members are rejected (`NSS-E-NET-00x` / `NSS-E-FIN-00x`). Every object also lives in `SHEET.sheet`. See `SST.NETWORK.md`, `SST.FINANCE.md`.

---

## Semantic Subjects (Nordshrift 2.0)

The 2.0 layer makes the meaning of a computation explicit via `subject:` blocks, following the canonical chain:

> **Subject → Quantity → Unit → Assumption → Relation → Formula → Transformation → Result → ComparativeNorm → Evidence → Explanation → Validation**

Evidence status is one of **observed · specified · derived · modeled · inferred · assumed** — so a number is not silently promoted to an observation. See **[Writing a Subject](Writing-a-Subject)** and `SST.SUBJECT.md`.

---

## Subject Libraries

First-class domain layers above the core (`impl/subjects/`):

| Subject | Reference | What it adds |
|---|---|---|
| **Math** | `MATH.md` | foundational numerical layer |
| **Physics** | `PHYSICS.md` | constants + mechanics, relativity, EM, gases, waves |
| **Economics** | `ECONOMICS.md` | time-value, elasticity, rates, macro identities |
| **Chemistry** | `CHEMISTRY.md` | composition, valence/bonds, symmetry, qualified inference |
| **Financial** | `FINANCIAL.md` | TVM, NPV, bonds, CAPM, WACC, 2×2 systems, quadratics |

Math is foundational; the others may **explicitly** depend on it.

---

## SHEET.sheet — the Object Catalog

`SHEET.sheet` is the machine-readable catalog of common system objects that backs both Sleela's *conducted methods* and Nordshrift's *object-compatibility list*. Currently **156 objects across 18 role categories**, rooted at `System` (depth **3024**). Nordshrift converts each object to a per-target **relevance**: `direct` · `model` · `none`.

---

## The Constitution

Ordained constraints the system honors (from `SHEET.sheet` invariants + NS-SST-0001 diagnostics):

- **Article I — System invariants:** depth ≤ **3024**; up to **3024** congruent-linear systems; complexity degree ≤ **4** (`1` direct, `2` mediated, `3` brokered, `4` federated); `connect(n)` permitted iff `n ≤ 3024 AND degree ≤ 4`.
- **Article II — Congrains:** Invariant, Precondition, Postcondition, Constraint, Dependency, Coupling, Congruence, Contract.
- **Article III — Limits:** Bound, Quota, RateLimit, Timeout, Capacity, Threshold, DepthLimit, ComplexityDegree.
- **Article IV — Health** (system-health metrics) and **Article V — IQ** (system *insight/quality* — **not** psychometric).

---

## Reference Index

| Doc | Topic |
|---|---|
| `README.md` | project overview |
| `SLEELA.md` | the `.sleela` Wrapper™, Sigil QR, steganographic frame |
| `COMPILER.md` / `SOURCE.md` / `VERSION.md` | compiler, source file, versions |
| `NORDSHRIFT.md` | `.sst` driver + 2.0 semantic layer |
| `SST.model` / `SST-2.0.model` | NS-SST-0001 1.0 / 2.0 specs |
| `impl/nordshrift/SST.NETWORK.md` / `SST.FINANCE.md` / `SST.SUBJECT.md` | series & subject specs |
| `MATH/PHYSICS/ECONOMICS/CHEMISTRY/FINANCIAL.md` | subject libraries |
| `SHEET.sheet` | object catalog |
| `GLOSSARY.md` | terms (Part A science/engineering · Part B US evidentiary method) |
| `SUMMARY.md` | whole-repo accounting |
| `impl/README.md` / `impl/DESIGN.md` | implementation guide + architecture |

---

*This wiki mirrors the repository docs; the repo is authoritative. See [Reference Index](#reference-index).*
