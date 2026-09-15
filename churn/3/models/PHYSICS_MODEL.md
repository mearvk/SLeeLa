# /3 · Physics model — Facets of physical law

The **physics model** reads *facets* of physical law: each entry is one face a
physical system presents when the defining constants of the SI are combined for
a particular quantity. Where `/2` (Structures) held the primary standing
constants (`c`, `h`, `e`, …), `/3` (Facets) holds the **derived faces** they
show — coupling strengths, spectroscopic scales, and quantum-electrical ratios.

## Data

[`../data/physics-facets.json`](../data/physics-facets.json) — derived and
related CODATA 2022 constants, and [`../data/si-derived-units.xml`](../data/si-derived-units.xml)
— SI coherent derived units with special names (faces of the seven base units).

| Facet | Symbol | Value | Kind |
|---|---|---|---|
| Fine-structure constant | α | 7.297 352 5643 × 10⁻³ | MEASURED (EM coupling) |
| Inverse fine-structure | 1/α | 137.035 999 177 | MEASURED |
| Rydberg constant | R∞ | 1.097 373 156 8157 × 10⁷ /m | MEASURED |
| Bohr radius | a₀ | 5.291 772 105 44 × 10⁻¹¹ m | MEASURED |
| Electron volt | eV | 1.602 176 634 × 10⁻¹⁹ J | EXACT (via defined `e`) |
| Josephson constant | K_J | 483 597.848 4 × 10⁹ Hz/V | EXACT |
| von Klitzing constant | R_K | 25 812.807 45 Ω | EXACT |
| Molar gas constant | R | 8.314 462 618 J/mol/K | EXACT (= N_A · k) |
| Stefan–Boltzmann constant | σ | 5.670 374 419 × 10⁻⁸ W/m²/K⁴ | EXACT |

## The distinction the model keeps

A facet is a **face**, not the whole object. Some faces are **EXACT** because
they follow by definition from the defining constants of the SI (`eV`, `R`,
`K_J`, `R_K`, `σ`). Others — the fine-structure constant, the Rydberg constant,
the Bohr radius — remain **MEASURED**: they are faces we determine by
experiment and quote with a standard uncertainty. The model tags each so a
derived-but-exact face is never confused with a measured one — `FACT ≠ MODEL`.

## Provenance

CODATA 2022 recommended values (Mohr, Newell, Taylor, Tiesinga), published by
NIST (physics.nist.gov/constants; 2022 SI wallet card); SI derived units from
the BIPM SI Brochure, 9th edition (2019). Values transcribed from these
authoritative public sources.
