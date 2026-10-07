# thermodynamics

A small, self-contained thermodynamics vignette for SLeeLa, written in C
(the ABI) and C++ (the orchestration layer), following the repository's
`C ABI + C++ orchestration` convention.

Every function implements a standard, textbook relation, so the code is real,
compiles cleanly under `-Wall -Wextra -Wpedantic`, and runs a self-test that
asserts the results against known physics.

## Equations implemented

### Scalar / lumped-parameter
| Quantity | Equation |
|---|---|
| Sensible heat | `Q = m c ΔT` |
| Fourier's law (1D) | `q = -k dT/dx` |
| Newton's law of cooling | `T(t) = T_env + (T0 - T_env) e^(-r t)` |
| Carnot efficiency | `η = 1 - T_c / T_h` |

### Statistical mechanics
| Quantity | Equation |
|---|---|
| Partition function | `Z = Σ_i exp(-E_i / k_B T)` |
| Boltzmann probability | `p_i = exp(-E_i / k_B T) / Z` |
| Mean internal energy | `⟨E⟩ = Σ_i E_i p_i` |

### Multivariable calculus for 3D thermal models
These operate on a uniform 3D grid (`sl_thermo_field3d`) and are the vector-
calculus core used to model heat in a solid:

| Operator | Discretization |
|---|---|
| Gradient `∇T` | central differences on each axis |
| Divergence `∇·F` | central differences, summed |
| Laplacian `∇²T = ∇·(∇T)` | 7-point stencil |
| Heat equation `∂T/∂t = α ∇²T` | explicit FTCS step |

The FTCS stepper enforces the 3D stability limit `dt ≤ h² / (6α)` and holds
Dirichlet boundaries fixed.

### Stochastic extension — time you do not control
The deterministic heat equation fixes every future exactly. Real systems are
driven by fluctuations outside your control, so this extension models the
randomness and quantifies the resulting outcomes:

| Concept | Method |
|---|---|
| Stochastic heat equation | Langevin form `∂T/∂t = α∇²T + σ·ξ(t)`, Gaussian white noise `ξ` |
| Random sampling | splitmix64 PRNG + Box–Muller normal draws (seedable, reproducible) |
| Distribution over futures | Monte Carlo: run N randomized simulations → mean, variance, 95% CI |
| Temperature-driven rate | Arrhenius `k = A·exp(−Eₐ/RT)` (and its inverse for T) |

The Monte Carlo driver uses Welford's online algorithm for a numerically
stable mean/variance across runs. With `σ = 0` the stochastic step reduces
exactly to the deterministic one — the self-test asserts this.

## Layout
```
thermodynamics/
  include/thermodynamics.h    public C ABI
  src/thermodynamics.c        C implementation (the deterministic physics)
  src/thermo_stochastic.c     stochastic extension (noise, Monte Carlo, Arrhenius)
  src/thermodynamics.cpp      C++17 RAII orchestration over the C ABI
  src/vignette_demo.c         deterministic worked example + self-test
  src/stochastic_demo.c       stochastic self-test (noise, Monte Carlo, Arrhenius)
  Makefile
```

## Build & run
```sh
make            # build objects, run the self-test, print the sanity summary
make selftest   # build and run the worked-example assertions only
make clean
```

The self-test walks a worked "heating a copper bar" example end to end:
sensible heat, conductive flux, cooling to equilibrium, Carnot efficiency, a
two-level Boltzmann system, and a hot spot diffusing through a 5×5×5 cube via
the 3D heat equation.

---
Max Rupplin - MEARVK LLC - 2026
