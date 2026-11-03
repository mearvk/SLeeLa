# Thermodynamics & Statistical Mechanics Canon

> 48 equations, sourced from `SMThermoEquations.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

Thermodynamics and statistical mechanics from the four laws outward: entropy (Clausius, Boltzmann, Gibbs), the free energies, equations of state, the Maxwell-Boltzmann distribution and partition function, the critical point where molecular attraction governs, chemical affinity, and the equilibrium (equality) conditions, out to the bridge to information at Landauer's limit. This is the reach the Stanford probability tradition touches through statistical mechanics, each row with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`sum`, `product`, `laplacian`, Greek names spelled out) so they stay greppable and
diff-friendly. The **Why it matters** column states each result's significance in one
faithful line.

## The 48 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Zeroth law of thermodynamics | `A~C and B~C  =>  A~B (thermal equilibrium is transitive)` | thermodynamics | Fowler & Guggenheim (named) | 1939 | Thermal equilibrium is transitive; what makes temperature a well-defined quantity. |
| 2 | First law of thermodynamics | `dU = dQ - dW` | thermodynamics | Clausius; Joule; Mayer | 1850 | Energy is conserved as heat and work; thermodynamic bookkeeping of energy. |
| 3 | Second law (Clausius) | `dS >= dQ / T,  dS_universe >= 0` | thermodynamics | Rudolf Clausius | 1865 | Entropy never decreases; the law that gives time its direction. |
| 4 | Third law of thermodynamics | `S -> 0 as T -> 0 (perfect crystal)` | thermodynamics | Walther Nernst | 1906 | Entropy approaches a constant at absolute zero; absolute zero is unreachable. |
| 5 | Entropy (Clausius definition) | `dS = dQ_rev / T` | thermodynamics | Rudolf Clausius | 1865 | Defines entropy from reversible heat flow; the macroscopic origin of the concept. |
| 6 | Boltzmann entropy | `S = k * ln(W)` | statistical mechanics | Ludwig Boltzmann | 1877 | S = k ln W links entropy to microscopic arrangements; the founding equation of statistical mechanics. |
| 7 | Gibbs entropy | `S = -k * sum p_i ln p_i` | statistical mechanics | J. Willard Gibbs | 1878 | Entropy from a probability distribution over states; the general statistical definition. |
| 8 | Ideal gas law | `P V = n R T` | thermodynamics | Clapeyron (combined) | 1834 | Pressure, volume, and temperature in one relation; the first equation of state students learn. |
| 9 | Gibbs free energy | `G = H - T S` | thermodynamics | J. Willard Gibbs | 1873 | Predicts spontaneity at constant T and P; the chemist's criterion for reaction direction. |
| 10 | Helmholtz free energy | `F = U - T S` | thermodynamics | Hermann von Helmholtz | 1882 | The usable work at constant temperature and volume; central to statistical mechanics. |
| 11 | Enthalpy | `H = U + P V` | thermodynamics | Kamerlingh Onnes (term) | 1909 | Heat content at constant pressure; the accounting variable for reactions and phase changes. |
| 12 | Carnot efficiency | `eta = 1 - T_cold / T_hot` | thermodynamics | Sadi Carnot | 1824 | The absolute ceiling on any heat engine; founded the second law and thermodynamic temperature. |
| 13 | Maxwell-Boltzmann distribution | `f(v) ~ v^2 e^(-m v^2 / (2 k T))` | statistical mechanics | Maxwell; Boltzmann | 1860 | The spread of molecular speeds in a gas; first link from mechanics to temperature. |
| 14 | Boltzmann factor | `P(E) ~ e^(-E / (k T))` | statistical mechanics | Ludwig Boltzmann | 1868 | Probability of a state falls exponentially with its energy; the single most-used rule in stat-mech. |
| 15 | Partition function | `Z = sum_i e^(-E_i / (k T))` | statistical mechanics | Boltzmann; Gibbs | 1901 | Sums over all states; the generating function from which every thermodynamic quantity follows. |
| 16 | Stefan-Boltzmann law | `j = sigma T^4` | thermodynamics | Stefan; Boltzmann | 1879 | Radiated power scales as T^4; how we measure the temperatures of stars. |
| 17 | Clausius-Clapeyron relation | `dP/dT = L / (T dV)` | thermodynamics | Clausius; Clapeyron | 1834 | Governs how phase-boundary pressure varies with temperature; explains boiling and melting curves. |
| 18 | Gibbs-Helmholtz equation | `d(G/T)/dT = -H / T^2` | thermodynamics | Gibbs; Helmholtz | 1882 | Relates free energy to enthalpy and temperature; key to the temperature dependence of equilibria. |
| 19 | Sackur-Tetrode equation | `S = N k [ln(V/N (4 pi m U/(3 N h^2))^{3/2}) + 5/2]` | statistical mechanics | Sackur; Tetrode | 1912 | The absolute entropy of an ideal gas from quantum constants; a triumph of early quantum stat-mech. |
| 20 | Landauer's principle | `E_min = k T ln(2) per bit erased` | information thermodynamics | Rolf Landauer | 1961 | Erasing a bit costs a minimum of kT ln2 of energy; the physical price of information. |
| 21 | Van der Waals equation of state | `(P + a n^2/V^2)(V - n b) = n R T` | thermodynamics | Johannes van der Waals | 1873 | The first realistic gas law with molecular size and attraction; predicts condensation. |
| 22 | Heat capacity relation (ideal gas) | `C_p - C_v = n R` | thermodynamics | Mayer's relation | 1842 | Cp - Cv = nR links the two heat capacities; an early quantitative success of kinetic theory. |
| 23 | Fourier's law of heat conduction | `q = -k grad T` | thermodynamics | Joseph Fourier | 1822 | Heat flows down the temperature gradient; the constitutive law of conduction and the heat equation. |
| 24 | Chemical potential (Gibbs) | `mu = (dG/dN)_{T,P}` | statistical mechanics | J. Willard Gibbs | 1876 | The free-energy cost of adding one particle; the driver of diffusion, reactions, and phase change. |
| 25 | Critical-point conditions | `(dP/dV)_T = 0 and (d2P/dV2)_T = 0` | thermodynamics | van der Waals (locus); Gibbs (stability) | 1873 | The first and second volume-derivatives of pressure both vanish -- this locates the critical point where liquid and gas become one. |
| 26 | Van der Waals critical constants | `V_c = 3 n b,  T_c = 8 a / (27 R b),  P_c = a / (27 b^2)` | thermodynamics | Johannes van der Waals | 1873 | Solving the critical conditions gives Tc, Pc, Vc directly from the attraction a and size b -- molecular attractiveness fixes where the phase law sits. |
| 27 | De Donder chemical affinity | `A = -sum_i nu_i mu_i,  with A dxi >= 0` | chemical thermodynamics | Theophile De Donder | 1922 | A = -sum nu_i mu_i is the thermodynamic driving force of a reaction; the bridge from thermodynamics to chemical kinetics. |
| 28 | Chemical / phase equilibrium (affinity = 0) | `A = 0  =>  sum_i nu_i mu_i = 0  (phase coexistence: mu_1 = mu_2 at equal T, P)` | chemical thermodynamics | Gibbs; De Donder | 1876 | Equilibrium is the equality A = 0 (sum nu_i mu_i = 0); for coexisting phases it means equal T, P, and chemical potential. |
| 29 | Maxwell relation (dS/dV = dP/dT) | `(dS/dV)_T = (dP/dT)_V` | thermodynamics | James Clerk Maxwell | 1871 | A thermodynamic cross-derivative identity from the free energy; relates measurable quantities. |
| 30 | Maxwell relation (dS/dP = -dV/dT) | `(dS/dP)_T = -(dV/dT)_P` | thermodynamics | James Clerk Maxwell | 1871 | A Maxwell relation connecting entropy, pressure, and thermal expansion. |
| 31 | Maxwell relation (dT/dV = -dP/dS) | `(dT/dV)_S = -(dP/dS)_V` | thermodynamics | James Clerk Maxwell | 1871 | A Maxwell relation from the internal energy; links temperature and pressure derivatives. |
| 32 | Maxwell relation (dT/dP = dV/dS) | `(dT/dP)_S = (dV/dS)_P` | thermodynamics | James Clerk Maxwell | 1871 | A Maxwell relation from the enthalpy; a tool for deriving equations of state. |
| 33 | Euler relation (extensivity) | `U = T S - P V + sum mu_i N_i` | thermodynamics | J. Willard Gibbs | 1875 | Integrates the fundamental relation using extensivity; yields the Gibbs-Duhem constraint. |
| 34 | Gibbs-Duhem relation | `S dT - V dP + sum N_i dmu_i = 0` | thermodynamics | Gibbs; Duhem | 1875 | Constrains how intensive variables can jointly vary; limits independent chemical potentials. |
| 35 | Clausius inequality | `closed_integral dQ/T <= 0` | thermodynamics | Rudolf Clausius | 1865 | Entropy cannot decrease in a cycle; the mathematical form of the second law. |
| 36 | Entropy change (reversible) | `dS = dQ_rev / T` | thermodynamics | Rudolf Clausius | 1865 | Defines entropy via reversible heat over temperature; the operational definition of entropy. |
| 37 | Joule-Thomson coefficient | `mu_JT = (dT/dP)_H` | thermodynamics | Joule; Thomson | 1852 | Describes temperature change on throttling a gas; the basis of refrigeration and liquefaction. |
| 38 | Dulong-Petit law | `C_molar = 3 R` | statistical mechanics | Dulong; Petit | 1819 | Solids have roughly constant molar heat capacity; an early classical result corrected by quantum theory. |
| 39 | Einstein heat capacity | `C_V = 3 N k_B (x^2 e^x)/(e^x - 1)^2` | statistical mechanics | Albert Einstein | 1907 | The first quantum model of solid heat capacity; explained its fall at low temperature. |
| 40 | Grand canonical partition function | `Xi = sum e^(-(E - mu N)/(k_B T))` | statistical mechanics | J. Willard Gibbs | 1902 | Sums over particle number and energy; the engine of open-system statistical mechanics. |
| 41 | Grand potential | `Omega = -k_B T ln Xi = -P V` | statistical mechanics | J. Willard Gibbs | 1902 | The thermodynamic potential for open systems; equals minus pressure times volume. |
| 42 | Free energy from partition function | `F = -k_B T ln Z` | statistical mechanics | J. Willard Gibbs | 1902 | Connects microscopic states to macroscopic free energy; the bridge of statistical mechanics. |
| 43 | Equipartition theorem | `<E> = (f/2) k_B T` | statistical mechanics | Boltzmann; Maxwell | 1876 | Each quadratic degree of freedom carries kT/2 of energy; a classical cornerstone. |
| 44 | Fluctuation-dissipation (energy variance) | `<dE^2> = k_B T^2 C_V` | statistical mechanics | Einstein; Nyquist | 1905 | Energy fluctuations relate to heat capacity; a case of the fluctuation-dissipation theorem. |
| 45 | Onsager reciprocal relations | `L_ij = L_ji` | non-equilibrium thermodynamics | Lars Onsager | 1931 | Symmetry of coupled irreversible flows; the founding result of non-equilibrium thermodynamics. |
| 46 | Clapeyron equation | `dP/dT = L / (T dV)` | phase transitions | Emile Clapeyron | 1834 | The slope of a phase boundary from latent heat and volume change; governs phase coexistence. |
| 47 | Gibbs adsorption isotherm | `dgamma = -sum Gamma_i dmu_i` | surface thermodynamics | J. Willard Gibbs | 1878 | Links surface tension to adsorbed concentration; the basis of surfactant science. |
| 48 | Kirchhoff's law (enthalpy vs T) | `d(dH)/dT = dC_p` | thermochemistry | Gustav Kirchhoff | 1858 | How reaction enthalpy varies with temperature via heat capacities; a thermochemistry workhorse. |

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
