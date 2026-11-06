# Physics Canon

> 89 equations, sourced from `SMPhysicsCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The load-bearing equations of physics -- classical and analytical mechanics, all four Maxwell equations and electromagnetism, special and general relativity, quantum mechanics and quantum field theory, atomic physics, optics, waves, and fluid dynamics. Each row credits the discoverer and year as the history-of-science record gives them, with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`sum`, `product`, `laplacian`, Greek names spelled out) so they stay greppable and
diff-friendly. The **Why it matters** column states each result's significance in one
faithful line.

## The 89 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Newton's second law | `F = m*a  (F = dp/dt)` | classical mechanics | Isaac Newton | 1687 | F = ma -- the equation of motion that founds classical dynamics. |
| 2 | Newton's law of gravitation | `F = G m1 m2 / r^2` | classical mechanics | Isaac Newton | 1687 | One inverse-square law governs apples and planets alike; the first universal force. |
| 3 | Work-energy / kinetic energy | `KE = (1/2) m v^2` | classical mechanics | classical (Leibniz, Coriolis) | 1829 | Links force over distance to motion; the energy bookkeeping of mechanics. |
| 4 | Conservation of momentum | `sum p_i = constant (closed system)` | classical mechanics | Newton; Descartes | 1687 | Total momentum is fixed in a closed system; the principle behind collisions and rockets. |
| 5 | Hooke's law | `F = -k x` | classical mechanics | Robert Hooke | 1678 | Restoring force proportional to displacement; the model for springs, elasticity, and oscillators. |
| 6 | Lagrangian equation of motion | `d/dt (dL/d qdot) - dL/dq = 0` | analytical mechanics | Joseph-Louis Lagrange | 1788 | Derives dynamics from a single scalar (the action); the reformulation that generalizes to fields. |
| 7 | Hamilton's equations | `qdot = dH/dp,  pdot = -dH/dq` | analytical mechanics | William Rowan Hamilton | 1833 | Recasts mechanics in phase space; the bridge to statistical and quantum mechanics. |
| 8 | Universal gravitation potential | `U = -G m1 m2 / r` | classical mechanics | Isaac Newton | 1687 | The energy field behind Newtonian gravity; the setting for orbital mechanics. |
| 9 | Coulomb's law | `F = k_e q1 q2 / r^2` | electromagnetism | Charles-Augustin de Coulomb | 1785 | The electric inverse-square force; the electromagnetic counterpart of gravitation. |
| 10 | Gauss's law (electric) | `div E = rho / epsilon0` | electromagnetism | Carl Friedrich Gauss / Maxwell | 1835 | Charge is the source of electric flux; the first Maxwell equation. |
| 11 | Gauss's law (magnetic) | `div B = 0` | electromagnetism | James Clerk Maxwell | 1865 | No magnetic monopoles -- field lines always close; the second Maxwell equation. |
| 12 | Faraday's law of induction | `curl E = -dB/dt` | electromagnetism | Michael Faraday / Maxwell | 1831 | Changing magnetic flux induces voltage; the principle of every generator and transformer. |
| 13 | Ampere-Maxwell law | `curl B = mu0 J + mu0 epsilon0 dE/dt` | electromagnetism | Ampere / James Clerk Maxwell | 1865 | Currents and changing electric fields make magnetism; the equation that predicted light. |
| 14 | Lorentz force | `F = q(E + v x B)` | electromagnetism | Hendrik Lorentz | 1895 | The force on a charge in electromagnetic fields; how fields move matter. |
| 15 | Electromagnetic wave speed | `c = 1 / sqrt(mu0 epsilon0)` | electromagnetism | James Clerk Maxwell | 1865 | Light's speed emerges from pure electromagnetism; Maxwell's unification of optics and EM. |
| 16 | Poynting vector | `S = (1/mu0) E x B` | electromagnetism | John Henry Poynting | 1884 | The flow of electromagnetic energy; how radiation carries power. |
| 17 | Mass-energy equivalence | `E = m c^2` | special relativity | Albert Einstein | 1905 | E = mc^2 -- mass is concentrated energy; the basis of nuclear energy and particle physics. |
| 18 | Energy-momentum relation | `E^2 = (p c)^2 + (m c^2)^2` | special relativity | Albert Einstein | 1905 | The relativistic link among energy, momentum, and rest mass; valid even for massless light. |
| 19 | Lorentz factor | `gamma = 1 / sqrt(1 - v^2/c^2)` | special relativity | Hendrik Lorentz | 1904 | The stretch factor of time and length at high speed; the signature of special relativity. |
| 20 | Time dilation | `dt = gamma * dtau` | special relativity | Albert Einstein | 1905 | Moving clocks run slow; a directly tested prediction of relativity (GPS depends on it). |
| 21 | Einstein field equations | `G_mu_nu + Lambda g_mu_nu = (8 pi G / c^4) T_mu_nu` | general relativity | Albert Einstein | 1915 | Matter curves spacetime and curvature steers matter; the governing law of gravity and cosmology. |
| 22 | Schwarzschild radius | `r_s = 2 G M / c^2` | general relativity | Karl Schwarzschild | 1916 | The size of a black hole's event horizon; where escape velocity reaches light speed. |
| 23 | Planck relation | `E = h * nu` | quantum mechanics | Max Planck | 1900 | Energy comes in quanta E = h*nu; the equation that opened quantum theory. |
| 24 | Planck's law (spectral) | `B(nu,T) = (2 h nu^3/c^2) / (e^(h nu/kT) - 1)` | quantum mechanics | Max Planck | 1900 | The blackbody spectrum that resolved the UV catastrophe; the true start of quantum physics. |
| 25 | de Broglie wavelength | `lambda = h / p` | quantum mechanics | Louis de Broglie | 1924 | Matter has a wavelength; the idea that made electron microscopes and quantum chemistry possible. |
| 26 | Time-dependent Schrodinger equation | `i hbar dPsi/dt = H Psi` | quantum mechanics | Erwin Schrodinger | 1926 | The evolution law of quantum states; the F=ma of quantum mechanics. |
| 27 | Time-independent Schrodinger equation | `H psi = E psi` | quantum mechanics | Erwin Schrodinger | 1926 | The eigenvalue problem for stationary states and energy levels; the workhorse of chemistry. |
| 28 | Heisenberg uncertainty principle | `dx * dp >= hbar / 2` | quantum mechanics | Werner Heisenberg | 1927 | Position and momentum can't both be sharp; a fundamental limit, not a measurement flaw. |
| 29 | Dirac equation | `(i hbar gamma^mu d_mu - m c) psi = 0` | quantum field theory | Paul Dirac | 1928 | Marries quantum mechanics and relativity; predicted antimatter and electron spin. |
| 30 | Canonical commutation relation | `[x, p] = i hbar` | quantum mechanics | Born; Heisenberg; Jordan | 1925 | [x,p] = i hbar encodes the quantum of action; the algebraic root of uncertainty. |
| 31 | Bohr model energy levels | `E_n = -13.6 eV / n^2` | atomic physics | Niels Bohr | 1913 | Quantized atomic energies that explained spectral lines; the first working atom model. |
| 32 | Rydberg formula | `1/lambda = R (1/n1^2 - 1/n2^2)` | atomic physics | Johannes Rydberg | 1888 | Predicts hydrogen's spectral lines; empirical evidence that energy is quantized. |
| 33 | Photoelectric equation | `K_max = h nu - phi` | quantum mechanics | Albert Einstein | 1905 | Light ejects electrons in quanta; Einstein's photon explanation, his Nobel work. |
| 34 | Navier-Stokes (incompressible) | `rho(dv/dt + v.grad v) = -grad p + mu laplacian v + f` | fluid dynamics | Navier; Stokes | 1845 | The governing equations of fluid flow; a Clay Millennium Problem still open. |
| 35 | Wave equation | `d^2u/dt^2 = c^2 laplacian u` | classical field theory | Jean le Rond d'Alembert | 1746 | The universal law of propagating disturbances -- sound, light, strings, and water. |
| 36 | Bernoulli's principle | `p + (1/2) rho v^2 + rho g h = constant` | fluid dynamics | Daniel Bernoulli | 1738 | Faster flow means lower pressure; explains lift, venturis, and much of aerodynamics. |
| 37 | Newton's first law (inertia) | `v = constant when sum F = 0` | classical mechanics | Isaac Newton | 1687 | Without a net force, motion is unchanged; the definition of an inertial frame. |
| 38 | Newton's third law | `F_12 = -F_21` | classical mechanics | Isaac Newton | 1687 | Every action has an equal, opposite reaction; the source of momentum conservation. |
| 39 | Gravitational potential energy | `U = m g h  (near surface)` | classical mechanics | classical | unknown | Stored energy of height near Earth's surface; the simplest energy-conservation tool. |
| 40 | Simple harmonic motion | `x(t) = A cos(omega t + phi)` | classical mechanics | classical | unknown | The sinusoidal motion of any restoring-force system; the template for all oscillation. |
| 41 | Centripetal acceleration | `a_c = v^2 / r` | classical mechanics | Christiaan Huygens | 1659 | The inward acceleration that bends motion into a circle; sets orbital and rotational dynamics. |
| 42 | Doppler effect | `f' = f (c +/- v_o) / (c -/+ v_s)` | waves | Christian Doppler | 1842 | Frequency shifts with relative motion; how radar, sonar, and redshift measurements work. |
| 43 | Snell's law | `n1 sin(theta1) = n2 sin(theta2)` | optics | Snellius; Descartes | 1621 | Governs how light bends between media; the foundation of lenses and optics. |
| 44 | Ohm's law | `V = I R` | electromagnetism | Georg Ohm | 1827 | Voltage, current, and resistance in proportion; the first law of circuit analysis. |
| 45 | Compton scattering | `d_lambda = (h / (m_e c))(1 - cos theta)` | quantum mechanics | Arthur Compton | 1923 | Light scatters off electrons like a particle; hard proof of the photon's momentum. |
| 46 | Wien's displacement law | `lambda_max T = b` | quantum mechanics | Wilhelm Wien | 1893 | Hotter bodies peak at shorter wavelengths; why embers glow red and stars blue. |
| 47 | Kepler's first law | `planets move on ellipses with the Sun at one focus` | celestial mechanics | Johannes Kepler | 1609 | Planets orbit in ellipses with the Sun at a focus; the first break from circular orbits. |
| 48 | Kepler's second law | `the radius vector sweeps equal areas in equal times` | celestial mechanics | Johannes Kepler | 1609 | A planet sweeps equal areas in equal times; an early statement of angular-momentum conservation. |
| 49 | Kepler's third law | `T^2 = (4 pi^2 / (G M)) a^3` | celestial mechanics | Johannes Kepler | 1619 | Orbital period squared scales with semi-major axis cubed; the harmony of the solar system. |
| 50 | Poisson's equation (electrostatics) | `laplacian(phi) = -rho / epsilon0` | electromagnetism | Simeon Denis Poisson | 1813 | Relates potential to charge density; the master equation of electrostatics and gravitation. |
| 51 | Continuity equation (mass) | `d(rho)/dt + div(rho v) = 0` | fluid dynamics | Leonhard Euler | 1757 | Expresses local conservation of mass in a flow; the foundation of fluid and charge transport. |
| 52 | Euler's equations (inviscid flow) | `rho (dv/dt + (v.grad) v) = -grad p` | fluid dynamics | Leonhard Euler | 1757 | Newton's law for an ideal fluid; the ancestor of the Navier-Stokes equations. |
| 53 | Reynolds number | `Re = rho v L / mu` | fluid dynamics | Osborne Reynolds | 1883 | The ratio of inertial to viscous forces; predicts laminar-versus-turbulent flow. |
| 54 | Biot-Savart law | `dB = (mu0 / (4 pi)) I dl x r-hat / r^2` | electromagnetism | Biot; Savart | 1820 | Gives the magnetic field of a current element; the magnetic analog of Coulomb's law. |
| 55 | Lenz's law | `induced EMF opposes the change in flux` | electromagnetism | Heinrich Lenz | 1834 | Induced currents oppose their cause; the sign rule enforcing energy conservation in induction. |
| 56 | Larmor power (accelerated charge) | `P = (q^2 a^2) / (6 pi epsilon0 c^3)` | electromagnetism | Joseph Larmor | 1897 | The power radiated by an accelerating charge; the basis of antenna and synchrotron theory. |
| 57 | Poynting theorem (energy flow) | `dU/dt + div S = -J.E` | electromagnetism | John Henry Poynting | 1884 | Tracks electromagnetic energy flow and conservation; defines the Poynting vector's role. |
| 58 | Lorentz transformation | `t' = gamma(t - v x / c^2),  x' = gamma(x - v t)` | special relativity | Hendrik Lorentz | 1904 | Relates space and time between inertial frames; the kinematic core of special relativity. |
| 59 | Relativistic momentum | `p = gamma m v` | special relativity | Albert Einstein | 1905 | Momentum grows with the Lorentz factor; corrects Newton at high speed. |
| 60 | Relativistic energy | `E = gamma m c^2` | special relativity | Albert Einstein | 1905 | Total energy is gamma times rest energy; unifies mass and kinetic energy. |
| 61 | Relativistic velocity addition | `w = (u + v)/(1 + u v / c^2)` | special relativity | Albert Einstein | 1905 | Velocities combine so nothing exceeds light speed; a hallmark of relativity. |
| 62 | Klein-Gordon equation | `(box + (m c / hbar)^2) psi = 0` | quantum field theory | Klein; Gordon | 1926 | The relativistic wave equation for spin-0 particles; an entry point to quantum field theory. |
| 63 | Yukawa potential | `V(r) = -g^2 e^(-m r) / r` | nuclear physics | Hideki Yukawa | 1935 | A screened potential explaining the short-range nuclear force; predicted the meson. |
| 64 | Fermi-Dirac distribution | `n(E) = 1 / (e^((E - mu)/(k_B T)) + 1)` | statistical physics | Fermi; Dirac | 1926 | Occupation statistics for fermions; governs electrons in metals and semiconductors. |
| 65 | Bose-Einstein distribution | `n(E) = 1 / (e^((E - mu)/(k_B T)) - 1)` | statistical physics | Bose; Einstein | 1924 | Occupation statistics for bosons; explains lasers and Bose-Einstein condensates. |
| 66 | Rayleigh-Jeans law | `B(lambda,T) = 2 c k_B T / lambda^4` | statistical physics | Rayleigh; Jeans | 1905 | The classical radiation law whose failure exposed the ultraviolet catastrophe. |
| 67 | Bragg's law | `n lambda = 2 d sin(theta)` | condensed matter | W. H. Bragg & W. L. Bragg | 1913 | The diffraction condition revealing crystal structure; the basis of X-ray crystallography. |
| 68 | Malus's law | `I = I0 cos^2(theta)` | optics | Etienne-Louis Malus | 1809 | Transmitted intensity through a polarizer varies as cosine squared; a foundation of optics. |
| 69 | Beer-Lambert law | `I = I0 e^(-alpha x)` | optics | Beer; Lambert; Bouguer | 1852 | Absorbance grows with path and concentration; the basis of spectrophotometry. |
| 70 | Fresnel reflection (normal incidence) | `R = ((n1 - n2)/(n1 + n2))^2` | optics | Augustin-Jean Fresnel | 1823 | Gives the fraction of light reflected at an interface; central to optical coatings. |
| 71 | Thin lens equation | `1/f = 1/do + 1/di` | optics | classical | unknown | Relates object, image, and focal lengths; the working formula of geometric optics. |
| 72 | Rayleigh resolution criterion | `theta = 1.22 lambda / D` | optics | Lord Rayleigh | 1879 | Sets the smallest resolvable angle of an aperture; the diffraction limit of imaging. |
| 73 | Hall effect voltage | `V_H = I B / (n q t)` | condensed matter | Edwin Hall | 1879 | A transverse voltage from carriers in a magnetic field; measures carrier type and density. |
| 74 | Drude conductivity | `sigma = n e^2 tau / m` | condensed matter | Paul Drude | 1900 | A classical model of electrical conduction; the first explanation of Ohm's law in metals. |
| 75 | Debye heat capacity (low-T) | `C_V = (12 pi^4 / 5) N k_B (T / Theta_D)^3` | condensed matter | Peter Debye | 1912 | Predicts the T-cubed law of heat capacity at low temperature; a triumph of solid-state physics. |
| 76 | London penetration depth | `lambda_L = sqrt(m / (mu0 n_s e^2))` | superconductivity | Fritz & Heinz London | 1935 | The scale over which a magnetic field decays in a superconductor; defines the Meissner effect. |
| 77 | Josephson relation | `I = I_c sin(phi),  dphi/dt = 2 e V / hbar` | superconductivity | Brian Josephson | 1962 | Supercurrent and voltage across a tunnel junction; the basis of SQUIDs and the voltage standard. |
| 78 | Casimir force (parallel plates) | `F/A = -pi^2 hbar c / (240 d^4)` | quantum electrodynamics | Hendrik Casimir | 1948 | An attractive force from quantum vacuum fluctuations; a measurable prediction of QED. |
| 79 | Stefan-Boltzmann (radiated power) | `P = sigma A epsilon T^4` | thermal radiation | Stefan; Boltzmann | 1879 | Radiated power scales as temperature to the fourth; governs stellar and thermal emission. |
| 80 | Escape velocity | `v_e = sqrt(2 G M / r)` | celestial mechanics | classical (Newtonian) | unknown | The speed needed to break free of gravity; a basic result of orbital mechanics. |
| 81 | Rocket equation (Tsiolkovsky) | `delta v = v_e ln(m0 / m1)` | astronautics | Konstantin Tsiolkovsky | 1903 | Links velocity change to exhaust speed and mass ratio; the founding law of spaceflight. |
| 82 | Torque | `tau = r x F` | classical mechanics | classical (Archimedes/Euler) | unknown | The rotational analog of force; sets angular acceleration about an axis. |
| 83 | Angular momentum | `L = r x p = I omega` | classical mechanics | Euler | 1775 | The conserved quantity of rotation; central from tops to galaxies to atoms. |
| 84 | Moment of inertia | `I = sum m_i r_i^2` | classical mechanics | Leonhard Euler | 1765 | A body's resistance to angular acceleration; the rotational counterpart of mass. |
| 85 | Rotational kinetic energy | `KE_rot = (1/2) I omega^2` | classical mechanics | Euler | 1765 | The energy stored in spinning motion; completes the mechanics of rotation. |
| 86 | Poiseuille's law (pipe flow) | `Q = pi r^4 dP / (8 mu L)` | fluid dynamics | Hagen; Poiseuille | 1840 | Flow rate scales with the fourth power of radius; governs viscous flow in tubes and vessels. |
| 87 | Stokes' drag law | `F = 6 pi mu r v` | fluid dynamics | George Gabriel Stokes | 1851 | The viscous drag on a slow sphere; used from sedimentation to Millikan's oil-drop experiment. |
| 88 | Pendulum period (small angle) | `T = 2 pi sqrt(L / g)` | classical mechanics | classical (Huygens) | 1673 | The period depends only on length and gravity; the principle of the pendulum clock. |
| 89 | Gravitational time dilation (weak field) | `dtau/dt = sqrt(1 - 2 G M / (r c^2))` | general relativity | Albert Einstein | 1915 | Clocks run slower deeper in a gravity well; confirmed by GPS and precise clocks. |

## Validate

These equations are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints every entry and the running total across
all four canons):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Names, forms, discoverers, and years follow the standard history-of-science record;
the **Why it matters** notes summarize the result's established significance. See
`SMProvenance.sleela` for the tiered source registry (primary papers, secondary
references such as NIST DLMF / Abramowitz & Stegun, and tertiary locators including
Wikipedia, cited as CC BY-SA and never treated as a primary authority) and the repo
`README.md` for per-class integrity goals. This archive records and attributes
published results; it does not originate, own, or certify them.

## Definitions & relevance (second-layer additions)

The entries below are the second-layer additions to this canon, organized by
type. Each gives a short **definition** (what the law states and what its symbols
mean) and its **relevance** (why it is foundational to physics).

### Celestial mechanics & astronautics

**Kepler's first law** — planets move on ellipses with the Sun at one focus. It
broke two millennia of circular-orbit assumptions and set the stage for Newton's
derivation of gravity from the inverse-square law.

**Kepler's second law** — the radius vector sweeps equal areas in equal times. It
is an early, geometric statement of angular-momentum conservation, explaining why
planets speed up near the Sun.

**Kepler's third law** — `T^2 = (4 pi^2 / (G M)) a^3`. Orbital period squared is
proportional to semi-major axis cubed. It quantitatively links the orbits of all
bodies around a common mass and lets astronomers weigh stars and planets.

**Escape velocity** — `v_e = sqrt(2 G M / r)`. The minimum speed needed to break
free of a body's gravity without further propulsion. It is a basic design number
for spaceflight and a conceptual threshold for black-hole horizons.

**Rocket equation (Tsiolkovsky)** — `delta v = v_e ln(m0 / m1)`. The achievable
change in velocity depends on exhaust speed and the ratio of initial to final
mass. It is the founding law of astronautics, exposing why staging and high
exhaust velocity are essential.

### Electromagnetism

**Poisson's equation (electrostatics)** — `laplacian(phi) = -rho / epsilon0`.
Relates the electric potential to the local charge density. It (and its
source-free form, Laplace's equation) is the master equation of electrostatics
and of Newtonian gravity.

**Biot-Savart law** — `dB = (mu0 / (4 pi)) I dl x r-hat / r^2`. Gives the magnetic
field produced by a current element. It is the magnetic counterpart of Coulomb's
law and the basic tool for computing fields of coils and wires.

**Lenz's law** — induced currents oppose the change in flux that creates them. It
fixes the direction of electromagnetic induction and is the embodiment of energy
conservation in Faraday's law.

**Larmor power (accelerated charge)** — `P = (q^2 a^2) / (6 pi epsilon0 c^3)`. An
accelerating charge radiates power proportional to the square of its
acceleration. It underlies antenna radiation, synchrotron light, and the
classical instability of atoms that quantum theory resolved.

**Poynting theorem (energy flow)** — `dU/dt + div S = -J.E`. Expresses local
conservation of electromagnetic energy, with the Poynting vector `S` carrying the
energy flux. It is how we track where field energy goes.

### Fluid dynamics

**Continuity equation (mass)** — `d(rho)/dt + div(rho v) = 0`. States local
conservation of mass in a flowing medium. It is a foundational conservation law
reused for charge, probability, and energy transport.

**Euler's equations (inviscid flow)** — `rho (dv/dt + (v.grad) v) = -grad p`.
Newton's second law for an ideal (frictionless) fluid. They are the ancestor of
the Navier-Stokes equations and the basis of much of aerodynamics.

**Reynolds number** — `Re = rho v L / mu`. The dimensionless ratio of inertial to
viscous forces. It predicts the transition from smooth laminar to chaotic
turbulent flow and underpins fluid-dynamical similarity and scale modeling.

**Poiseuille's law (pipe flow)** — `Q = pi r^4 dP / (8 mu L)`. Volumetric flow
through a pipe scales with the fourth power of the radius. It governs viscous flow
in tubes and blood vessels, where small radius changes have large effects.

**Stokes' drag law** — `F = 6 pi mu r v`. The viscous drag on a small sphere moving
slowly through a fluid. It is used from sedimentation and aerosol physics to
Millikan's measurement of the electron charge.

### Relativity

**Lorentz transformation** — `t' = gamma(t - v x / c^2),  x' = gamma(x - v t)`.
Relates space and time coordinates between inertial frames in relative motion. It
is the kinematic core of special relativity, replacing the Galilean
transformation.

**Relativistic momentum** — `p = gamma m v`. Momentum grows without bound as speed
approaches `c`. It corrects Newtonian momentum and keeps conservation laws valid
at high speed.

**Relativistic energy** — `E = gamma m c^2`. Total energy is the Lorentz factor
times the rest energy, unifying kinetic energy and mass. It reduces to `E = mc^2`
at rest and to Newtonian kinetic energy at low speed.

**Relativistic velocity addition** — `w = (u + v)/(1 + u v / c^2)`. Velocities
combine so that the result never exceeds the speed of light. It is a direct,
testable hallmark of relativity.

**Gravitational time dilation (weak field)** — `dtau/dt = sqrt(1 - 2 G M / (r c^2))`.
Clocks deeper in a gravitational well run slower. It is a prediction of general
relativity confirmed daily in the GPS system and by precision atomic clocks.

### Quantum, nuclear & statistical physics

**Klein-Gordon equation** — `(box + (m c / hbar)^2) psi = 0`. The relativistic wave
equation for spin-0 particles. It is an entry point to quantum field theory and
the template for relativistic wave equations.

**Yukawa potential** — `V(r) = -g^2 e^(-m r) / r`. A screened, short-range
potential mediated by a massive particle. It explained the short range of the
nuclear force and predicted the existence of the meson.

**Fermi-Dirac distribution** — `n(E) = 1 / (e^((E - mu)/(k_B T)) + 1)`. The
occupation statistics of fermions, which obey the Pauli exclusion principle. It
governs electrons in metals, semiconductors, and white dwarfs.

**Bose-Einstein distribution** — `n(E) = 1 / (e^((E - mu)/(k_B T)) - 1)`. The
occupation statistics of bosons, which can pile into the same state. It explains
lasers, superfluids, and Bose-Einstein condensation.

**Rayleigh-Jeans law** — `B(lambda,T) = 2 c k_B T / lambda^4`. The classical
prediction for blackbody radiation, which diverges at short wavelengths. Its
failure — the ultraviolet catastrophe — forced the birth of quantum theory.

### Condensed matter, optics & QED

**Bragg's law** — `n lambda = 2 d sin(theta)`. The condition for constructive
interference of waves scattered by crystal planes. It is the foundation of X-ray
crystallography, through which molecular and material structures are determined.

**Hall effect voltage** — `V_H = I B / (n q t)`. A transverse voltage appears when
current flows across a magnetic field. It reveals carrier sign and density and is
the basis of magnetic-field sensors and the quantum Hall standard.

**Drude conductivity** — `sigma = n e^2 tau / m`. A classical free-electron model
of electrical conduction. It gave the first microscopic account of Ohm's law in
metals and still frames how we think about carrier transport.

**Debye heat capacity (low-T)** — `C_V = (12 pi^4 / 5) N k_B (T / Theta_D)^3`.
Predicts the characteristic T-cubed fall of a solid's heat capacity at low
temperature. It completed the quantum theory of lattice vibrations.

**Malus's law** — `I = I0 cos^2(theta)`. The intensity transmitted through a
polarizer varies as the cosine squared of the angle. It is a foundational law of
polarization optics.

**Beer-Lambert law** — `I = I0 e^(-alpha x)`. Light intensity decays exponentially
with path length through an absorbing medium. It underlies spectroscopy and
optical attenuation generally.

**Fresnel reflection (normal incidence)** — `R = ((n1 - n2)/(n1 + n2))^2`. The
fraction of light reflected at an interface between two media. It is central to
lens coatings, fiber optics, and the physics of reflection.

**Thin lens equation** — `1/f = 1/do + 1/di`. Relates focal length to object and
image distances. It is the working formula of geometric optics and lens design.

**Rayleigh resolution criterion** — `theta = 1.22 lambda / D`. Sets the smallest
angular separation an aperture of diameter `D` can resolve. It is the diffraction
limit of telescopes, microscopes, and cameras.

**London penetration depth** — `lambda_L = sqrt(m / (mu0 n_s e^2))`. The depth to
which a magnetic field penetrates a superconductor before being expelled. It
quantifies the Meissner effect that defines superconductivity.

**Josephson relation** — `I = I_c sin(phi),  dphi/dt = 2 e V / hbar`. Supercurrent
and voltage across a thin insulating barrier between superconductors. It underpins
SQUID magnetometers and the modern voltage standard.

**Casimir force (parallel plates)** — `F/A = -pi^2 hbar c / (240 d^4)`. An
attractive force between uncharged conducting plates arising from quantum vacuum
fluctuations. It is a measurable, striking prediction of quantum electrodynamics.

**Stefan-Boltzmann (radiated power)** — `P = sigma A epsilon T^4`. The power a body
radiates scales with the fourth power of its temperature. It governs thermal and
stellar emission and planetary energy balance.

### Classical mechanics of rotation

**Torque** — `tau = r x F`. The rotational analog of force, setting the rate of
change of angular momentum. It is the basic cause of rotational acceleration.

**Angular momentum** — `L = r x p = I omega`. The conserved quantity of rotational
motion. Its conservation explains everything from spinning skaters to the
stability of planetary orbits and atomic states.

**Moment of inertia** — `I = sum m_i r_i^2`. A body's resistance to angular
acceleration, the rotational counterpart of mass. It depends on how mass is
distributed about the axis.

**Rotational kinetic energy** — `KE_rot = (1/2) I omega^2`. The energy stored in a
spinning body. It completes the energy bookkeeping of combined translation and
rotation.

**Pendulum period (small angle)** — `T = 2 pi sqrt(L / g)`. For small swings a
pendulum's period depends only on its length and gravity, not amplitude. This
isochronism is the principle behind the pendulum clock.
