<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Thermodynamics IV — Curriculum

A novice-to-advanced ladder of the **Thermodynamics IV** module's equations.
Each rung is the next logical step (*sequitur*) after the one before it; the
upper rungs (statistical mechanics, 3D PDEs, stochastic methods) are what make
this a 4th-level ("IV") treatment rather than an introduction.

- **`EQUATIONS.md`** — the equations as readable math, Level 0 → Level 6, each
  with the one new idea it introduces.
- **`SL*.sleela`** — the same equations encoded as SLeeLa (`#sleela 1.3`)
  classes, one file per rung.

| Level | SLeeLa file | Equation |
|------:|-------------|----------|
| 0 | `SLHeatLinear.sleela` | `Q = m c ΔT` |
| 1 | `SLFourierFlux.sleela` | `q = -k ΔT/Δx` |
| 2 | `SLCarnot.sleela` | `η = 1 - Tc/Th` |
| 3 | `SLNewtonCooling.sleela` | `T(t) = T_env + (T₀-T_env) e^(−rt)` |
| 4 | `SLBoltzmann.sleela` | `p_i = e^(−E_i/kT)/Z` |
| 5 | `SLHeatEquation3D.sleela` | `∂T/∂t = α ∇²T` |
| 6 | `SLArrhenius.sleela` | `k = A e^(−Ea/RT)` |

## Notes on the SLeeLa encoding
- The SLeeLa 1.3 surface has `+ - * / %` but no built-in `exp`/`sqrt`. Levels
  0–2 and 5 are expressed exactly with those operators.
- Levels 3, 4, and 6 need `exp`. Following the repo convention that native
  work crosses an explicit bridge, each such file:
  - carries a self-contained `expSeries(x)` (truncated Taylor series) so the
    SLeeLa class runs standalone for teaching/testing, and
  - documents the authoritative native entry point in `lib/thermodynamics`
    (the C functions `sl_thermo_*`) for production-accuracy results.
- These are library-definition classes (like `lib/core/*.sleela`): each defines
  a type and has no `main`; a `main` is only required when classes are linked
  into a runnable program.

The authoritative, fully-tested implementations of every equation here live in
the parent module (`thermodynamics/` C and C++ sources).

---
_Max Rupplin - MEARVK LLC - 2026_