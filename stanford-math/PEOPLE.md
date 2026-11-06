# Named Stanford Mathematicians

> 9 people, sourced from `SMPersonRegistry.sleela` -- the single source of truth.
> This document is a human-readable mirror of that record; if the two ever
> disagree, the `.sleela` registry wins and this file is stale.

Historical and current figures publicly associated with Stanford mathematics,
drawn from Stanford's own faculty/emeriti rosters and bulletins. Each row records
the person under the name the public record uses, their years, field, standing at
Stanford, and the contribution they are best known for -- plus a one-line note on
why that contribution matters.

The one hard rule governs every row: **values are faithful to the subject's real
origin where publicly known**; a lifespan still open is written `YYYY-unknown`,
never guessed. No name is added for prestige and none that belongs is omitted.

## The 9 mathematicians

| # | Name | Years | Field | Standing at Stanford | Best known for | Why it matters |
|---|------|-------|-------|----------------------|----------------|----------------|
| 1 | George Polya | 1887-1985 | analysis, probability, combinatorics | Professor of Mathematics (from 1942) | random-walk recurrence (1921); Polya enumeration theorem | Pinned down when a random walk returns home, and gave combinatorics its central counting tool; also shaped how mathematics is taught ("How to Solve It"). |
| 2 | Charles Loewner (Karl Loewner) | 1893-1968 | complex analysis, operator theory | Professor of Mathematics | Loewner's theorem on operator monotone functions (1934) | Characterized operator-monotone functions via analytic continuation; the Loewner equation later underpinned Schramm-Loewner evolution in modern probability. |
| 3 | Paul Cohen | 1934-2007 | logic, set theory, analysis | Professor of Mathematics | forcing; independence of the continuum hypothesis; Fields Medal 1966 | Invented forcing and proved the continuum hypothesis independent of ZFC -- the deepest result in set theory since Godel, and Stanford's only math Fields Medal. |
| 4 | Persi Diaconis | 1945-unknown | probability, statistics | Professor of Mathematics and Statistics | riffle-shuffle mixing -- 'seven shuffles suffice' (Bayer-Diaconis 1992) | Turned card shuffling into rigorous mixing-time theory, giving the famous result that seven riffle shuffles randomize a deck. |
| 5 | Samuel Karlin | 1924-2007 | applied probability, mathematical biology | Professor (emeritus) | total positivity; birth-death processes | Built the theory of total positivity and applied probability to genetics and molecular biology, bridging pure math and the life sciences. |
| 6 | Joseph B. Keller | 1923-2016 | applied mathematics | Professor (emeritus) | geometrical theory of diffraction (1962) | Extended geometrical optics with diffracted rays, giving engineers a practical way to predict wave behavior around edges -- a cornerstone of modern applied math. |
| 7 | Donald Ornstein | 1934-2023 | ergodic theory | Professor (emeritus) | the Ornstein isomorphism theorem for Bernoulli shifts | Proved when two random systems are the same up to relabeling, settling a central classification problem in ergodic theory. |
| 8 | Yakov Eliashberg | 1946-unknown | symplectic and contact topology | Professor of Mathematics | foundational results in symplectic/contact geometry | Co-founded modern symplectic and contact topology, revealing deep rigidity phenomena at the heart of geometry and mechanics. |
| 9 | Robert Osserman | 1926-2011 | differential geometry | Professor (emeritus) | minimal surfaces; mathematics outreach | Advanced the theory of minimal surfaces and became a leading public voice for mathematics, widening its reach beyond the academy. |

## Validate

These records are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints the full roster among the rest of the
archive):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Rosters are drawn from Stanford's own faculty/emeriti pages and registrar
bulletins; biographical facts follow the standard public record (see the repo
`README.md` Sources section and `SMProvenance.sleela`). The **Why it matters**
notes summarize each contribution's established significance. This archive records
and attributes publicly documented people and results; it does not originate, own,
or certify them, and it trades on no one's name.
