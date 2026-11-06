# Covalent-Bonding Chemistry Canon

> 43 equations, sourced from `SMChemistryCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The equations of the chemical bond -- Lewis's shared electron pair, the octet rule, valence-bond and molecular-orbital theory (Heitler-London, LCAO, bond order), Pauling electronegativity, the interatomic potentials (Morse, Lennard-Jones), and the quantum rules that make bonding work (Pauli, Born-Oppenheimer, hybridization). Each row credits the chemist and year as the record gives them, with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `sum`, Greek
names spelled out) so they stay greppable and diff-friendly. The **Why it matters**
column states each result's significance in one faithful line.

## The 43 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Shared electron-pair (covalent) bond | `A. + .B -> A:B  (one shared pair = one single bond)` | chemical bonding | Gilbert N. Lewis | 1916 | Lewis's insight that a bond is a shared pair of electrons; the foundational picture of covalent bonding. |
| 2 | Octet rule | `atoms share/transfer electrons to reach 8 valence electrons` | chemical bonding | Lewis; Langmuir | 1919 | Atoms bond to reach a full outer shell of eight; the simplest predictor of molecular structure. |
| 3 | Bond order (molecular orbital) | `BO = (1/2)(n_bonding - n_antibonding)` | molecular orbital theory | Hund; Mulliken | 1932 | Net bonding from filled orbitals; predicts bond strength and whether a molecule is even stable. |
| 4 | LCAO molecular orbital | `psi_MO = c_A phi_A + c_B phi_B` | molecular orbital theory | Lennard-Jones; Mulliken | 1929 | Builds molecular orbitals from atomic ones; the practical method behind modern electronic-structure theory. |
| 5 | MO energies (two-center) | `E_pm = (alpha +/- beta) / (1 +/- S)` | molecular orbital theory | Hund-Mulliken MO theory | 1932 | Gives the bonding and antibonding energy split; shows why sharing electrons lowers energy. |
| 6 | Heitler-London valence bond (H2) | `psi = phi_A(1) phi_B(2) + phi_A(2) phi_B(1)` | valence bond theory | Heitler & London | 1927 | The first quantum explanation of a covalent bond (H2); launched quantum chemistry. |
| 7 | Pauling electronegativity difference | `chi_A - chi_B = 0.102 sqrt(E_d(AB) - sqrt(E_d(AA) E_d(BB)))` | chemical bonding | Linus Pauling | 1932 | Quantifies an atom's pull on shared electrons; predicts bond polarity and ionic character (Pauling's Nobel work). |
| 8 | Bond dissociation energy | `AB -> A + B,  dH = D(A-B)` | chemical bonding | classical thermochemistry | unknown | The energy to break a bond homolytically; the basic measure of bond strength. |
| 9 | Morse potential | `V(r) = D_e (1 - e^(-a (r - r_e)))^2` | molecular physics | Philip M. Morse | 1929 | A realistic bond potential with dissociation; the standard model of a vibrating, breakable bond. |
| 10 | Lennard-Jones (12-6) potential | `V(r) = 4 eps ((sigma/r)^12 - (sigma/r)^6)` | molecular physics | John Lennard-Jones | 1924 | Captures attraction and repulsion between atoms; the workhorse potential of molecular simulation. |
| 11 | Coulomb's law (two charges) | `F = k_e q1 q2 / r^2` | electrostatics of bonding | Charles-Augustin de Coulomb | 1785 | The electrostatic force between charges; the physical root of all chemical bonding. |
| 12 | Born-Oppenheimer separation | `psi_total ~ psi_electronic * psi_nuclear` | quantum chemistry | Born & Oppenheimer | 1927 | Separates fast electrons from slow nuclei; the approximation that makes quantum chemistry tractable. |
| 13 | Hybridization (sp^n directed bonds) | `sp^n gives (n+1) equivalent directed orbitals` | valence bond theory | Linus Pauling | 1931 | Mixes atomic orbitals into directed bonds; explains molecular shapes like tetrahedral carbon. |
| 14 | Formal charge | `FC = V - N_nonbonding - (1/2) N_bonding` | chemical bonding | standard Lewis-structure rule | unknown | Bookkeeping for electrons in a Lewis structure; picks the most plausible structure among alternatives. |
| 15 | Molecular dipole moment | `mu = sum_i q_i r_i` | molecular polarity | Peter Debye | 1912 | Measures charge separation in a molecule; governs polarity, solubility, and intermolecular forces (Debye's Nobel work). |
| 16 | Hooke's law bond (harmonic stretch) | `V(r) = (1/2) k (r - r_e)^2` | molecular vibration | classical (Hooke form) | unknown | Treats a bond as a spring near equilibrium; the basis of vibrational spectroscopy and force fields. |
| 17 | Pauli exclusion principle | `no two electrons share all four quantum numbers` | quantum chemistry | Wolfgang Pauli | 1925 | No two electrons share all quantum numbers; why electrons pair and shells fill as they do. |
| 18 | Aufbau / Hund's rule filling | `fill lowest orbitals first; maximize unpaired spin` | atomic structure | Hund; Aufbau principle | 1927 | Rules for filling orbitals; explain electron configurations and magnetic behavior. |
| 19 | Henderson-Hasselbalch equation | `pH = pKa + log10([A-]/[HA])` | acid-base chemistry | Henderson; Hasselbalch | 1917 | Relates pH to acid dissociation and buffer ratio; the working formula for buffers. |
| 20 | pH definition | `pH = -log10[H+]` | acid-base chemistry | S. P. L. Sorensen | 1909 | Defines acidity as the negative log of hydrogen-ion activity; the universal acidity scale. |
| 21 | Water autoionization constant | `Kw = [H+][OH-] = 1.0e-14 at 25 C` | acid-base chemistry | classical | unknown | Fixes the product of hydrogen and hydroxide concentrations in water; anchors the pH scale. |
| 22 | Equilibrium constant | `K = product[products]^n / product[reactants]^m` | chemical equilibrium | Guldberg & Waage | 1864 | Sets the ratio of products to reactants at equilibrium; the law of chemical balance. |
| 23 | Law of mass action (rate) | `rate = k [A]^m [B]^n` | chemical kinetics | Guldberg & Waage | 1864 | Reaction rate depends on reactant concentrations raised to their orders; the basis of kinetics. |
| 24 | First-order rate law | `[A] = [A]0 e^(-k t)` | chemical kinetics | classical | unknown | Concentration decays exponentially; describes radioactive-style and many chemical decays. |
| 25 | Second-order rate law | `1/[A] = 1/[A]0 + k t` | chemical kinetics | classical | unknown | Inverse concentration grows linearly in time; a common kinetic order. |
| 26 | Arrhenius equation (rate) | `k = A e^(-Ea / (R T))` | chemical kinetics | Svante Arrhenius | 1889 | Reaction rate rises exponentially with temperature; defines activation energy. |
| 27 | Eyring equation (transition state) | `k = (k_B T / h) e^(-dG_act / (R T))` | chemical kinetics | Eyring; Polanyi; Evans | 1935 | Derives rate constants from transition-state thermodynamics; the quantum-statistical rate law. |
| 28 | Van 't Hoff equation | `d(ln K)/dT = dH / (R T^2)` | chemical thermodynamics | Jacobus van 't Hoff | 1884 | How an equilibrium constant shifts with temperature; links kinetics to thermodynamics. |
| 29 | Gibbs free energy of reaction | `dG = dH - T dS` | chemical thermodynamics | J. Willard Gibbs | 1876 | Balances enthalpy against entropy to decide spontaneity; the master criterion of reactions. |
| 30 | Gibbs energy and equilibrium | `dG = -R T ln K` | chemical thermodynamics | J. Willard Gibbs | 1876 | Connects standard free energy to the equilibrium constant; the thermodynamics of equilibrium. |
| 31 | Nernst equation (cell potential) | `E = E0 - (R T / (n F)) ln Q` | electrochemistry | Walther Nernst | 1889 | Gives a cell's voltage away from standard conditions; the foundation of electrochemistry. |
| 32 | Faraday's laws of electrolysis | `m = (Q / F)(M / z)` | electrochemistry | Michael Faraday | 1834 | Mass deposited is proportional to charge passed; the quantitative law of electrolysis. |
| 33 | Butler-Volmer equation | `j = j0 [e^(a1 f eta) - e^(-a2 f eta)]` | electrochemistry | Butler; Volmer | 1930 | Relates electrode current to overpotential; the central equation of electrode kinetics. |
| 34 | Debye-Huckel limiting law | `log10 gamma = -A z+ z- sqrt(I)` | physical chemistry | Debye; Huckel | 1923 | Predicts activity coefficients in dilute electrolytes; the theory of ionic solutions. |
| 35 | Raoult's law | `p_i = x_i p_i*` | solution chemistry | Francois-Marie Raoult | 1887 | A solvent's vapor pressure scales with its mole fraction; the basis of ideal solutions. |
| 36 | Henry's law | `p = k_H c` | solution chemistry | William Henry | 1803 | Dissolved gas is proportional to its partial pressure; governs solubility of gases. |
| 37 | Gibbs phase rule | `F = C - P + 2` | phase equilibria | J. Willard Gibbs | 1875 | Counts the degrees of freedom of a system at equilibrium; organizes phase diagrams. |
| 38 | Colligative freezing-point depression | `dTf = i K_f m` | solution chemistry | Francois-Marie Raoult | 1878 | Solutes lower the freezing point in proportion to concentration; a colligative property. |
| 39 | Osmotic pressure (van 't Hoff) | `Pi = i M R T` | solution chemistry | Jacobus van 't Hoff | 1886 | Solute concentration drives osmotic pressure like an ideal gas; central to biology and chemistry. |
| 40 | Langmuir adsorption isotherm | `theta = K p / (1 + K p)` | surface chemistry | Irving Langmuir | 1918 | Models monolayer adsorption saturating with pressure; the base model of surface chemistry. |
| 41 | BET isotherm (multilayer) | `multilayer adsorption extension of Langmuir` | surface chemistry | Brunauer; Emmett; Teller | 1938 | Extends Langmuir to multilayer adsorption; the standard method for measuring surface area. |
| 42 | Beer-Lambert absorbance | `A = epsilon l c` | spectroscopy | Beer; Lambert | 1852 | Absorbance is proportional to concentration and path; the basis of quantitative spectroscopy. |
| 43 | Clausius-Clapeyron (vapor pressure) | `ln(p2/p1) = -(dH_vap / R)(1/T2 - 1/T1)` | physical chemistry | Clausius; Clapeyron | 1834 | How vapor pressure rises with temperature; relates boiling to the heat of vaporization. |

## Validate

These equations are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints every entry and the running total across
all canons):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Names, forms, discoverers, and years follow the standard history-of-science / history-
of-economics record; the **Why it matters** notes summarize each result's established
significance. See `SMProvenance.sleela` for the tiered source registry and the repo
`README.md` for per-class integrity goals. This archive records and attributes
published results; it does not originate, own, or certify them.

## Definitions & relevance (second-layer additions)

The entries below are the second-layer additions to this canon, organized by
type. Each gives a short **definition** (what the equation states and what its
symbols mean) and its **relevance** (why it is foundational to chemistry). These
broaden the canon from covalent bonding out into the rest of general chemistry —
acids and bases, kinetics, thermodynamics, electrochemistry, solutions, and
surfaces.

### Acid-base chemistry

**pH definition** — `pH = -log10[H+]`. Defines acidity as the negative base-10
logarithm of the hydrogen-ion activity, compressing an enormous concentration
range into a convenient scale. It is the universal language of acidity, the
reference point every other acid-base relation is expressed against.

**Water autoionization constant** — `Kw = [H+][OH-] = 1.0e-14 at 25 C`. Water
self-ionizes so that the product of hydrogen- and hydroxide-ion concentrations is
a fixed constant at a given temperature. It anchors the entire pH scale and fixes
the neutral point (pH 7 at 25 C), making acidity and basicity two sides of one
equilibrium.

**Henderson-Hasselbalch equation** — `pH = pKa + log10([A-]/[HA])`. Expresses the
pH of a buffer in terms of the acid's pKa and the ratio of conjugate base to
acid. It is the working formula of buffer chemistry and physiology, explaining
how blood and reagents resist pH change near `pH = pKa`.

### Chemical equilibrium & kinetics

**Equilibrium constant** — `K = product[products]^n / product[reactants]^m`. At
equilibrium the concentrations settle to a fixed ratio captured by `K`; large `K`
favors products, small `K` favors reactants. It is the central quantitative
statement of where a reaction comes to rest.

**Law of mass action (rate)** — `rate = k [A]^m [B]^n`. Reaction rate is
proportional to reactant concentrations raised to their reaction orders. It is
the foundational postulate of chemical kinetics, from which individual rate laws
are built.

**First-order rate law** — `[A] = [A]0 e^(-k t)`. A reactant whose rate depends
linearly on its own concentration decays exponentially in time, with a
concentration-independent half-life. It is the kinetic template for radioactive
decay, many drug-elimination processes, and unimolecular steps.

**Second-order rate law** — `1/[A] = 1/[A]0 + k t`. When rate depends on the
square of a concentration (or the product of two), the inverse concentration
grows linearly in time. It is a common and easily-diagnosed kinetic order,
identified by a straight line in a `1/[A]` plot.

**Arrhenius equation (rate)** — `k = A e^(-Ea / (R T))`. The rate constant rises
exponentially with temperature, controlled by an activation energy `Ea`. It
quantifies why heating speeds reactions and defines the activation barrier that
dominates reaction engineering.

**Eyring equation (transition state)** — `k = (k_B T / h) e^(-dG_act / (R T))`.
Transition-state theory derives the rate constant from the free energy of
reaching the activated complex. Its relevance is a molecular, thermodynamic
account of rate that complements Arrhenius's empirical form.

### Chemical thermodynamics

**Van 't Hoff equation** — `d(ln K)/dT = dH / (R T^2)`. Describes how an
equilibrium constant shifts with temperature according to the reaction enthalpy.
It links equilibrium to thermochemistry and is the basis for extracting `dH`
from the temperature dependence of `K`.

**Gibbs free energy of reaction** — `dG = dH - T dS`. Balances enthalpy against
entropy at a given temperature; a negative `dG` marks a spontaneous process. It
is the master criterion that decides the direction of chemical change.

**Gibbs energy and equilibrium** — `dG = -R T ln K`. Ties the standard free-energy
change directly to the equilibrium constant. It is the quantitative bridge
between thermodynamics and equilibrium, converting energies into predicted
compositions.

### Electrochemistry

**Nernst equation (cell potential)** — `E = E0 - (R T / (n F)) ln Q`. Gives a
cell's voltage away from standard conditions as a function of the reaction
quotient `Q`. It is the foundation of electrochemistry, underlying batteries,
sensors, and the resting potentials of living cells.

**Faraday's laws of electrolysis** — `m = (Q / F)(M / z)`. The mass deposited or
dissolved at an electrode is proportional to the electric charge passed. They are
the quantitative basis of electroplating, electrorefining, and coulometry.

**Butler-Volmer equation** — `j = j0 [e^(a1 f eta) - e^(-a2 f eta)]`. Relates the
net current at an electrode to the overpotential `eta`, combining forward and
reverse electron-transfer rates. It is the central equation of electrode
kinetics, governing fuel cells, batteries, and corrosion.

### Physical & solution chemistry

**Debye-Huckel limiting law** — `log10 gamma = -A z+ z- sqrt(I)`. Predicts how
ion–ion interactions lower activity coefficients in dilute electrolytes, scaling
with the square root of ionic strength. It is the founding theory of real
(non-ideal) electrolyte solutions.

**Clausius-Clapeyron (vapor pressure)** — `ln(p2/p1) = -(dH_vap / R)(1/T2 - 1/T1)`.
Relates a liquid's vapor pressure at two temperatures to its enthalpy of
vaporization. It connects boiling behavior to a measurable thermodynamic quantity
and lets one predict boiling points under changed pressure.

**Raoult's law** — `p_i = x_i p_i*`. In an ideal solution each component's vapor
pressure equals its mole fraction times its pure-component vapor pressure. It is
the reference behavior of solutions, the baseline against which real deviations
(and distillation) are understood.

**Henry's law** — `p = k_H c`. The amount of a gas dissolved in a liquid is
proportional to its partial pressure above the liquid. It governs gas solubility
from carbonated drinks to oxygen in blood and the bends in divers.

**Colligative freezing-point depression** — `dTf = i K_f m`. Adding solute lowers
a solvent's freezing point in proportion to molal concentration (and the van 't
Hoff factor `i`). It is a colligative property depending on particle count, not
identity — the principle behind road salt and antifreeze.

**Osmotic pressure (van 't Hoff)** — `Pi = i M R T`. Dissolved particles exert an
osmotic pressure formally identical to an ideal gas law in concentration. It is
central to biology (cell turgor, dialysis) and to measuring molar masses of
macromolecules.

### Phase equilibria, surfaces & spectroscopy

**Gibbs phase rule** — `F = C - P + 2`. Counts the degrees of freedom `F` of a
system from its components `C` and coexisting phases `P`. It organizes phase
diagrams, telling us how many conditions can vary independently at equilibrium.

**Langmuir adsorption isotherm** — `theta = K p / (1 + K p)`. Models surface
coverage as monolayer adsorption that saturates as pressure rises. It is the
base model of surface chemistry and heterogeneous catalysis.

**BET isotherm (multilayer)** — a multilayer extension of Langmuir adsorption. It
is the standard method for measuring the surface area of porous solids and
catalysts from gas-adsorption data.

**Beer-Lambert absorbance** — `A = epsilon l c`. Absorbance is proportional to the
molar absorptivity, path length, and concentration. It is the foundation of
quantitative spectroscopy, turning a light measurement into a concentration.
