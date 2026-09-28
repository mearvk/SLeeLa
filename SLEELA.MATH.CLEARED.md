# SLEELA · MATH · CLEARED

**A National Document of Mathematical Clearance for the SLeeLa `math` Subject Library**

> This document is an evidentiary clearance record. It reports the PASS/FAIL
> status of SLeeLa's native mathematics code as measured by two independent
> harnesses on the commit named below. Every result in this file was produced
> by running the code — nothing here is asserted without a corresponding run.
> It certifies the *software system's* numerical accuracy; it makes no claim
> about any person or population (see [`MATH.md`](MATH.md), "Purpose").

---

## Clearance record

| Field | Value |
|---|---|
| **Subject** | SLeeLa native `math` subject library |
| **Status** | ✅ **CLEARED** |
| **Date of clearance** | 2026-09-20 (UTC) |
| **Commit** | `ba8ab24` (`ba8ab24577081e700f8b594e9d1b020ea1b7e113`) |
| **Runtime** | Sleelvac™ 1.4 (SHA-256 execution gate active) |
| **Reference authority** | Python 3.9 `math` module (independent computation) |
| **Integrity gate** | SHA-256 manifest verified: PASS (94 items) |

Two harnesses were run. Both must be green for clearance; both are green.

| Harness | Cases | PASS | FAIL | Result |
|---|---|---|---|---|
| In-tree numeric assertions (`math_values.sleela`) | 27 | 27 | 0 | ✅ PASS |
| Independent accuracy checker (`check-math-accuracy.py`) | 32 | 32 | 0 | ✅ PASS |

---

## Article I — In-tree numeric assertions

Source: [`impl/tests/subjects/math_values.sleela`](impl/tests/subjects/math_values.sleela),
run through the built `sleela` binary. Each line compares a computed value
against a reference within tolerance and emits a stable `PASS`/`FAIL` token.
`EXPECTED_PASS=27`; observed PASS = 27; observed FAIL = 0.

| # | Check | Result |
|---|---|---|
| 1 | `sqrt2` | PASS |
| 2 | `sqrt_144` | PASS |
| 3 | `pow_2_10` | PASS |
| 4 | `exp_0` | PASS |
| 5 | `exp_1` | PASS |
| 6 | `log_e` | PASS |
| 7 | `sin_0` | PASS |
| 8 | `sin_pi_half` | PASS |
| 9 | `cos_0` | PASS |
| 10 | `abs_neg` | PASS |
| 11 | `min` | PASS |
| 12 | `max` | PASS |
| 13 | `clamp_hi` | PASS |
| 14 | `clamp_lo` | PASS |
| 15 | `hypot_3_4` | PASS |
| 16 | `fmod` | PASS |
| 17 | `pi` | PASS |
| 18 | `log_1000` | PASS |
| 19 | `log10_1000` | PASS |
| 20 | `log10_82100` | PASS |
| 21 | `floor_pos` | PASS |
| 22 | `floor_neg` | PASS |
| 23 | `floor_whole` | PASS |
| 24 | `ceil_pos` | PASS |
| 25 | `ceil_neg` | PASS |
| 26 | `trunc_pos` | PASS |
| 27 | `trunc_neg` | PASS |

**Article I result: 27 / 27 PASS, 0 FAIL.**

---

## Article II — Independent accuracy checker

Source: [`tools/check-math-accuracy.py`](tools/check-math-accuracy.py). Every
reference value is computed *independently in Python* — not read from the
`.sleela` source — so this is a genuine cross-check of SLeeLa's output, not a
re-reading of embedded literals. Relative tolerance `1e-5` (absolute floor
`1e-6`), which honors SLeeLa's ~6-significant-figure `print` rendering.

`got` is what the SLeeLa runtime printed; `reference` is Python's value;
`|diff|` is the absolute difference.

| # | Case | got | reference | \|diff\| | Result |
|---|---|---|---|---|---|
| 1 | `sqrt2` | 1.41421 | 1.41421 | 3.56e-06 | PASS |
| 2 | `sqrt_144` | 12.0 | 12 | 0 | PASS |
| 3 | `pow_2_10` | 1024.0 | 1024 | 0 | PASS |
| 4 | `pow_frac` | 3.0 | 3 | 0 | PASS |
| 5 | `exp_0` | 1.0 | 1 | 0 | PASS |
| 6 | `exp_1` | 2.71828 | 2.71828 | 1.83e-06 | PASS |
| 7 | `exp_3` | 20.0855 | 20.0855 | 3.69e-05 | PASS |
| 8 | `log_e` | 1.0 | 1 | 0 | PASS |
| 9 | `log_1000` | 6.90776 | 6.90776 | 4.72e-06 | PASS |
| 10 | `log10_1000` | 3.0 | 3 | 0 | PASS |
| 11 | `log10_82100` | 4.91434 | 4.91434 | 3.16e-06 | PASS |
| 12 | `sin_0` | 0.0 | 0 | 0 | PASS |
| 13 | `sin_1` | 0.841471 | 0.841471 | 1.52e-08 | PASS |
| 14 | `sin_pi_half` | 1.0 | 1 | 0 | PASS |
| 15 | `cos_0` | 1.0 | 1 | 0 | PASS |
| 16 | `cos_1` | 0.540302 | 0.540302 | 3.06e-07 | PASS |
| 17 | `hypot_3_4` | 5.0 | 5 | 0 | PASS |
| 18 | `hypot_5_12` | 13.0 | 13 | 0 | PASS |
| 19 | `fmod_7p5_2` | 1.5 | 1.5 | 0 | PASS |
| 20 | `fmod_10_3` | 1.0 | 1 | 0 | PASS |
| 21 | `abs_neg` | 7.25 | 7.25 | 0 | PASS |
| 22 | `pi` | 3.14159 | 3.14159 | 2.65e-06 | PASS |
| 23 | `floor_pos` | 3.0 | 3 | 0 | PASS |
| 24 | `floor_neg` | -4.0 | -4 | 0 | PASS |
| 25 | `ceil_pos` | 4.0 | 4 | 0 | PASS |
| 26 | `ceil_neg` | -3.0 | -3 | 0 | PASS |
| 27 | `trunc_pos` | 3.0 | 3 | 0 | PASS |
| 28 | `trunc_neg` | -3.0 | -3 | 0 | PASS |
| 29 | `min` | 3.0 | 3 | 0 | PASS |
| 30 | `max` | 8.0 | 8 | 0 | PASS |
| 31 | `clamp_hi` | 10.0 | 10 | 0 | PASS |
| 32 | `clamp_lo` | 0.0 | 0 | 0 | PASS |

**Article II result: 32 / 32 accurate, 0 inaccurate, 0 missing.**

Largest observed deviation across all cases: `|diff| = 3.69e-05` (`exp_3`),
within the display precision of a ~6-significant-figure print.

---

## Article III — Reproduction

Any party may reproduce this clearance from a clean checkout of commit
`ba8ab24`:

```sh
# 1. Build the runtime (the SHA-256 gate must pass first).
cd impl
export SLEELA_SHA256_MANIFEST="$(cd .. && pwd)/security/sha256-manifest.json"
make

# 2. Article I — in-tree numeric assertions (expects 27 PASS, 0 FAIL).
make test-math

# 3. Article II — independent accuracy checker (expects MATH ACCURACY: PASS).
cd ..
python3 tools/check-math-accuracy.py --verbose
```

A non-zero exit from either step voids this clearance for the commit under test.

---

## Determination

On commit `ba8ab24`, the SLeeLa native `math` subject library produced **no
FAIL** across **59 total checks** (27 in-tree assertions + 32 independent
accuracy cases), with every value agreeing with an independent Python reference
to the runtime's display precision.

**The SLeeLa mathematics code is CLEARED.**

*Cleared by Kiro, AI software engineer, on the results of the runs recorded above.*
