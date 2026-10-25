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
| `SMProvenance`     | Purpose/method/gains/bedrock + tiered literature callouts (Wikipedia attributed) | Primary/secondary/tertiary sources |
| `SMArchive` (demo) | Runnable entry point that broadcasts the archive | — |

## The equation canon (102 entries)

> On "all of the equations known in the known Universe": **no finite, complete
> list of every equation exists** — and fabricating one would violate this
> repo's one hard rule (values faithful to real origin; unknowns are `"none"`,
> never invented). So this archive records a **curated, fully-attributed canon**
> of the genuinely foundational equations that every math/physics PhD rests on:
> **102 entries**, each with its real name, symbolic form, discoverer(s), year,
> and field.

Breakdown (counts verified by running `SMArchive`):

- **Mathematics — 34:** Pythagoras, Euler's identity/formula, FTC, binomial
  theorem, Taylor & Fourier series, Cauchy–Schwarz, Gauss/Green/Stokes,
  Cauchy integral & residue theorems, Riemann zeta functional equation, Euler
  product, prime number theorem, Bayes, CLT, Stirling, Gamma, Basel problem,
  Euler–Mascheroni, golden ratio, polyhedron formula, eigenvalue equation,
  Green–Tao, and more.
- **Physics — 36:** Newton's laws & gravitation, Lagrange/Hamilton, Coulomb,
  all four Maxwell equations, Lorentz force, `E=mc^2`, Einstein field equations,
  Schwarzschild radius, Planck relation & law, de Broglie, both Schrödinger
  equations, Heisenberg uncertainty, Dirac, canonical commutation, Bohr levels,
  Rydberg, photoelectric, Navier–Stokes, wave equation, Bernoulli.
- **Thermodynamics & statistical mechanics — 20:** the four laws, Clausius
  entropy, Boltzmann `S=k ln W`, Gibbs entropy, ideal gas law, Gibbs/Helmholtz
  free energies, enthalpy, Carnot efficiency, Maxwell–Boltzmann, Boltzmann
  factor, partition function, Stefan–Boltzmann, Clausius–Clapeyron,
  Gibbs–Helmholtz, Sackur–Tetrode, Landauer's principle.
- **Crossovers — 12:** Black–Scholes (PDE & call price), Itô's lemma, Nernst,
  Arrhenius, Friedmann, Hubble–Lemaître, Drake, Shannon entropy & channel
  capacity, logistic equation, heat equation.

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
  < ---- Scientific record (purpose / method / gains / bedrock) ---- >
  String metaPurpose           = "...";   < what this record is for >
  String metaScientificMethod  = "...";   < the method it follows >
  String metaAuthorialGains    = "...";   < what its authors legitimately gain >
  String metaBedrock           = "...";   < the foundation the method rests on >
```

| Facet | What it declares |
|-------|------------------|
| **Purpose** | The program's reason to exist: *carry, not create* — an honest index of published equations and rightful credit. |
| **Perceived scientific method** | **Tertiary cataloguing** — record claims traceable to primary/secondary literature; originate nothing. |
| **Authorial gains** | Editorial/curatorial credit for the *catalogue only*. The theorems and laws are public-domain science owned by no one; cited crossover results belong to their authors/publishers. The archive claims no authorship over the works it cites. |
| **Bedrock of the method** | The peer-reviewed and historical scientific record — primary sources where citable, tertiary indexes to locate them. |

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
