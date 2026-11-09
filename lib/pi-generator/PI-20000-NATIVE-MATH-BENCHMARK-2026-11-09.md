# SLeeLa Pi 20,000-Digit Native Math Benchmark

**Date:** 2026-11-09  
**Status:** PASS  
**Branch:** `pi-20000-sleelactl-native-math`  
**Test commit:** `2feed4fb627adc22196766f55dd7706fa9812773`  
**Workflow run:** [Pi 20,000-digit native math benchmark](https://github.com/mearvk/SLeeLa/actions/runs/42690168886)

## Result

SLeeLa generated **20,000 fractional digits of π** with the Chudnovsky implementation in `PiChudnovsky.sleela`. The complete output contains 20,002 ASCII characters (`3.` plus 20,000 fractional digits).

An independent `mpmath` reference, evaluated at 20,080 decimal places, was compared character-for-character with the SLeeLa output. Every digit matched.

- Verification: **PASS — all 20,000 fractional digits**
- Output SHA-256: `977f8d4251b4f633100b4d2dbc214b874c45353b0e3debe41046753777fd6efc`
- Prefix: `3.14159265358979323846264338327950288419716939937510`
- Final 52 characters: `2429681062037765788371669091094180744878140490755178`

## Execution and measurements

| Measurement | Result |
|---|---:|
| Runner OS | Ubuntu 26.04.1 LTS |
| Runtime build | PASS (`make -C impl clean all`) |
| Trusted manifest verification | PASS (12,499 items) |
| `sleelactl run math` native subject smoke test | PASS |
| Pi generation | PASS |
| Independent digit comparison | PASS |
| Wall-clock time for Pi generation | 10.72 s |
| User CPU time | 9.87 s |
| System CPU time | 0.84 s |
| CPU utilization | 99% |
| Peak resident memory | 49,724 KiB (about 48.6 MiB) |
| Process exit status | 0 |

The Pi generation command was:

```sh
timeout 30m impl/build/sleela run pi-generator/PiChudnovsky.sleela
```

The registered native math subject was separately exercised through:

```sh
sh bin/sleelactl run math
```

That distinction matters: `sleelactl run math` runs the native math subject demonstration; the arbitrary-precision Pi program is executed by the freshly built SLeeLa runtime. Both stages passed in the same CI job.

## Implementation under test

- Program: `pi-generator/PiChudnovsky.sleela`
- Algorithm: Chudnovsky series with binary splitting
- Precision target: 20,000 digits after the decimal point
- Arithmetic: SLeeLa VM arbitrary-precision integer operations (`bigmul`, `bigadd`, `bigsub`, `bigdiv`, `bigisqrt`, and `bigdivmod`)

## Reproduction

Run the workflow on branch `pi-20000-sleelactl-native-math`, or dispatch the workflow manually. The job regenerates the SHA-256 source manifest, builds the runtime, executes the native math subject smoke test, runs the Pi program, and compares its output against the independent `mpmath` reference. The workflow preserves the output, timing report, and verification summary as the `sleela-pi-20000-benchmark` artifact.

## Interpretation and limitations

This is a successful correctness and single-run performance result on the specified hosted Linux runner. The 10.72-second timing and memory figure are measurements of this run, not a cross-platform guarantee or a statistically characterized benchmark. No speedup ratio is claimed: comparisons with other algorithms or earlier scalar math tests would not be like-for-like without matched workloads, hardware, and measurement conditions.

The workflow and its artifacts are the authoritative record for this result.
