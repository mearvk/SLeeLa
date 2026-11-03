# Calculus-of-Events Canon

> 22 equations, sourced from `SMEventsCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

A **calculus of events**: the foundational equations that govern *things that
happen* rather than steady states. Four kinds of event are recorded --

- **Collision** -- the event of impact: how momentum, energy, and direction are
  redistributed when bodies meet.
- **Sound** -- the event of a pressure disturbance born, propagating through a
  medium, and being heard: the modeling of acoustics.
- **Departure** -- arrivals-and-departures: how discrete events enter and leave a
  system (queueing theory and first-passage / escape).
- **Routine** -- the periodic, recurrent, and renewal processes that bring events
  back around on a schedule.

Each row is a faithful record: the result under the name history gives it, with
the rightful discoverer(s) and year, plus a one-line note on why it matters. The
one hard rule governs every row: **values are faithful to the subject's real
origin where publicly known**; diffuse or unknown attributions are marked
honestly (`classical`, named-pair), never invented to look legitimate. Symbolic
forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`laplacian`, Greek names spelled out) so they stay greppable and diff-friendly.

## The 22 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Impulse-momentum theorem | `J = integral F dt = delta p` | collision mechanics | Isaac Newton (Principia) | 1687 | Equates the time-integral of force to the change in momentum; the bridge between a force and the event it causes. |
| 2 | Coefficient of restitution | `e = -(v2' - v1') / (v2 - v1)` | collision mechanics | Isaac Newton | 1687 | One number, 0 to 1, that classifies a collision from perfectly inelastic to perfectly elastic. |
| 3 | Elastic collision (1-D final velocities) | `v1' = ((m1-m2) v1 + 2 m2 v2)/(m1+m2)` | collision mechanics | Huygens; Wallis; Wren | 1668 | The closed-form outcome of a head-on elastic hit; conserves both momentum and kinetic energy. |
| 4 | Perfectly inelastic collision | `v' = (m1 v1 + m2 v2)/(m1 + m2)` | collision mechanics | classical | 1668 | When bodies stick, momentum alone fixes the common final velocity; the maximum-energy-loss event. |
| 5 | Konig's theorem (kinetic-energy split) | `KE = KE_cm + KE_internal` | collision mechanics | Johann Samuel Konig | 1751 | Splits kinetic energy into center-of-mass motion plus internal motion; the key to analyzing any collision. |
| 6 | Rutherford scattering cross-section | `dsigma/dOmega = (k/(4 E))^2 / sin^4(theta/2)` | collision mechanics | Ernest Rutherford | 1911 | The angular law of Coulomb scattering that revealed the atomic nucleus; the founding result of particle collisions. |
| 7 | Speed of sound (Newton-Laplace) | `c = sqrt(gamma P / rho)` | acoustics | Newton; Laplace | 1816 | Laplace's adiabatic correction to Newton gives the true speed of sound in a gas; the constant at the heart of acoustics. |
| 8 | Acoustic wave equation | `d2p/dt2 = c^2 laplacian(p)` | acoustics | Euler; d'Alembert | 1750 | The PDE that governs how a pressure disturbance propagates; the master equation of sound. |
| 9 | Helmholtz equation | `laplacian(p) + k^2 p = 0` | acoustics | Hermann von Helmholtz | 1860 | The time-harmonic (single-frequency) form of the wave equation; the basis of modal and room acoustics. |
| 10 | Doppler effect (source and observer) | `f' = f (c +/- v_o) / (c -/+ v_s)` | acoustics | Christian Doppler | 1842 | How motion shifts perceived frequency; why a passing siren drops in pitch, and the basis of radar and sonar. |
| 11 | Sound intensity level (decibel) | `L = 10 log10(I / I0)` | acoustics | Bell System; Barkhausen | 1924 | Puts loudness on a logarithmic scale matched to hearing; the decibel that all audio engineering speaks in. |
| 12 | Webster horn equation | `d2p/dx2 + (A'/A) dp/dx + k^2 p = 0` | acoustics | Arthur Gordon Webster | 1919 | Models how sound travels down a flaring tube; the design equation for horns, brass, and loudspeaker waveguides. |
| 13 | Sabine reverberation time | `T60 = 0.161 V / A` | acoustics | Wallace Clement Sabine | 1900 | Predicts how long sound lingers in a room from its volume and absorption; the founding formula of architectural acoustics. |
| 14 | Poisson arrival process | `P(N(t) = n) = (lambda t)^n e^(-lambda t) / n!` | queueing theory | Simeon Denis Poisson | 1837 | The law of purely random arrivals at constant rate; the standard model for events entering a system. |
| 15 | M/M/1 mean number in system | `L = rho / (1 - rho),  rho = lambda/mu` | queueing theory | A. K. Erlang; D. G. Kendall | 1909 | The average backlog of a single-server queue; shows how congestion explodes as utilization nears one. |
| 16 | Erlang C (M/M/c wait probability) | `P_wait = Erlang-C(c, a),  a = lambda/mu` | queueing theory | Agner Krarup Erlang | 1917 | The chance an arrival must wait with c servers; the formula that sizes call centers and trunk lines. |
| 17 | Pollaczek-Khinchine (M/G/1 queue) | `L_q = rho^2 (1 + C^2) / (2 (1 - rho))` | queueing theory | Pollaczek; Khinchine | 1930 | Mean queue length for general service times; shows variability, not just load, drives waiting. |
| 18 | Kramers escape (first-passage) rate | `k = (w0 wb / (2 pi gamma)) e^(-E_b / (k_B T))` | first-passage theory | Hendrik Anthony Kramers | 1940 | The rate at which a system first escapes a potential well; the physics of departure, reaction, and barrier crossing. |
| 19 | Simple-harmonic period | `T = 2 pi sqrt(m / k)` | periodic processes | classical (Huygens) | 1673 | The period of an ideal oscillator, independent of amplitude; the clockwork behind every routine cycle. |
| 20 | Fourier transform (time to spectrum) | `F(w) = integral f(t) e^(-i w t) dt` | periodic processes | Joseph Fourier | 1822 | Decomposes any signal into its recurring frequencies; the lens that turns a time event into a spectrum. |
| 21 | Poincare recurrence theorem | `a measure-preserving flow returns arbitrarily close to its start` | recurrent processes | Henri Poincare | 1890 | A bounded conservative system eventually (almost) repeats its state; the deep statement that routines recur. |
| 22 | Elementary renewal theorem | `m(t) / t -> 1 / mu  as  t -> inf` | renewal theory | Feller; Smith | 1941 | The long-run rate of recurring events equals the reciprocal of their mean spacing; the backbone of renewal/reliability theory. |

## Validate

These equations are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints every entry and the running total across
all canons):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Names, forms, discoverers, and years follow the standard history-of-science record;
the **Why it matters** notes summarize the result's established significance. Where a
result is credited to a named pair (Newton/Laplace for the speed of sound,
Euler/d'Alembert for the wave equation, Pollaczek/Khinchine, Feller/Smith) both names
are given as the literature records them; diffuse attributions are marked `classical`.
See `SMProvenance.sleela` for the tiered source registry and the repo `README.md` for
per-class integrity goals. This archive records and attributes published results; it
does not originate, own, or certify them.

*Content was rephrased from standard references for compliance with licensing
restrictions.*
