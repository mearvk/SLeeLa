<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Thermodynamics Courses — Algorithms (SLeeLa source)

The standard thermodynamics course sequence encoded as algorithms, one
`#sleela 1.3` class per course. Every method is a real, named textbook relation;
transcendental terms (`exp`, `ln`) that the SLeeLa 1.3 surface lacks are
provided as self-contained series, with the native core
(`lib/thermodynamics` C/C++) authoritative for production accuracy.

## Core sequence

### `SLThermoI.sleela` — Thermodynamics I (Introductory)
Basic concepts, pure-substance properties, and the laws for closed/open systems.
| Algorithm | Relation |
|---|---|
| Zeroth law (transitive equilibrium) | A~B, B~C ⇒ A~C |
| Ideal-gas property | `P V = m R T` |
| First law (closed) | `ΔU = Q − W` |
| First law (open, steady flow) | `Q − W = ṁ (h_out − h_in)` |
| Second law (entropy from heat) | `dS = Q / T` |
| Second law (isothermal ideal gas) | `ΔS = m R ln(V₂/V₁)` |
| Clausius inequality classifier | `∮ dQ/T ≤ 0` |

### `SLThermoII.sleela` — Thermodynamics II (Applied)
Power/refrigeration cycles, gas mixtures, reactions.
| Algorithm | Relation |
|---|---|
| Rankine efficiency | `η = ((h₁−h₂) − (h₄−h₃)) / (h₁−h₄)` |
| Brayton efficiency | `η = 1 − q_out/q_in` (and `1 − rp^(−(γ−1)/γ)`) |
| Vapor-compression COP | `COP_R = (h₁−h₄)/(h₂−h₁)` |
| Heat-pump COP | `COP_HP = COP_R + 1` |
| Gas mixture (Dalton/Amagat) | mole fraction, apparent `M`, partial pressure |

## Advanced & specialized

### `SLStatisticalMechanics.sleela` — Statistical Thermodynamics
Microscopic → macroscopic via Boltzmann.
| Algorithm | Relation |
|---|---|
| Boltzmann factor | `e^(−E/k_BT)` |
| Partition function (degenerate levels) | `Z = Σ gᵢ e^(−Eᵢ/k_BT)` |
| Occupation probability | `pᵢ = gᵢ e^(−Eᵢ/k_BT) / Z` |
| Average energy | `⟨E⟩ = Σ pᵢ Eᵢ` |
| Boltzmann entropy | `S = k_B ln W` |
| Gibbs/Shannon entropy | `S = −k_B Σ pᵢ ln pᵢ` |

### `SLAdvancedChemicalThermo.sleela` — Advanced / Chemical Thermodynamics
Multicomponent equilibrium, kinetics, nonequilibrium.
| Algorithm | Relation |
|---|---|
| Equilibrium constant | `K = e^(−ΔG°/RT)` |
| Gibbs energy of reaction | `ΔG = ΔH − T ΔS` |
| van't Hoff slope | `d(ln K)/d(1/T) = −ΔH/R` |
| Arrhenius kinetics | `k = A e^(−Eₐ/RT)` |
| Onsager reciprocity | `L_ij = L_ji` |
| Entropy production | `σ = Σ Jᵢ Xᵢ ≥ 0` |
| Coupled flow | `Jᵢ = Σ L_ij X_j` |

---
_Max Rupplin - MEARVK LLC - 2026_