<!-- @MT:SXG -->
# SLeeLa

## Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Ubuntu.Determinant.Beta.Restricted/blob/main/images/Bitcoin_and_wallet_in_slots_2K_202609042306%20(1).jpeg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh

US Democratic and US Policy.

**Sleela** is a Java-like programming language running on a Turing-complete,
thread-friendly **C/C++ execution core**, with **Nordshrift** — a `.sst`
transpiler driver (spec `SST.model`, NS-SST-0001) — sitting on top to drive the
triplet of targets (**Java**, **Sleela**, **C**).

- The working implementation lives under [`impl/`](impl/) — see
  [`impl/README.md`](impl/README.md) for the full guide, and
  [`impl/DESIGN.md`](impl/DESIGN.md) for the architecture.
- [`SHEET.sheet`](SHEET.sheet) is the catalog of common system objects (129
  objects across 16 role categories) that backs Sleela's *conducted methods*
  and Nordshrift's *object compatibility list*.
- [`SST.model`](SST.model) is the normative `.sst` format specification.

```sh
cd impl && make
./build/sleela run examples/hello.sleela
./build/nordshrift build nordshrift/examples/demo/build.sst
```

## Document metalayers

Every document in this repository can carry two cooperating metalayers:

- **Metatags — [`METATAGS.md`](METATAGS.md) (MT-META-0001).** A closed series of
  16 single-letter tags (`@MT:…`) that make files findable by facet at a glance
  — `P`rint, `M`ath/Methods, `R`eals, `T`asks … and concern tags for
  governance, rights, voters, humans, and more. Reachable by `grep`, IDEs, OS
  search, and web crawlers through one token. Tool: [`impl/metatag/`](impl/metatag/).

- **Integrity seal — [`SEAL.md`](SEAL.md) (MT-SEAL-0001).** A tamper-evidence
  metaframe bound to a document with a keyed HMAC and a *solve matrix* of the
  document's machine natures and numbers. It is revealed only through its **lip**
  (a careful load with the word) and settles across **three layers** — an L1
  Sleela compiler signature, a temporary L2 OS rider, and, once it **dries** over
  a few minutes, a stable L3 binding. `seal layers <file>` shows the same
  artifact existing at two layers at once: the bare document (L1) and the
  document *with* its metaframe (L2). Tool: [`impl/seal/`](impl/seal/).

Both are verified together in CI by [`impl/ci/mt-check.sh`](impl/ci/mt-check.sh)
(`metatag scan` + `seal audit`) via
[`.github/workflows/mt-check.yml`](.github/workflows/mt-check.yml).

## The Constitution — ordained constraints, in order

This is the ordered constitution of governing constraints ("congrains") the
system honors. Every rule here is enforced or expressible today; each is drawn
from `SHEET.sheet` (the `congrains` and `limits` sections and the `system`
invariants) or from the NS-SST-0001 diagnostics. Constraints are organized into
named **groupings**. Two of the groupings — **Health** and **IQ** — are
*system* metric groupings (system health and system insight/quality); they
describe the software system's own condition, nothing about people.

### Article I — System invariants (ordained, always in force)

In order of precedence:

1. **Depth** — the relevant system depth is **3024**: the maximum
   nesting/reachability from `System` to any leaf object (`SHEET.sheet` →
   `system.depth`; the `DepthLimit` congrain).
2. **Connectivity** — up to **3024** congruent-linear systems may be connected
   (`system.congruent-linear-systems-max`).
3. **Complexity degree** — the connection order between systems must be **≤ 4**
   (`system.complexity-degree-max`; the `ComplexityDegree` congrain). Degrees:
   `1` direct · `2` mediated · `3` brokered · `4` federated.
4. **The connect rule** — `connect(n)` is permitted iff `n ≤ 3024` **and**
   `degree(topology) ≤ 4`. These are surfaced in Sleela as `sysdepth()` (3024)
   and `degreemax()` (4).

### Article II — Congrains grouping (the binding constraints)

In `SHEET.sheet` order (`section congrains`):

| # | Constraint      | Ordinance                                                        |
|---|-----------------|-------------------------------------------------------------------|
| 1 | `Invariant`     | a condition that must always hold for an object                   |
| 2 | `Precondition`  | what must be true before an operation runs                        |
| 3 | `Postcondition` | what the operation promises afterward                             |
| 4 | `Constraint`    | a restriction on legal object states/relations                    |
| 5 | `Dependency`    | a binding one object has on another (co-grained)                  |
| 6 | `Coupling`      | strength of inter-object binding (keep it loose)                  |
| 7 | `Congruence`    | structural sameness enabling congruent-linear connection          |
| 8 | `Contract`      | the bundle of congrains an object honors                          |

### Article III — Limits grouping (the bounding constraints)

In `SHEET.sheet` order (`section limits`):

| # | Limit             | Ordinance                                                      |
|---|-------------------|-----------------------------------------------------------------|
| 1 | `Bound`           | an inclusive/exclusive extent on a value                        |
| 2 | `Quota`           | a permitted amount over a scope/time                            |
| 3 | `RateLimit`       | cap on operations per unit time                                 |
| 4 | `Timeout`         | a limit on how long an operation may take                       |
| 5 | `Capacity`        | the maximum a container/resource may hold                       |
| 6 | `Threshold`       | a boundary value that changes behavior when crossed             |
| 7 | `DepthLimit`      | maximum reachable depth — here the System depth is **3024**     |
| 8 | `ComplexityDegree`| the connection order between systems — capped at **4**          |

### Article IV — Health grouping (system-health metrics)

System **Health** is the grouping of runtime-condition constraints the system
watches (drawn from the catalog's `attention` and `gain` roles). A system is
*healthy* while all hold:

| Metric        | Constraint                                                          |
|---------------|--------------------------------------------------------------------|
| `Watchdog`    | no line of execution is stalled (heartbeat within its `Timeout`)   |
| `Monitor`     | observed metrics stay within their `Threshold`s                    |
| `Throughput`  | work completed per unit time stays above its floor                 |
| `Capacity`    | no `Resource` exceeds its `Capacity` / high-water mark             |
| `Exception`   | unhandled exceptions do not cross a system boundary                |

### Article V — IQ grouping (system insight / quality)

System **IQ** is the grouping of insight/quality constraints — how well the
system *understands and expresses itself* (drawn from the `gain` and
`congruence` roles and Sleela's conducted-method insights). Higher IQ = more of
these satisfied:

| Metric        | Constraint                                                          |
|---------------|--------------------------------------------------------------------|
| `Insight`     | every conducted object resolves an `insight(...)` (a known gloss)  |
| `Congruence`  | related objects `route(...)` to a known congruence (no dead ends)  |
| `Coupling`    | inter-object coupling stays loose (Article II·6)                   |
| `Contract`    | each object's pre/post/invariant `Contract` is stated              |
| `Optimization`| a defined objective (`gain`) is being measured and improved        |

Query these from Sleela via the conducted methods (`insight`, `role`,
`congruent`, `route`, `sysdepth`, `degreemax`) and from Nordshrift via
`nordshrift objects` / `nordshrift relevance`.
