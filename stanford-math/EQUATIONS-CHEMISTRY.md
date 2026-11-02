# Covalent-Bonding Chemistry Canon

> 18 equations, sourced from `SMChemistryCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The equations of the chemical bond -- Lewis's shared electron pair, the octet rule, valence-bond and molecular-orbital theory (Heitler-London, LCAO, bond order), Pauling electronegativity, the interatomic potentials (Morse, Lennard-Jones), and the quantum rules that make bonding work (Pauli, Born-Oppenheimer, hybridization). Each row credits the chemist and year as the record gives them, with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `sum`, Greek
names spelled out) so they stay greppable and diff-friendly. The **Why it matters**
column states each result's significance in one faithful line.

## The 18 equations

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
