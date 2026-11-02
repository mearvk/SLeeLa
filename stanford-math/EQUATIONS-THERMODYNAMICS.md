# Thermodynamics & Statistical Mechanics Canon

> 24 equations, sourced from `SMThermoEquations.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

Thermodynamics and statistical mechanics from the four laws outward: entropy (Clausius, Boltzmann, Gibbs), the free energies, equations of state, the Maxwell-Boltzmann distribution and partition function, and the bridge to information at Landauer's limit. This is the reach the Stanford probability tradition touches through statistical mechanics.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`sum`, `product`, `laplacian`, Greek names spelled out) so they stay greppable and
diff-friendly.

## The 24 equations

| # | Equation | Form | Field | Discoverer(s) | Year |
|---|----------|------|-------|---------------|------|
| 1 | Zeroth law of thermodynamics | `A~C and B~C  =>  A~B (thermal equilibrium is transitive)` | thermodynamics | Fowler & Guggenheim (named) | 1939 |
| 2 | First law of thermodynamics | `dU = dQ - dW` | thermodynamics | Clausius; Joule; Mayer | 1850 |
| 3 | Second law (Clausius) | `dS >= dQ / T,  dS_universe >= 0` | thermodynamics | Rudolf Clausius | 1865 |
| 4 | Third law of thermodynamics | `S -> 0 as T -> 0 (perfect crystal)` | thermodynamics | Walther Nernst | 1906 |
| 5 | Entropy (Clausius definition) | `dS = dQ_rev / T` | thermodynamics | Rudolf Clausius | 1865 |
| 6 | Boltzmann entropy | `S = k * ln(W)` | statistical mechanics | Ludwig Boltzmann | 1877 |
| 7 | Gibbs entropy | `S = -k * sum p_i ln p_i` | statistical mechanics | J. Willard Gibbs | 1878 |
| 8 | Ideal gas law | `P V = n R T` | thermodynamics | Clapeyron (combined) | 1834 |
| 9 | Gibbs free energy | `G = H - T S` | thermodynamics | J. Willard Gibbs | 1873 |
| 10 | Helmholtz free energy | `F = U - T S` | thermodynamics | Hermann von Helmholtz | 1882 |
| 11 | Enthalpy | `H = U + P V` | thermodynamics | Kamerlingh Onnes (term) | 1909 |
| 12 | Carnot efficiency | `eta = 1 - T_cold / T_hot` | thermodynamics | Sadi Carnot | 1824 |
| 13 | Maxwell-Boltzmann distribution | `f(v) ~ v^2 e^(-m v^2 / (2 k T))` | statistical mechanics | Maxwell; Boltzmann | 1860 |
| 14 | Boltzmann factor | `P(E) ~ e^(-E / (k T))` | statistical mechanics | Ludwig Boltzmann | 1868 |
| 15 | Partition function | `Z = sum_i e^(-E_i / (k T))` | statistical mechanics | Boltzmann; Gibbs | 1901 |
| 16 | Stefan-Boltzmann law | `j = sigma T^4` | thermodynamics | Stefan; Boltzmann | 1879 |
| 17 | Clausius-Clapeyron relation | `dP/dT = L / (T dV)` | thermodynamics | Clausius; Clapeyron | 1834 |
| 18 | Gibbs-Helmholtz equation | `d(G/T)/dT = -H / T^2` | thermodynamics | Gibbs; Helmholtz | 1882 |
| 19 | Sackur-Tetrode equation | `S = N k [ln(V/N (4 pi m U/(3 N h^2))^{3/2}) + 5/2]` | statistical mechanics | Sackur; Tetrode | 1912 |
| 20 | Landauer's principle | `E_min = k T ln(2) per bit erased` | information thermodynamics | Rolf Landauer | 1961 |
| 21 | Van der Waals equation of state | `(P + a n^2/V^2)(V - n b) = n R T` | thermodynamics | Johannes van der Waals | 1873 |
| 22 | Heat capacity relation (ideal gas) | `C_p - C_v = n R` | thermodynamics | Mayer's relation | 1842 |
| 23 | Fourier's law of heat conduction | `q = -k grad T` | thermodynamics | Joseph Fourier | 1822 |
| 24 | Chemical potential (Gibbs) | `mu = (dG/dN)_{T,P}` | statistical mechanics | J. Willard Gibbs | 1876 |

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
see `SMProvenance.sleela` for the tiered source registry (primary papers, secondary
references such as NIST DLMF / Abramowitz & Stegun, and tertiary locators including
Wikipedia, cited as CC BY-SA and never treated as a primary authority) and the repo
`README.md` for per-class integrity goals. This archive records and attributes
published results; it does not originate, own, or certify them.
