# Sleela Native API

## Module Model

Sleela uses explicit imports for native facilities. The source form is:

```sleela
import math;
import physics;
import economics;
import excel;
```

A module-qualified call is written as:

```sleela
math.sqrt(x)
physics.kinetic_energy(m, v)
economics.npv(cashflows, rate)
excel.open("input.xlsx")
```

The compiler maintains a native module registry so dependencies are explicit and the global namespace remains small.

## Current Native Module Registry

The compiler recognizes these module namespaces:

| Module | Purpose | API status |
|---|---|---|
| `math` | scalar mathematics, transforms, groups, numerical objects | contract established |
| `physics` | constants, mechanics, fields, thermodynamics, relativity, quantum-aware models | contract established in `PHYSICS.md` |
| `economics` | valuation, growth, elasticity, equilibrium, macro identities | contract established in `ECONOMICS.md` |
| `excel` | workbook and worksheet I/O | reserved for Excel implementation |
| `json` | structured data | reserved |
| `crypto` | cryptographic APIs | reserved |
| `net` | network APIs | reserved/overlaps existing network builtins |

Unknown modules are rejected by Sleelvac. Duplicate imports are rejected.

## Math API

The native mathematics namespace is intended to include:

```text
math.abs
math.sign
math.min
math.max
math.clamp
math.floor
math.ceil
math.round
math.trunc
math.fmod
math.sqrt
math.cbrt
math.pow
math.exp
math.exp2
math.log
math.log2
math.log10
math.sin
math.cos
math.tan
math.asin
math.acos
math.atan
math.atan2
math.sinh
math.cosh
math.tanh
math.hypot
math.isfinite
math.isnan
math.isinf
math.fourier
math.inverse_fourier
math.laplace
math.inverse_laplace
```

Standard constants include `PI`, `TAU`, `E`, and `SQRT2`, with transform objects and mathematical objects added as the object layer matures.

## Physics API

Representative calls:

```text
physics.velocity(dx, dt)
physics.acceleration(dv, dt)
physics.kinematic_position(x0, v0, a, t)
physics.force(m, a)
physics.momentum(m, v)
physics.kinetic_energy(m, v)
physics.gravitational_force(m1, m2, r)
physics.escape_velocity(M, r)
physics.lorentz_gamma(v)
physics.relativistic_energy(m, v)
physics.electrical_power(V, I)
physics.ideal_gas_pressure(n, R, T, V)
physics.wave_number(lambda)
physics.angular_frequency(f)
physics.neutrino_oscillation_probability(theta, delta_m2, L, E)
```

Constants include `C`, `H`, `HBAR`, `E_CHARGE`, `K_B`, `N_A`, `G`, `G0`, `R`, `EPSILON_0`, and `MU_0`.

## Economics API

Representative calls:

```text
economics.future_value(pv, rate, periods)
economics.present_value(fv, rate, periods)
economics.effective_annual_rate(rate, compounds)
economics.compound_growth(initial, rate, periods)
economics.real_value(nominal, price_index)
economics.elasticity(dQdP, P, Q)
economics.marginal_cost(dTCdQ)
economics.marginal_revenue(dTRdQ)
economics.npv(cashflows, discount_rate)
economics.irr(cashflows)
economics.consumer_surplus(demand, price)
economics.producer_surplus(supply, price)
economics.linear_equilibrium(a, b, c, d)
```

The API should distinguish accounting identities from empirical assumptions and preserve model provenance for Review objects.

## Compiler and Runtime Boundary

The architecture is:

**source import → module registry → native API resolution → Core lowering → runnable `.sleela` artifact**

The import grammar and compiler registry are now present. The native registry is intentionally separate from the Core VM so that native functions can be added without polluting the language grammar.

Native operations that require new VM instructions or external libraries must not be represented as implemented merely because their names appear in the API contract. Such operations should be added to Core with explicit opcode/serialization support and then marked implemented.

## Reasoning Contract

For advanced readers, every native scientific/economic function should eventually expose:

1. domain conditions;
2. units or dimensional expectations;
3. numerical stability notes;
4. approximation regime;
5. provenance of constants;
6. failure/convergence semantics;
7. relationship to mathematical objects and Review objects.

This is the bridge from a convenient formula library to a serious computational reasoning system.
