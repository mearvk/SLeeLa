# PHYSICS.md

## Purpose

`PHYSICS.md` is the primary reference for the executable Physics domain in SLeeLa. Physics formulas are represented as explicit transformations over quantities and assumptions, then lowered through Sleela's native math layer into Core bytecode.

## Executable API

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

## Constants

The current executable constants include:

| Symbol | Meaning | Value |
|---|---|---:|
| `C` | speed of light | `299792458` |
| `H` | Planck constant | `6.62607015e-34` |
| `HBAR` | reduced Planck constant | `1.054571817e-34` |
| `E_CHARGE` | elementary charge | `1.602176634e-19` |
| `K_B` | Boltzmann constant | `1.380649e-23` |
| `G` | gravitational constant | `6.67430e-11` |
| `G0` | standard gravity | `9.80665` |
| `N_A` | Avogadro constant | `6.02214076e23` |
| `R` | molar gas constant | `8.31446261815324` |

## Formula Families

The executable model covers kinematics, Newtonian mechanics, gravitation, special relativity, electrical relations, idealized gas relations, wave relations, and a simplified neutrino-oscillation probability expression.

Representative relations include:

```text
velocity = dx / dt
acceleration = dv / dt
x = x0 + v0*t + 0.5*a*t^2
F = m*a
KE = 0.5*m*v^2
Fgrav = G*m1*m2/r^2
vescape = sqrt(2*G*M/r)
gamma = 1/sqrt(1-v^2/c^2)
E = gamma*m*c^2
p = gamma*m*v
V = I*R
P = V*I
f = v/lambda
omega = 2*pi*f
k = 2*pi/lambda
```

The neutrino model is intentionally documented as a simplified computational model. It is not a substitute for experimental data, a complete oscillation formalism, or an observation.

## Domain Conditions

Physics functions have meaningful domains and dimensional expectations. Values such as velocity relative to `C`, distance denominators, temperature, wavelength, and time intervals must be interpreted with their units and assumptions. A numerical output does not automatically establish a physical fact.

One current API name, `ideal_gas_pressure(n,R,T)`, represents the implemented `nRT` relation; a future contract should add volume explicitly or rename the function so the physical dimensional meaning is unambiguous.

## Cross-Domain Role

Physics uses the executable Math kernels for square roots, powers, trigonometric functions, and related transformations. The architecture preserves the chain:

**physical subject → quantity → unit → assumption → equation → transformation → numerical result → interpretation**.

## Status

The scalar Physics API is implemented and integrated with Sleelvac and persistent `.sleela` artifact generation. Expanded dimensional typing, richer physical models, uncertainty propagation, and additional validated model families remain future work.
