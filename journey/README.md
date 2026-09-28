# journey/ — the SLeeLa component register

This directory is the **runnable equivalent** of [`JOURNEY.md`](../JOURNEY.md):
a set of `.sleela` **register** documents that name every component of the
SLeeLa implementation, in the order a program travels through it, and run as a
roll-call. They are documented in the **SleelaDoc** brand.

| Register | Covers | Notes |
|---|---|---|
| [`register.sleela`](register.sleela) | **All tiers** — the master roll-call, in journey order, with a running count (54 components). | The one to run first. |
| [`register_frontend.sleela`](register_frontend.sleela) | Tier 1 — the C++ front end, in pipeline order. | |
| [`register_core.sleela`](register_core.sleela) | Tiers 2–4 — the stack VM, runtime services, and OS-abstraction layer. | |
| [`register_subjects.sleela`](register_subjects.sleela) | Tier 6 — the subject libraries. | Also runs a **live** `math.sqrt(144)==12` check. |
| [`register_nordshrift.sleela`](register_nordshrift.sleela) | Tier 7 — the Nordshrift `.sst` driver. | |
| [`register_catalog.sleela`](register_catalog.sleela) | Tier 5 — conducted methods, **resolved live against `SHEET.sheet`** at compile time. | `role`/`insight`/`congruent`/`route` are real queries. |

## Run

```sh
cd impl && make                          # build build/sleela
cd ..
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run journey/register.sleela
```

## Why runnable

A component list in prose can drift from the code. These registers are
*executed* on the same C/C++ core they describe, and the catalog register
answers its entries with real compile-time queries against `SHEET.sheet` — so
the roll-call is auditable, not merely asserted.

Note: the conducted built-ins (`conduct` / `role` / `insight` / `congruent` /
`route`) take **string-literal** object names because they are resolved at
compile time. That is why each catalogued entry is spelled out explicitly — a
register is an explicit roll-call by design, not a loop over a variable.
