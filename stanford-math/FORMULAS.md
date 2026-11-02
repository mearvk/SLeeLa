# Named Formulas & Theorems (Stanford-anchored)

> 6 formulas, sourced from `SMFormulaCatalog.sleela` -- the single source of
> truth. This document is a human-readable mirror of that record; if the two
> ever disagree, the `.sleela` catalog wins and this file is stale.

Named formulas and theorems tied specifically to Stanford mathematicians -- a
great move recorded only where the public record truly ties it to a Stanford
name. Each row pairs the human statement with its canonical symbolic form and its
real citation, plus a one-line note on why it matters. The credit goes to the
person, never to the brand.

The one hard rule governs every row: **values are faithful to the subject's real
origin where publicly known**; nothing is invented and no result is claimed for
Stanford that the literature does not place there.

## The 6 named results

| # | Result | Form | Credit | Why it matters |
|---|--------|------|--------|----------------|
| 1 | Seven shuffles suffice (riffle-shuffle cutoff) | `t* = (3/2) * log2(N) riffle shuffles to mix N cards` | Bayer & Diaconis, 1992 | The most famous mixing-time result: it quantifies exactly how much shuffling randomizes a deck, and became the public face of rigorous Markov-chain mixing. |
| 2 | Polya recurrence theorem | `simple random walk on Z^d: recurrent for d in {1,2}, transient for d >= 3` | G. Polya, 1921 | Answers whether a random walker returns home -- yes in 1-2 dimensions, no in 3+ -- a foundational fact of probability ("a drunk man finds his way home, a drunk bird may not"). |
| 3 | Polya enumeration theorem | `orbit count = (1/|G|) * sum over g in G of m^c(g), with c(g) = cycles of g` | G. Polya, 1937 | The master tool for counting arrangements up to symmetry; indispensable in combinatorics, chemistry (counting isomers), and group theory. |
| 4 | Loewner's theorem (operator monotone functions) | `f is operator monotone on an interval iff f extends to a Pick (Nevanlinna) function on the upper half-plane` | K. Loewner, 1934 | Links a purely algebraic property (operator monotonicity) to complex analysis; central to matrix analysis and quantum information. |
| 5 | Geometrical theory of diffraction | `total field = geometrical-optics field + diffracted-ray field from edges/corners/grazing rays` | J. B. Keller, 1962 | Made it possible to predict how waves bend around edges and obstacles; a workhorse of radar, antenna, and acoustics engineering. |
| 6 | Cohen forcing (independence of CH) | `ZFC does not decide the continuum hypothesis: CH is independent of ZFC` | P. Cohen, 1963 | Proved that a famous question about the sizes of infinity simply cannot be settled by standard set theory; forcing became the defining technique of modern logic. |

## Validate

These records are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints the catalog among the rest of the
archive):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Citations name the Stanford author directly (Bayer-Diaconis 1992; Polya 1921,
1937; Loewner 1934; Keller 1962; Cohen 1963); see the repo `README.md` Sources
section and `SMProvenance.sleela` for the tiered source registry. The **Why it
matters** notes summarize each result's established significance. This archive
records and attributes published results; it does not originate, own, or certify
them, and it attributes each move to the person, not the brand.
