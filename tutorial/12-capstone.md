# Lesson 12 — Putting It Together (Capstone)

> Series: [index](README.md) · prev: [11](11-http3-and-colors.md)

Time to combine the whole series into one small module: write a Wrapper, drive
it with a sheet, build the runnable artifact, get its `.ledger`, and verify it.

## 1. The Wrapper

`capstone/src/Ledgerize.sleela`:

```sleela
#sleela 1.2
// A tiny module that sums a window of work — the "throughput" the demo sheet
// declares as a subject. Deterministic, no I/O beyond print.
class Ledgerize {

    int sumWindow(int completed, int window) {
        // guard: an empty window has no throughput
        if (window <= 0) {
            return 0;
        }
        return completed / window;
    }

    void main() {
        int completed = 120;
        int window    = 4;
        print("throughput = " + sumWindow(completed, window) + " ops/second");
        print("done");
    }
}
```

## 2. The sheet

`capstone/build.sst`:

```sst
#nordshrift 1.0
#sleela     1.2

sheet capstone:
  version 1.0.0
  description "Capstone: build a runnable Sleela artifact and ledger it."

source:
  root  "src"
  glob  "**/*.sleela"
  exclude [ "build/**", "**/build/**" ]

target:
  root            "out"
  target-language sleela
```

## 3. Build (verified toolchain)

```sh
cd impl && export SLEELA_SHA256_MANIFEST="$PWD/../security/sha256-manifest.json"
make build/nordshrift          # Lesson 09: fail-closed build
cd ..
impl/build/nordshrift check capstone/build.sst
impl/build/nordshrift build capstone/build.sst
```

Expected (Lessons 05–06, 10): a runnable artifact **and** its ledger:

```text
nordshrift: capstone/src/Ledgerize.sleela -> capstone/src/build/Ledgerize.sleela (runnable Sleela Core artifact)
nordshrift: capstone/src/build/Ledgerize.sleela -> ...Ledgerize.sleela.ledger (+ .qr.svg)
```

## 4. Verify the ledger

```sh
python3 ledger/ledger.py verify capstone/src/build/Ledgerize.sleela.ledger
# ledger: OK — 1 records, chain intact
```

## What you exercised

| Lesson | In the capstone |
|---|---|
| 02–04 | the Wrapper: class, method, `if`, `print` |
| 05 | the `.sst` sheet driving the build |
| 06 | the `sleela` triplet target -> runnable artifact |
| 08 | the "throughput" quantity idea from the subject model |
| 09 | the SHA-256-gated, fail-closed build |
| 10 | the emitted `.ledger` + QR insignia, verified |

## Where to go next

- Retarget the capstone to `java` or `c` (Lesson 06) and read the emitted source.
- Give a service an HTTP **color** and wire it to the pipeline (Lesson 11).
- Add a `subject` block to the sheet and inspect the component manifest (Lesson 08).
- Explore the deeper docs: [`../ARCHITECTURE.md`](../ARCHITECTURE.md),
  [`../NORDSHRIFT.md`](../NORDSHRIFT.md), [`../impl/DESIGN.md`](../impl/DESIGN.md).

That's SLeeLa, source-first: a governed Wrapper, a sheet that drives it, three
faithful targets, a self-describing model, a fail-closed build, and a
tamper-evident ledger — end to end.

*(This capstone is a worked example in Markdown; create the two files under
`capstone/` if you want to run it live.)*
