# Pi Generator

A complete, concise SLeeLa program that computes the decimal digits of **Pi**
and streams them to a local file.

- Source: [`PiGenerator.sleela`](PiGenerator.sleela)
- Output file: `pi-1000-digits.txt` (created in the working directory)
- Digit count: the `DIGITS` constant at the top of the class — set to
  **1,000**, a value that both allocates and completes on this VM in about a
  second and whose output matches an independent reference exactly. Raise it to
  produce more digits (read the performance note first).

## Algorithm

The source declares `#sleela 2.9` so it is accepted by both the older
`Sleelvac™` compiler (supported range 1.0 – 2.9) and the current
`SLeeLa™ Language/Runtime` build (1.0 – 3.0). It uses no 3.0-only syntax — only
integer arithmetic, `int[]`, `while`, and the `openFile`/`write`/`close` file
API, all available in 2.x.

It uses the **Rabinowitz–Wagon spigot**, which emits Pi's digits one at a time
using only bounded integer arithmetic over a single `int[]` array. That matters
here: SLeeLa's runtime has no working arbitrary-precision/bignum type (the
widest genuine integer is 128-bit), so a method that needs no big-integer type
is the right fit. Digits are written to the file in fixed-size chunks via
`openFile` / `write` / `close`, so memory stays flat instead of building one
enormous string.

## Run it

```sh
# from the repository root, with the source-built compiler
impl/build/sleela run pi-generator/PiGenerator.sleela
# or the prebuilt toolchain (supports syntax 1.0 – 3.0):
working-binaries/sleela run pi-generator/PiGenerator.sleela
```

To validate quickly, lower `DIGITS` (e.g. `1000` or `10000`) and run again — a
small count finishes in seconds and the file begins `3.14159…`.

## Verified correct

The output was checked against an independent reference (Chudnovsky via Python
`decimal`) and matches **exactly**:

- 50 digits → `3.1415926535897932384626433832795028841971693993751`
- 1,000 digits → identical to the reference, character for character.

## Honest performance note (please read)

The `DIGITS` constant ships at **1,000** so the program runs to completion out of
the box. The algorithm is correct at any digit count, but a very large target
such as 7,000,000 **will not complete in practice on this VM**, for two measured
reasons — and `main()` now guards both so a raised `DIGITS` fails gracefully
instead of crashing:

1. **The VM is an interpreted bytecode engine with no JIT, and `int[]`
   element access is super-linear.** Measured on this machine: 1,000 digits
   finish in ~1 s, 10,000 digits take ~48 s, and 100,000 does not finish within
   a couple of minutes. A 7,000,000-digit spigot needs an array of ~23 million
   cells, each touched on every one of 7,000,000 passes (~10¹⁴ accesses).
2. **The array store refuses large allocations.** A single array is hard-capped
   at `SL_MAX_ARRAY_LEN = 16,777,216` (2²⁴) cells, and very large allocations
   are rejected below that too. The ~23-million-cell state a 7,000,000-digit run
   needs is above the cap, so it cannot be allocated as written. `main()` now
   detects this, prints a clear message, and caps the digit count rather than
   dying with the raw runtime error:
   `array allocation failed (negative/too-large size or store exhausted)`.

In other words: the program is a faithful, correct Pi-to-file generator. To
produce a file within a reasonable time, keep `DIGITS` in the reachable range
(hundreds to a few thousand). Reaching millions of digits would require either a
faster execution path for array access in the VM or a native big-integer helper
to do the heavy arithmetic; that is a runtime capability, not a defect in this
program.

**Max Rupplin — MEARVK LLC — 2026**
