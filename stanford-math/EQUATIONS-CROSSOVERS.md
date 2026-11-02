# Crossover Canon

> 16 equations, sourced from `SMCrossoverCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

Applied and PhD crossovers where the mathematics leaves pure theory and lands in another field: mathematical finance, electro- and physical chemistry, cosmology, astrobiology, information theory, and mathematical biology. Each crossover is credited to its primary paper and authors, with a one-line note on why it matters; the archive claims nothing for itself.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`sum`, `product`, `laplacian`, Greek names spelled out) so they stay greppable and
diff-friendly. The **Why it matters** column states each result's significance in one
faithful line.

## The 16 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Black-Scholes PDE | `dV/dt + (1/2) sigma^2 S^2 d2V/dS2 + r S dV/dS - r V = 0` | mathematical finance | Black; Scholes; Merton | 1973 | The partial differential equation of option pricing; the founding equation of quantitative finance. |
| 2 | Black-Scholes call price | `C = S N(d1) - K e^(-r T) N(d2)` | mathematical finance | Black; Scholes; Merton | 1973 | A closed-form fair price for a call option; its publication reshaped global derivatives markets. |
| 3 | Ito's lemma | `df = (df/dt + mu df/dx + (1/2) sigma^2 d2f/dx2) dt + sigma df/dx dW` | stochastic calculus | Kiyosi Ito | 1951 | The chain rule for stochastic processes; the mathematical engine of all of mathematical finance. |
| 4 | Nernst equation | `E = E0 - (R T / (n F)) ln(Q)` | electrochemistry | Walther Nernst | 1889 | Relates electrode potential to concentration; the foundation of electrochemistry and batteries. |
| 5 | Arrhenius equation | `k = A e^(-Ea / (R T))` | physical chemistry | Svante Arrhenius | 1889 | Reaction rate rises exponentially with temperature; the central law of chemical kinetics. |
| 6 | Friedmann equation | `(adot/a)^2 = (8 pi G / 3) rho - k c^2/a^2 + Lambda c^2/3` | cosmology | Alexander Friedmann | 1922 | Governs the expansion of the universe from general relativity; the equation of modern cosmology. |
| 7 | Hubble-Lemaitre law | `v = H0 * d` | cosmology | Lemaitre; Hubble | 1927 | Galaxies recede in proportion to distance; the observational evidence for an expanding universe. |
| 8 | Drake equation | `N = R* f_p n_e f_l f_i f_c L` | astrobiology | Frank Drake | 1961 | A structured estimate of communicative civilizations; the organizing framework of SETI. |
| 9 | Shannon entropy | `H = -sum p_i log2 p_i` | information theory | Claude Shannon | 1948 | Measures information in bits; the founding quantity of information theory. |
| 10 | Shannon channel capacity | `C = B log2(1 + S/N)` | information theory | Claude Shannon | 1948 | The maximum error-free rate of a noisy channel; the hard limit all communication obeys. |
| 11 | Logistic (population) equation | `dN/dt = r N (1 - N/K)` | mathematical biology | Pierre Francois Verhulst | 1838 | Growth that saturates at a carrying capacity; the basic model of constrained populations. |
| 12 | Schrodinger-form diffusion (heat equation) | `du/dt = alpha laplacian u` | applied analysis | Joseph Fourier | 1822 | The prototype parabolic PDE; models heat, diffusion, and Brownian motion alike. |
| 13 | Michaelis-Menten kinetics | `v = V_max [S] / (K_m + [S])` | biochemistry | Michaelis; Menten | 1913 | The rate law of enzyme catalysis; the cornerstone of biochemistry and pharmacology. |
| 14 | Hardy-Weinberg principle | `p^2 + 2 p q + q^2 = 1` | population genetics | Hardy; Weinberg | 1908 | The equilibrium of allele frequencies with no selection; the null model of population genetics. |
| 15 | Lotka-Volterra predator-prey | `dx/dt = a x - b x y,  dy/dt = c x y - d y` | mathematical biology | Lotka; Volterra | 1925 | Coupled equations producing population cycles; a founding model of mathematical ecology. |
| 16 | Kelly criterion | `f* = (b p - q) / b` | mathematical finance | John L. Kelly Jr. | 1956 | The bet size that maximizes long-run growth; a bridge between information theory and investing. |

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
