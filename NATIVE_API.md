# Sleela Native API

## Module Model

Sleela uses explicit imports for native facilities:

```sleela
import math;
import physics;
import economics;
import chemistry;
```

Calls are module-qualified. Sleelvac lowers executable scientific/economic/chemistry calls into ordinary Sleela Core methods before bytecode emission. Consequently a compiled `.sleela` artifact contains the implementation and the runtime does not invoke Sleelvac again.

## Native Module Registry

| Module | Purpose | Status |
|---|---|---|
| `math` | scalar mathematics and numerical kernels | executable subset implemented |
| `physics` | constants, mechanics, relativity, waves, selected neutrino model | executable subset implemented |
| `economics` | valuation, compounding, elasticity, growth and macro identities | executable subset implemented |
| `chemistry` | periodic subjects, ratios, bonds/valences, signals, similarity norms and bounded presumed inference | executable inference subset implemented |
| `excel` | workbook/worksheet I/O | reserved for Excel backend |
| `json` | structured data | reserved |
| `crypto` | cryptographic APIs | reserved |
| `net` | network APIs | reserved/overlaps existing network builtins |

Unknown modules and duplicate imports are rejected by Sleelvac. `chemistry` is handled by a dedicated frontend adapter so its inferred layer remains explicitly distinguishable from the established math/physics/economics registry.

## Executable Chemistry API

The current executable chemistry layer is deliberately evidence-oriented:

```text
chemistry.ratio(numerator,denominator)
chemistry.similarity(a,b)
chemistry.stochastic(seed,weight)
chemistry.inference_level(observation,bond,valence,similarity,signal,symmetry)
chemistry.confidence(observation,bond,valence,similarity,signal,symmetry)
chemistry.uncertainty(observation,bond,valence,similarity,signal,symmetry)
```

The inference level is bounded to the project's **0..24** semantic scale. It measures model support, not probability, IQ, educational attainment, or scientific certainty.

### Presumed Chemistry

For an experimental, newly proposed, or incompletely characterized subject, the chemistry layer maintains the distinction:

**observed != derived != inferred**

The intended causal-symmetry order is:

**observation → measurement → structure → bond → valence → geometry → electronic state → reaction relationship → compound-family similarity → thermodynamic consequence → astronomical context**

Ratios are first-class comparison objects. Similarity is relational compatibility rather than identity. Signals and conferrers support competing structural hypotheses. The current evidence weights are observation 0.24, bond 0.20, valence 0.22, similarity 0.13, signal 0.12, symmetry 0.09. These are inspectable project-model weights, not chemical laws.

The stochastic function is deterministic for a supplied seed and bounded so it can rank close candidates without manufacturing evidence or overriding contradictory observations.

## 0..24 Inference Norms

| Level | Interpretation |
|---:|---|
| 0 | Raw subject; insufficient evidence |
| 1–4 | Direct observations dominate |
| 5–8 | Elementary structural inference |
| 9–12 | Valence and bonding inference |
| 13–16 | Similarity and compound-family inference |
| 17–20 | Multi-property convergent inference |
| 21–23 | Strong convergent model support |
| 24 | Maximum model-supported inference; never experimental proof |

## Chemistry Data Library

The chemistry library is maintained under:

```text
impl/chemistry/chemistry.h
impl/chemistry/chemistry.cpp
impl/chemistry/periodic_table.model
impl/chemistry/presumed_chemistry.model
impl/frontend/chemistry_api.h
impl/frontend/chemistry_api.cpp
```

The periodic registry contains all 118 recognized elements. The declarative presumed-chemistry model records the evidence classes, ratio norms, causal ordering, stochastic rule, and epistemic boundaries.

## Normed Chemistry Description

The planned high-level description layer should produce 2–8 paragraphs according to available evidence, using ordered titles such as:

**Chemical Subject → Identity and Composition → Structural Presumption → Bond and Valence Norms → Signals and Similarity → Presumed Properties → Inference Status → Astronomical Norm**

The astronomical norm connects elemental identity to periodic and astrophysical context only where supported. It must not imply that an arbitrary laboratory compound has an independently established astronomical origin.

## Executable Math API

Implemented native calls include:

```text
math.pi()
math.e()
math.tau()
math.abs(x)
math.sign(x)
math.min(a,b)
math.max(a,b)
math.clamp(x,lo,hi)
math.sqrt(x)
math.pow(x,y)
math.exp(x)
math.log(x)
math.sin(x)
math.cos(x)
math.tan(x)
math.hypot(a,b)
math.fmod(a,b)
```

The transcendental kernels are Sleela bytecode numerical approximations rather than host-libm calls. Their approximation regimes are therefore part of the computational contract and should be tightened as the numerical object layer matures.

## Executable Physics API

Implemented calls include:

```text
physics.C()
physics.H()
physics.HBAR()
physics.E_CHARGE()
physics.K_B()
physics.G()
physics.G0()
physics.N_A()
physics.R()
physics.velocity(dx,dt)
physics.acceleration(dv,dt)
physics.kinematic_position(x0,v0,a,t)
physics.force(m,a)
physics.kinetic_energy(m,v)
physics.gravitational_force(m1,m2,r)
physics.escape_velocity(M,r)
physics.lorentz_gamma(v)
physics.relativistic_energy(m,v)
physics.momentum(m,v)
physics.ohms_voltage(I,R)
physics.electric_power(V,I)
physics.ideal_gas_pressure(n,R,T)
physics.wave_frequency(v,wavelength)
physics.angular_frequency(f)
physics.wavenumber(wavelength)
physics.neutrino_oscillation_probability(theta,delta_m2,L)
```

The physics constants are represented with SI-compatible values. Domain restrictions remain important: for example, Lorentz gamma requires `|v| < C` for a real result, and the neutrino expression is an explicitly unit- and approximation-dependent model.

## Executable Economics API

Implemented calls include:

```text
economics.future_value(principal,rate,periods)
economics.present_value(future,rate,periods)
economics.annuity_present(payment,rate,periods)
economics.annuity_future(payment,rate,periods)
economics.elasticity(pct_quantity,pct_price)
economics.real_rate(nominal,inflation)
economics.fisher_nominal(real,inflation)
economics.doubling_time(rate)
economics.continuous_value(principal,rate,time)
economics.profit(revenue,cost)
economics.margin(profit,revenue)
economics.gdp_identity(C,I,G)
```

These are computational identities/models, not empirical forecasts. Units, period conventions, compounding assumptions, and the distinction between nominal and real quantities remain part of the model supplied by the caller.

## Compiler and Runtime Boundary

The execution path is:

**source import → module validation → chemistry lowering → native lowering → Sleela Core bytecode → runnable `.sleela` artifact**

Chemistry lowering happens before both immediate source execution and persistent artifact generation. The artifact therefore carries executable Core instructions rather than a dependency on the chemistry frontend.

## Numerical and Reasoning Contract

The 133+ through 181+ design range is a project complexity marker, not a psychometric claim. The intended bridge for advanced readers is:

**physical/chemical/economic observation → quantity → unit → assumption → equation → transformation → numerical approximation → result → interpretation**

A formula is not the same thing as its measurement, and a numerical answer is not automatically a conclusion about the world. Sleela's native layer should preserve those logical locations rather than collapse them.

For every advanced function, future versions should expose domain conditions, dimensional expectations, stability notes, approximation regime, constant provenance, convergence/failure semantics, and relationships to mathematical and Review objects.
