# stanford-math

> An honest record of the **Stanford Math brand** as it can be known from the
> public record — the people, the named formulas and symbols, and how far the
> thermodynamic / statistical-mechanics line of the Stanford mathematical
> tradition actually reaches.

This directory is a *metadata archive*, not a numerical kernel. Every class is a
SLeeLa (`.sleela`) record carrying the executive-grade **Metadata Class
metapattern** (see `.kiro/steering/metadata-class-standard.md` and
`SLEELA.syntax` §15). The one hard rule governs everything here: **values are
faithful to the subject's real origin where publicly known**; unknowns are
`"none"` / `"unknown"`, never invented to look legitimate.

## What is recorded

| Class | Subject | Real, citable anchor |
|-------|---------|----------------------|
| `SMBrand`          | The Stanford Department of Mathematics as a brand | Stanford Dept. of Mathematics, founded 1891 (university charter) |
| `SMPersonRegistry` | Named Stanford mathematicians | Dept. faculty/emeriti rosters (see Sources) |
| `SMFormulaCatalog` | Named formulas/theorems tied to Stanford names | Peer-reviewed literature (see Sources) |
| `SMSymbolTable`    | Standard symbols used by those formulas | Established mathematical notation |
| `SMThermodynamics` | How far Stanford's thermodynamics/stat-mech reaches | Research-area pages, course records, lectures |
| `SMEquation`       | One equation record (name, form, field, discoverer, year) | — |
| `SMMathCanon`      | Foundational mathematics equations/identities | Standard references (see Sources) |
| `SMPhysicsCanon`   | Foundational physics equations | Standard references (see Sources) |
| `SMThermoEquations`| Thermodynamics & statistical-mechanics equations | Standard references (see Sources) |
| `SMCrossoverCanon` | Applied/PhD crossovers (finance, chemistry, cosmology, info) | Standard references (see Sources) |
| `SMFinanceCanon`   | Finance & economics equations (money, capitalism, credit) | Standard references (see Sources) |
| `SMChemistryCanon` | Covalent-bonding chemistry equations | Standard references (see Sources) |
| `SMSocialCanon`    | Social-structure equations (ties, networks, wealth distribution) | Standard references (see Sources) |
| `SMEventsCanon`    | Calculus-of-events equations (collision, sound, departure, routine) | Standard references (see Sources) |
| `SMProvenance`     | Purpose/method/gains/bedrock + tiered literature callouts (Wikipedia attributed) | Primary/secondary/tertiary sources |
| `SMArchive` (demo) | Runnable entry point that broadcasts the archive | — |

## The equation canon (248 entries)

> On "all of the equations known in the known Universe": **no finite, complete
> list of every equation exists** — and fabricating one would violate this
> repo's one hard rule (values faithful to real origin; unknowns are `"none"`,
> never invented). So this archive records a **curated, fully-attributed canon**
> of the genuinely foundational equations that every math/physics PhD rests on:
> **248 entries**, each with its real name, symbolic form, discoverer(s), year,
> and field.

Breakdown (counts verified by running `SMArchive`):

- **Mathematics — 75:** Pythagoras, Euler's identity/formula, FTC, binomial
  theorem, Taylor & Fourier series, Cauchy–Schwarz, Gauss/Green/Stokes,
  Cauchy integral & residue theorems, Riemann zeta functional equation, Euler
  product, prime number theorem, Bayes, CLT, Stirling, Gamma, Basel problem,
  Euler–Mascheroni, golden ratio, polyhedron formula, eigenvalue equation,
  Green–Tao, plus L'Hôpital's rule, integration by parts, chain rule, Jensen &
  AM–GM inequalities, geometric series, Euler's totient, Fermat's little
  theorem, Cantor's diagonal cardinality, Laplace's equation, Euler's totient
  theorem, Gauss's quadratic reciprocity, Cramer's rule, the Wallis product,
  Lagrange's mean value theorem, the intermediate value theorem, Wilson's
  theorem, the Chinese remainder theorem, the Vandermonde determinant,
  Cayley–Hamilton, Gram–Schmidt, Parseval's identity, Chebyshev's inequality,
  the fundamental theorem of algebra, Rolle's theorem, the Cauchy–Riemann
  equations, the argument principle, Lagrange interpolation, Newton's method,
  the singular value decomposition, Bolzano–Weierstrass, the Banach
  fixed-point theorem, Hölder & Minkowski inequalities, the weak law of large
  numbers, Markov's inequality, Möbius inversion, Dirichlet on primes in
  arithmetic progression, and the Catalan numbers, and more.
- **Physics — 46:** Newton's three laws & gravitation, Lagrange/Hamilton, Coulomb,
  all four Maxwell equations, Lorentz force, `E=mc^2`, Einstein field equations,
  Schwarzschild radius, Planck relation & law, de Broglie, both Schrödinger
  equations, Heisenberg uncertainty, Dirac, canonical commutation, Bohr levels,
  Rydberg, photoelectric, Navier–Stokes, wave equation, Bernoulli, plus simple
  harmonic motion, centripetal acceleration, Doppler, Snell's law, Ohm's law,
  Compton scattering, and Wien's displacement law.
- **Thermodynamics & statistical mechanics — 28:** the four laws, Clausius
  entropy, Boltzmann `S=k ln W`, Gibbs entropy, ideal gas law, Gibbs/Helmholtz
  free energies, enthalpy, Carnot efficiency, Maxwell–Boltzmann, Boltzmann
  factor, partition function, Stefan–Boltzmann, Clausius–Clapeyron,
  Gibbs–Helmholtz, Sackur–Tetrode, Landauer's principle, van der Waals,
  Mayer's heat-capacity relation, Fourier's law of conduction, the Gibbs
  chemical potential, plus the **critical-point conditions** and **van der Waals
  critical constants** (where molecular attraction locates the phase law), **De
  Donder's chemical affinity**, and the **equilibrium/equality condition**
  (affinity = 0; equal T, P, and chemical potential across coexisting phases).
- **Crossovers — 16:** Black–Scholes (PDE & call price), Itô's lemma, Nernst,
  Arrhenius, Friedmann, Hubble–Lemaître, Drake, Shannon entropy & channel
  capacity, logistic equation, heat equation, plus Michaelis–Menten kinetics,
  Hardy–Weinberg, Lotka–Volterra, and the Kelly criterion.
- **Finance & economics — 25:** the equation of exchange / quantity theory of
  money, compound & continuous interest, present value, annuities & perpetuities,
  Gordon growth, the Fisher equation, the money multiplier, CAPM & beta,
  Modigliani–Miller, WACC, bond pricing & Macaulay duration, the Sharpe ratio,
  Markowitz portfolio variance, put–call parity, Okun's law, the Phillips curve,
  the Taylor rule, Cobb–Douglas, Solow growth, MC = MR, and **Little's Law**
  (`L = λW`, the throughput–inventory relation behind economic "pace").
- **Covalent-bonding chemistry — 18:** the Lewis shared electron pair, the octet
  rule, MO bond order, LCAO & two-center MO energies, Heitler–London valence
  bond, Pauling electronegativity, bond dissociation energy, the Morse and
  Lennard-Jones potentials, Coulomb's law, Born–Oppenheimer, hybridization,
  formal charge, dipole moment, the harmonic-bond law, Pauli exclusion, and
  Aufbau/Hund filling.
- **Social structure — 18:** network density, clustering & centrality (degree,
  betweenness), tie strength & the strength of weak ties (Granovetter), tie
  overlap, homophily, social capital (bonding vs bridging), structural holes,
  Dunbar's number, preferential attachment, small-world path length, modularity,
  assortativity, and wealth distribution (Gini, Lorenz, Pareto). These measure
  **structure and distribution — not persons**.
- **Calculus of events — 22:** a canon of the equations that govern *things that
  happen* rather than steady states, in four kinds. **Collision** (impact):
  impulse–momentum, coefficient of restitution, 1-D elastic and perfectly
  inelastic collisions, König's kinetic-energy split, Rutherford scattering.
  **Sound** (a pressure event propagating and heard): speed of sound
  (Newton–Laplace), the acoustic wave and Helmholtz equations, the Doppler
  effect, the decibel, the Webster horn equation, Sabine reverberation.
  **Departure** (arrivals-and-departures / first passage): the Poisson arrival
  process, M/M/1 and Erlang-C queues, Pollaczek–Khinchine, Kramers escape.
  **Routine** (periodic, recurrent, renewal): the simple-harmonic period, the
  Fourier transform, Poincaré recurrence, and the elementary renewal theorem.

Each group has a full human-readable table — every equation with its symbolic
form, field, discoverer(s), and year — mirrored directly from the `.sleela`
canon (the single source of truth):

- [`EQUATIONS-MATH.md`](EQUATIONS-MATH.md) — 75 mathematics equations (`SMMathCanon`)
- [`EQUATIONS-PHYSICS.md`](EQUATIONS-PHYSICS.md) — 46 physics equations (`SMPhysicsCanon`)
- [`EQUATIONS-THERMODYNAMICS.md`](EQUATIONS-THERMODYNAMICS.md) — 28 thermodynamics & statistical-mechanics equations (`SMThermoEquations`)
- [`EQUATIONS-CROSSOVERS.md`](EQUATIONS-CROSSOVERS.md) — 16 applied/PhD crossovers (`SMCrossoverCanon`)
- [`EQUATIONS-FINANCE.md`](EQUATIONS-FINANCE.md) — 25 finance & economics equations (`SMFinanceCanon`)
- [`EQUATIONS-CHEMISTRY.md`](EQUATIONS-CHEMISTRY.md) — 18 covalent-bonding chemistry equations (`SMChemistryCanon`)
- [`EQUATIONS-SOCIAL.md`](EQUATIONS-SOCIAL.md) — 18 social-structure equations (`SMSocialCanon`)
- [`EQUATIONS-EVENTS.md`](EQUATIONS-EVENTS.md) — 22 calculus-of-events equations (`SMEventsCanon`)

Where attribution is genuinely diffuse (e.g. the quadratic formula, the 2×2
determinant), the discoverer field says so honestly rather than crediting a
single name.

## The metadata pattern, extended into a scientific record

The baseline Metadata metapattern (`SLEELA.syntax` §15) records *who made this,
under what authority, with what intent*. This archive extends it — using only
ordinary catalogued fields + accessors, exactly as §15 sanctions — with a
second greppable block that makes each class state its standing as a **scientific
record**:

```sleela
  < ---- Scientific record (purpose / method / gains / bedrock / integrity) ---- >
  String metaPurpose           = "...";   < what this record is for >
  String metaScientificMethod  = "...";   < the method it follows >
  String metaAuthorialGains    = "...";   < what its authors legitimately gain >
  String metaBedrock           = "...";   < the foundation the method rests on >
  String metaIntegrityGoal     = "...";   < the standing promise the record keeps >
```

This block is present on **all 14 classes** in the archive, and — this is the
point — it is **written truthfully for each class, not copied**. The purpose of
`SMEquation` ("hold one equation faithfully") is not the purpose of `SMArchive`
("broadcast the whole, unaltered"); the gains of a public-domain theorem differ
from the gains of a cited 1973 finance paper. *Not the same.*

| Facet | What it declares (archive-wide spirit) |
|-------|----------------------------------------|
| **Purpose** | The record's reason to exist: *carry, not create.* |
| **Perceived scientific method** | **Tertiary cataloguing** — record claims traceable to primary/secondary literature; originate nothing. |
| **Authorial gains** | Curatorial credit for the *catalogue only*. Public-domain science is owned by no one; cited results belong to their authors/publishers. The archive claims no authorship over the works it cites. |
| **Bedrock of the method** | The peer-reviewed and historical scientific record — primary sources where citable, tertiary indexes to locate them. |
| **Integrity Goal** | The standing promise: **allow greatness to be recorded — fully and forward — without inflating it.** Record a great move exactly as it happened, credit the real author, claim nothing for the archive, and mark honestly where each reach stops. |

### The Integrity Goal, per record (each distinct)

| Class | Its own Integrity Goal (abridged) |
|-------|-----------------------------------|
| `SMBrand` | Record a great institution by its own public record — honor the name, never trade on it. |
| `SMPersonRegistry` | Record mathematicians the public record names — never add for prestige, never omit who belongs. |
| `SMFormulaCatalog` | Record a move as Stanford's only where the record truly ties it there — attribute the person, not the brand. |
| `SMSymbolTable` | Let a symbol mean what the field means — never bend it to fit. |
| `SMThermodynamics` | Record the great reach fully, out to black-hole thermodynamics — while marking where the math dept ends and Physics begins. |
| `SMMathCanon` / `SMPhysicsCanon` / `SMThermoEquations` | Record each great result under the name history/experiment gives it — credit the discoverer, claim nothing. |
| `SMCrossoverCanon` | Record each crossover with its primary paper — credit the authors/publishers, claim nothing. |
| `SMEventsCanon` | Record each event-law under the name history gives it — impact, pressure, arrival, or return — crediting the discoverer, claiming nothing. |
| `SMProvenance` | Tier every source honestly — trace greatness to its origin, not a convenient index. |
| `SMEquation` | Record the move exactly as it happened — great or small — crediting the real author, never the archive. |
| `SMArchive` | Read the greatness aloud exactly as recorded — adding nothing, hiding nothing. |

### Calls out to sane, published literature (`SMProvenance`)

`SMProvenance` is the provenance registry. It tiers its sources honestly:

- **Primary** — the original papers/treatises (Newton's *Principia*, Euler's
  *Introductio*, Maxwell 1865, Einstein 1905/1915, Shannon 1948, Black–Scholes
  1973).
- **Secondary** — standard references (NIST DLMF, Abramowitz & Stegun,
  Gradshteyn & Ryzhik, MTW *Gravitation*, Jackson, the CODATA/SI Brochure).
- **Tertiary** — locators, including **Wikipedia**, cited with respect and
  attributed honestly: a *tertiary locator*, content under **CC BY-SA 4.0**,
  never treated as a primary authority. Also Wolfram MathWorld and the Stanford
  Encyclopedia of Philosophy.

The registry carries a standing disclaimer: *this archive records and attributes
published results; it does not originate, own, or certify them.* That is the
§15.5 fidelity rule applied to the question of authorial gains — the record
stays honest about exactly how much it may claim.

## People recorded (public record)

Historical and current figures publicly associated with Stanford mathematics,
including: **George Pólya** (random-walk recurrence; the Pólya enumeration
theorem), **Charles Loewner / Karl Löwner** (Loewner's theorem on operator
monotone functions), **Paul Cohen** (forcing; independence of the continuum
hypothesis; Fields Medal 1966), **Persi Diaconis** (riffle-shuffle mixing — the
"seven shuffles" result with Dave Bayer), **Samuel Karlin** (total positivity,
birth–death processes), **Joseph B. Keller** (the geometrical theory of
diffraction), **Donald Ornstein** (the isomorphism theorem in ergodic theory),
**Yakov Eliashberg** (symplectic/contact topology), and **Robert Osserman**
(minimal surfaces). Rosters drawn from Stanford's own bulletins and faculty
pages.

> Full roster with years, field, Stanford standing, best-known contribution, and
> a "why it matters" note: [`PEOPLE.md`](PEOPLE.md) (mirrors `SMPersonRegistry`).

## Named formulas / symbols (examples recorded)

- **"Seven shuffles suffice" (Bayer–Diaconis, 1992):** riffle-shuffle mixing
  occurs, to leading order, after `(3/2) · log2(N)` shuffles for `N` cards.
- **Pólya's recurrence theorem (1921):** simple random walk on `Z^d` is
  recurrent for `d = 1, 2` and transient for `d ≥ 3`.
- **Pólya enumeration theorem:** counts orbits of colored arrangements via the
  cycle index of the acting group.
- **Loewner's theorem:** a real function is operator monotone on an interval iff
  it extends to an analytic Pick (Nevanlinna) function on the upper half-plane.
- **Geometrical theory of diffraction (Keller, 1962):** extends geometrical
  optics with diffracted rays from edges, corners, and grazing surfaces.

> Full table of all six Stanford-anchored results with symbolic form, citation,
> and a "why it matters" note: [`FORMULAS.md`](FORMULAS.md) (mirrors
> `SMFormulaCatalog`).

## The reach of "their thermodynamics"

Within the **Stanford Department of Mathematics** proper, thermodynamics appears
through **statistical mechanics inside the probability group** (Markov chains,
Gibbs measures, entropy), not as a standalone chair. The reach extends outward:

- **Probability group** research explicitly lists problems from *statistical
  mechanics*.
- **Entropy** links to information via the broader Stanford intellectual
  tradition (the *Stanford Encyclopedia of Philosophy* entry on information
  processing and thermodynamic entropy).
- At the frontier, Stanford mathematics touches **black-hole thermodynamics**
  (the classical-thermodynamics / black-hole-mechanics analogy) through
  geometry/relativity colloquia — this is the far edge of how far the line
  reaches.
- Formal **thermodynamics & statistical mechanics courses** sit in Physics
  (e.g. Physics 170), i.e. the *teaching* of thermodynamics is adjacent to, not
  inside, the math department. `SMThermodynamics` records this boundary
  honestly.

So: **the Stanford Math brand's thermodynamics reaches from entropy and Gibbs
measures in probability, through statistical-mechanics analysis of Markov
chains, out to the thermodynamic analogies of general relativity — while the
foundational thermodynamics coursework lives in Physics.**

## Validate

These are library classes; validate through the sibling demo `SMArchive`.
Because this archive lives at the **repo root** (not under `lib/`, which is the
only tree the auto-linker scans), point `SLEELA_LIB` at this directory so the
sibling records resolve:

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

Build the source compiler first if needed (the prebuilt `working-binaries/sleela`
tops out at syntax 1.11 and cannot check these `#sleela 2.1` files):

```sh
cd impl && SLEELA_SHA256_MANIFEST="$PWD/../security/sha256-manifest.json" make build/sleela
```

> Note: the SHA-256 verified-build manifest covers only the toolchain and the
> `lib/` standard library; user programs at the repo root (like this archive)
> are deliberately outside the gate, so no manifest regeneration is required.

## Sources

- Stanford Department of Mathematics — Faculty & Lecturers; Research Areas;
  Probability / Number Theory / Combinatorics pages (`mathematics.stanford.edu`).
- Stanford Registrar bulletins (emeriti/professor rosters), `web.stanford.edu`.
- D. Bayer & P. Diaconis, "Trailing the Dovetail Shuffle to its Lair" (1992);
  "seven shuffles" / `(3/2)log2 N` cutoff (arXiv:2510.22783 restates it).
- G. Pólya (1921), random-walk recurrence; Pólya enumeration theorem
  (Wikipedia; arXiv:1301.3916).
- K. Löwner (1934), operator monotone functions — Loewner's theorem
  (arXiv:1112.0098; Charles Loewner, Wikipedia).
- J. B. Keller, "Geometrical Theory of Diffraction," *J. Opt. Soc. Amer.* 52:116–130 (1962).
- Stanford Encyclopedia of Philosophy, "Information Processing and Thermodynamic
  Entropy" (`plato.stanford.edu`).
- Stanford Dept. of Mathematics FARS colloquium on black-hole thermodynamics;
  Physics 170 syllabus (`syllabus.stanford.edu`).

### Equation-canon attributions

The canon's names, symbolic forms, discoverers, and years follow the standard
history-of-science record. Representative anchors: Newton, *Principia* (1687);
Euler, *Introductio in analysin infinitorum* (1748); Maxwell, "A Dynamical
Theory of the Electromagnetic Field" (1865); Einstein, *Annalen der Physik*
(1905) and the field equations (1915); Planck (1900); Schrödinger (1926);
Heisenberg (1927); Dirac (1928); Boltzmann (1877); Gibbs (1873–78); Clausius
(1850/1865); Carnot (1824); Black & Scholes, *J. Political Economy* (1973),
with Merton; Nernst (1889); Friedmann (1922); Shannon, *Bell System Tech. J.*
(1948); Landauer, *IBM J. Res. Dev.* (1961). Each is standard, citable history;
diffuse attributions are marked as such in the data.

*Content was rephrased from the above sources for compliance with licensing
restrictions.*

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
