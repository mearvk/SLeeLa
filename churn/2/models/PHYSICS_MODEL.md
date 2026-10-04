# /2 · Physics model — Structures of nature

The **physics model** reads the fundamental constants of nature as *Structures*:
standing quantities that hold a fixed magnitude in SI units. Where a Long runs
over time, a physical constant **stands** — it is the same today as yesterday.

## Data

[`../data/physics-constants.json`](../data/physics-constants.json) — selected
CODATA 2022 fundamental constants, and [`../data/si-base-units.xml`](../data/si-base-units.xml)
— the seven SI base units and the defining constant that fixes each.

The data file now holds **16 constants** (alphabetized by name); a representative
selection:

| Structure | Symbol | Value | Kind |
|---|---|---|---|
| Atomic mass constant | `m_u` | 1.660 539 069 × 10⁻²⁷ kg | MEASURED |
| Avogadro constant | `N_A` | 6.022 140 76 × 10²³ /mol | EXACT (defines the mole) |
| Boltzmann constant | `k` | 1.380 649 × 10⁻²³ J/K | EXACT (defines the kelvin) |
| Electron mass | `m_e` | 9.109 383 71 × 10⁻³¹ kg | MEASURED |
| Elementary charge | `e` | 1.602 176 634 × 10⁻¹⁹ C | EXACT (defines the ampere) |
| Faraday constant | `F` | 96 485.332 12 C/mol | EXACT (= N_A · e) |
| Gravitational constant | `G` | 6.674 30 × 10⁻¹¹ m³/kg/s² | MEASURED (has uncertainty) |
| Molar gas constant | `R` | 8.314 462 618 J/mol/K | EXACT (= N_A · k) |
| Neutron mass | `m_n` | 1.674 927 500 56 × 10⁻²⁷ kg | MEASURED |
| Planck constant | `h` | 6.626 070 15 × 10⁻³⁴ J·s | EXACT (defines the kilogram) |
| Proton mass | `m_p` | 1.672 621 926 × 10⁻²⁷ kg | MEASURED |
| Speed of light in vacuum | `c` | 299 792 458 m/s | EXACT (defines the metre) |
| Standard gravity | `g_n` | 9.806 65 m/s² | EXACT (conventional) |
| Stefan–Boltzmann constant | `σ` | 5.670 374 419 × 10⁻⁸ W/m²/K⁴ | EXACT |
| Vacuum permeability | `μ₀` | 1.256 637 061 × 10⁻⁶ N/A² | MEASURED (post-2019) |

## The distinction the model keeps

Since the **2019 SI redefinition**, several constants are **EXACT**: they are
definitions, fixed with no uncertainty, and the base units are derived *from
them*. Others (like `G`, `m_e`, `m_p`) remain **MEASURED** and carry a standard
uncertainty. The model tags each structure `EXACT` or `MEASURED` so a reader
never mistakes a definition for a measurement — `FACT ≠ MODEL`.

## Provenance

CODATA 2022 recommended values (Mohr, Newell, Taylor, Tiesinga), published by
NIST (physics.nist.gov/constants) and cross-referenced with the Particle Data
Group's physical-constants review; the complete set of seven SI base units from
the BIPM SI Brochure, 9th edition (2019). Values are transcribed from these
authoritative public sources.

---

*Copyright (C) 2026 MEARVK LLC*
*Author: Maximilian Eric Alexander Rupplin von Keffikon (Max Rupplin)*
