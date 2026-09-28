# Tutorial Demos — 12 runnable Wrappers

One runnable `.sleela` demo per lesson in the [tutorial series](../README.md).
Each is a small, self-contained Wrapper™ that illustrates its lesson.

| Demo | Lesson |
|---|---|
| [`src/D01_Hello.sleela`](src/D01_Hello.sleela) | 01 — What SLeeLa is |
| [`src/D02_Greeter.sleela`](src/D02_Greeter.sleela) | 02 — Your first Wrapper™ |
| [`src/D03_Rect.sleela`](src/D03_Rect.sleela) | 03 — Types, structs & classes |
| [`src/D04_Counter.sleela`](src/D04_Counter.sleela) | 04 — Control flow & expressions |
| [`src/D05_Build.sleela`](src/D05_Build.sleela) | 05 — The `.sst` sheet & Nordshrift |
| [`src/D06_Triplet.sleela`](src/D06_Triplet.sleela) | 06 — The triplet: Java, Sleela, C |
| [`src/D07_Conducted.sleela`](src/D07_Conducted.sleela) | 07 — Conducted methods |
| [`src/D08_Subject.sleela`](src/D08_Subject.sleela) | 08 — Subjects & the sheet model |
| [`src/D09_Verify.sleela`](src/D09_Verify.sleela) | 09 — Verification & the SHA-256 gate |
| [`src/D10_Ledger.sleela`](src/D10_Ledger.sleela) | 10 — The `.ledger` class |
| [`src/D11_Colors.sleela`](src/D11_Colors.sleela) | 11 — HTTP 3.0 & HTTP colors |
| [`src/D12_Capstone.sleela`](src/D12_Capstone.sleela) | 12 — Putting it together |

## Build them all

From the repo root, with the verified toolchain built (see tutorial Lesson 09):

```sh
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
impl/build/nordshrift check tutorial/demos/build.sst
impl/build/nordshrift build tutorial/demos/build.sst
```

The `sleela` target writes a runnable artifact per demo under `src/build/`, and
a `.ledger` (+ `.qr.svg`) beside each — verify one with:

```sh
python3 ledger/ledger.py verify tutorial/demos/src/build/D10_Ledger.sleela.ledger
```

Retarget to `java` or `c` by changing `target-language` in
[`build.sst`](build.sst) (Lesson 06).

> All 12 sources compile cleanly through Nordshrift to runnable Sleela Core
> artifacts. The `src/build/` output is generated (git-ignored); only the
> sources and the sheet are tracked.
