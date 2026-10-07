# Thermodynamics IV — Equations, Novice to Advanced

The equation ladder for the **Thermodynamics IV** module. "IV" denotes the
4th-level scope: the ladder climbs from introductory scalar relations (Levels
0–2) through statistical mechanics (Level 4) and multivariable 3D PDEs (Level
5) to the stochastic methods (Level 6) that define the advanced tier. Each
level is a *sequitur* from the
one before: the new idea is exactly the next thing you need once the previous
level is understood. Every equation here is implemented in the module's C/C++
code and mirrored as a `.sleela` file in this folder.

> Reading guide: start at Level 0. Each level states the equation, names the
> new idea it introduces, and points to the `.sleela` file that encodes it.

---

## Level 0 — Linear: heat and temperature (for the novice)

The simplest useful relation. Everything is a straight line: double the mass or
the temperature change, double the heat.

**Sensible heat**
```
Q = m · c · ΔT
```
- `Q` heat added [J], `m` mass [kg], `c` specific heat [J/(kg·K)], `ΔT` temperature change [K]
- **New idea:** direct proportionality — a linear equation in each variable.
- SLeeLa: `SLHeatLinear.sleela`

**Linear temperature rise** (rearranged — solve for the thing you want)
```
ΔT = Q / (m · c)
```
- **New idea:** algebraic rearrangement of a linear law.

---

## Level 1 — Linear gradients: Fourier's law

Once heat is linear, ask: how fast does it *flow*? Over a small region the
temperature profile is a straight line, so the flow depends on its slope.

**Fourier's law (1D)**
```
q = -k · (ΔT / Δx)
```
- `q` heat flux [W/m²], `k` conductivity [W/(m·K)], `ΔT/Δx` temperature slope
- **New idea:** a *rate* is the slope of a line (a first derivative in disguise). The minus sign: heat flows from hot to cold.
- SLeeLa: `SLFourierFlux.sleela`

---

## Level 2 — Linear efficiency: the Carnot limit

Still linear in ratio form, but now a *bound*: the best any heat engine can do
between two temperatures.

**Carnot efficiency**
```
η = 1 - (T_cold / T_hot)
```
- Temperatures in kelvin. `η` is a pure number in [0, 1).
- **New idea:** a ratio of two quantities gives a dimensionless limit.
- SLeeLa: `SLCarnot.sleela`

---

## Level 3 — Exponential decay: Newton's cooling

The first *non*-linear step. Reality: the hotter you are above your
surroundings, the faster you cool — the rate depends on the current value, which
produces an exponential, not a line.

**Newton's law of cooling (closed form)**
```
T(t) = T_env + (T₀ - T_env) · e^(−r·t)
```
- `T₀` start temp, `T_env` ambient, `r` cooling rate [1/s], `t` time [s]
- **New idea:** `e^(−rt)` — exponential decay toward equilibrium. The jump from a line to a curve.
- SLeeLa: `SLNewtonCooling.sleela` (linear/structural part; the `e^x` term crosses to the native core)

---

## Level 4 — Statistical weighting: the Boltzmann distribution

Why does an exponential appear? Because at the microscopic level, states are
weighted by `e^(−E/kT)`. This is the engine behind temperature itself.

**Partition function and occupation probability**
```
Z   = Σ_i e^(−E_i / (k_B · T))
p_i = e^(−E_i / (k_B · T)) / Z
⟨E⟩ = Σ_i E_i · p_i
```
- `k_B` Boltzmann constant, `E_i` energy of level `i`, `Z` normalizer
- **New idea:** a *normalized* sum of exponentials turns energies into probabilities.
- SLeeLa: `SLBoltzmann.sleela`

---

## Level 5 — Multivariable calculus: the 3D heat equation

Now space is three-dimensional. The slope of Level 1 becomes a gradient; its
"slope of the slope" becomes the Laplacian; and heat evolves in time by it.

**Gradient (vector of partials)**
```
∇T = ( ∂T/∂x , ∂T/∂y , ∂T/∂z )
```
**Divergence of a vector field**
```
∇·F = ∂Fx/∂x + ∂Fy/∂y + ∂Fz/∂z
```
**Laplacian (divergence of the gradient)**
```
∇²T = ∂²T/∂x² + ∂²T/∂y² + ∂²T/∂z²
```
**Heat equation**
```
∂T/∂t = α · ∇²T
```
- `α` thermal diffusivity [m²/s]
- **New idea:** vector calculus — the gradient/divergence/Laplacian chain, and a partial differential equation evolving a 3D field in time.
- Discretization (what the code does): 7-point stencil; explicit FTCS step with stability limit `dt ≤ h² / (6α)`.
- SLeeLa: `SLHeatEquation3D.sleela`

---

## Level 6 — Stochastic calculus: futures you don't control

The deterministic heat equation fixes every future exactly. The next sequitur:
add the randomness real systems carry. One equation becomes a *distribution* of
equations.

**Stochastic (Langevin) heat equation**
```
∂T/∂t = α · ∇²T + σ · ξ(t)
```
- `σ` noise amplitude, `ξ(t)` Gaussian white noise
- **New idea:** add a random forcing term — the solution is now a random process, not a single curve.

**Monte Carlo over futures** (how you make it quantitative)
```
run N simulations → mean, variance, 95% CI = mean ± 1.96 · (σ̂ / √N)
```
- **New idea:** sample many futures and summarize them with a confidence interval.

**Arrhenius log-rate law** (the logarithm/engineering link)
```
k = A · e^(−E_a / (R · T))        ⇔        ln k = ln A − E_a / (R · T)
```
- `A` pre-factor, `E_a` activation energy [J/mol], `R` gas constant
- **New idea:** taking the logarithm turns an exponential rate law into a *straight line* in `1/T` — the ladder closes back to Level 0's linearity, one level up.
- SLeeLa: `SLArrhenius.sleela`

---

## The ladder at a glance

| Level | Equation | New idea |
|------:|----------|----------|
| 0 | `Q = m c ΔT` | linear proportionality |
| 1 | `q = -k ΔT/Δx` | rate = slope (first derivative) |
| 2 | `η = 1 - Tc/Th` | dimensionless ratio limit |
| 3 | `T(t) = T_env + (T₀-T_env) e^(−rt)` | exponential decay |
| 4 | `p_i = e^(−E_i/kT)/Z` | normalized sum of exponentials |
| 5 | `∂T/∂t = α ∇²T` | vector calculus + PDE in 3D |
| 6 | `∂T/∂t = α ∇²T + σ ξ(t)` | stochastic process + Monte Carlo |

Each row's "new idea" is the single concept you need to move up one rung.

---
_Max Rupplin - MEARVK LLC - 2026_
