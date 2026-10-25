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
| `SMArchive` (demo) | Runnable entry point that broadcasts the archive | — |

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

*Content was rephrased from the above sources for compliance with licensing
restrictions.*
