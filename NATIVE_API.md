# Sleela Native API

## Module Model

Sleela uses explicit imports for native facilities:

```sleela
import math;
import physics;
import economics;
```

Calls are module-qualified. Sleelvac lowers the executable scientific/economic calls into ordinary Sleela Core methods before bytecode emission. Consequently a compiled `.sleela` artifact contains the implementation and the runtime does not invoke Sleelvac again.

## Native Module Registry

| Module | Purpose | Status |
|---|---|---|
| `math` | scalar mathematics and numerical kernels | executable subset implemented |
| `physics` | constants, mechanics, relativity, waves, selected neutrino model | executable subset implemented |
| `economics` | valuation, compounding, elasticity, growth and macro identities | executable subset implemented |
| `excel` | workbook/worksheet I/O | reserved for Excel backend |
| `json` | structured data | reserved |
| `crypto` | cryptographic APIs | reserved |
| `net` | network APIs | reserved/overlaps existing network builtins |

Unknown modules and duplicate imports are rejected by Sleelvac.

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

**source import → module validation → native lowering → Sleela Core bytecode → runnable `.sleela` artifact**

Native lowering happens before both immediate source execution and persistent artifact generation. The artifact therefore carries executable Core instructions rather than a dependency on the front-end compiler.

## Numerical and Reasoning Contract

The 133+ through 181+ design range is a project complexity marker, not a psychometric claim. The intended bridge for advanced readers is:

**physical/economic observation → quantity → unit → assumption → equation → transformation → numerical approximation → result → interpretation**

A formula is not the same thing as its measurement, and a numerical answer is not automatically a conclusion about the world. Sleela's native layer should preserve those logical locations rather than collapse them.

For every advanced function, future versions should expose domain conditions, dimensional expectations, stability notes, approximation regime, constant provenance, convergence/failure semantics, and relationships to mathematical and Review objects.
