# Physics Canon

> 46 equations, sourced from `SMPhysicsCanon.sleela` -- the single
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

## The 46 equations

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
