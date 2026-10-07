# MATH.KNOWNS.md

Known quantities for the **Thermodynamics IV** module. This file separates two
kinds of numbers clearly:

1. **Assumptions (user-specified slots)** — caps you provided. They are recorded
   as stated; they are *not* derived from or validated by the physics in this
   module. They are labeled as assumptions on purpose.
2. **Defined constants (in code)** — real, named values the implementation
   actually uses, with their source.

---

## 1. Assumptions — user-specified slots

These are the "slots for the numbers we've given." They are upper bounds
(capacity limits) supplied as assumptions, not results.

| Slot | Meaning | Cap (not more than) |
|------|---------|---------------------|
| `FUTURES_MAX` | Sort all futures | **22** |
| `POSITIVE_GAINS_MAX` | Sort all positive gains | **6** |
| `LONG_TERM_CONFIDENCES_MAX` | Sort all long-term gains / confidences | **2** |

Notes:
- These are stated assumptions. No physical derivation backs the specific
  values; they define how many entries each category is allowed to hold.
- **These caps are now enforced in code.** They are defined as the macros
  `SL_THERMO_FUTURES_MAX` (22), `SL_THERMO_POSITIVE_GAINS_MAX` (6), and
  `SL_THERMO_LONG_TERM_CONFIDENCES_MAX` (2) in `include/thermodynamics.h`, and
  applied by the `sl_thermo_slots` collector (`src/thermo_slots.c`):
  - the futures slot is a **hard cap** — the 23rd offer is rejected (code 2);
  - the positive-gains and long-term slots are **keep-the-best caps** — they
    retain the strongest entries up to the limit.
  The self-test `src/slots_demo.c` asserts each cap holds.

---

## 2. Defined constants — actually used in the code

These are the real numeric values the C/C++ implementation uses, with sources.

| Constant | Value | Units | Where / meaning |
|----------|-------|-------|-----------------|
| `SL_THERMO_KB` | 1.380649e-23 | J/K | Boltzmann constant (SI exact) |
| `SL_THERMO_R` | 8.314462618 | J/(mol·K) | Molar gas constant (SI) |
| FTCS stability factor | 6 | — | 3D explicit stability limit `dt ≤ h² / (6·α)` |
| 95% CI z-score | 1.96 | — | Monte Carlo confidence interval half-width multiplier |
| PRNG increment | 0x9E3779B97F4A7C15 | — | splitmix64 golden-ratio step |

### Worked-example inputs used in the self-tests
These are illustrative sample values in the demos, not fundamental constants.

| Quantity | Value | Units | Used in |
|----------|-------|-------|---------|
| Copper specific heat `c` | 385 | J/(kg·K) | sensible-heat example |
| Copper conductivity `k` | 401 | W/(m·K) | Fourier-flux example |
| Copper thermal diffusivity `α` | 1.11e-4 | m²/s | heat-equation / Monte Carlo |
| Example activation energy `Eₐ` | 80000 | J/mol | Arrhenius example |
| Example pre-factor `A` | 1.0e13 | 1/s | Arrhenius example |

---

_Max Rupplin - MEARVK LLC - 2026_
