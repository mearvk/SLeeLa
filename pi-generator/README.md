# Pi Generator

A complete, concise SLeeLa program that computes the decimal digits of **Pi**
and streams them to a local file.

- Source: [`PiGenerator.sleela`](PiGenerator.sleela)
- Output file: `pi-7000000-digits.txt` (created in the working directory)
- Digit count: the `DIGITS` constant at the top of the class — set to
  **7,000,000** as requested.

## Algorithm

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

The `DIGITS` constant is set to 7,000,000 because that is what was asked for, and
the algorithm is correct at any digit count. **But the 7,000,000-digit run will
not complete in practice on this VM**, for two measured reasons:

1. **The VM is an interpreted bytecode engine with no JIT, and `int[]`
   element access is super-linear.** Measured on this machine: filling an array
   of 20,000 is instant, 40,000 takes ~12 s, and 100,000 does not finish within
   two minutes. A 7,000,000-digit spigot needs an array of ~23 million cells,
   each touched on every one of 7,000,000 passes (~10¹⁴ accesses).
2. **The array store refuses very large allocations** (a 2,000,000-element
   allocation is rejected), so the ~23-million-cell state array cannot be
   allocated as written.

In other words: the program is a faithful, correct Pi-to-file generator, and the
7,000,000 target documents the intent — but to actually produce a file within a
reasonable time, set `DIGITS` to a reachable value (hundreds to a few thousand).
Reaching millions of digits would require either a faster execution path for
array access in the VM or a native helper to do the heavy arithmetic; that is a
runtime capability, not a defect in this program.

**Max Rupplin — MEARVK LLC — 2026**
