# /5 · Math model — integers and line lengths

The **math model** here reads *a Line as mathematics defines it*: a whole
**integer count**, or the **exact length** of a shape's edge. Where a physical
length is measured and a social count is surveyed, a mathematical Line is
**EXACT** — an integer is defined, and a length formula is fixed with no
uncertainty and no date.

## Data — integers and common counts

[`../data/integers.json`](../data/integers.json) — canonical integers, including
the common counts a society reaches for.

| Name | Value | Kind | Note |
|---|---:|---|---|
| dozen | 12 | EXACT | a common count |
| baker's dozen | 13 | EXACT | a common count |
| score | 20 | EXACT | a common count |
| gross | 144 | EXACT | a dozen dozen |
| great gross | 1,728 | EXACT | a dozen gross (12³) |
| first perfect number | 6 | EXACT | 6 = 1+2+3 (OEIS A000396) |
| second perfect number | 28 | EXACT | 28 = 1+2+4+7+14 |
| largest 2-digit prime | 97 | EXACT | OEIS A000040 |
| Mersenne prime 2⁷−1 | 127 | EXACT | OEIS A000668 |
| byte range count | 256 | EXACT | 2⁸ values in one byte |
| days in a common year | 365 | EXACT | non-leap year |
| degrees in a turn | 360 | EXACT | by convention |

## Data — line & perimeter formulas

[`../data/geometry-lengths.json`](../data/geometry-lengths.json) — the 1D
formulas, each with a worked value (π = 3.14159265358979, √2 = 1.41421356237).

| Shape | Formula | Worked value | Kind |
|---|---|---:|---|
| Segment | b − a | 5.0 (a=0, b=5) | EXACT |
| Square perimeter | 4·s | 12.0 (s=3) | EXACT |
| Rectangle perimeter | 2·(w+h) | 14.0 (3,4) | EXACT |
| Circle circumference | 2·π·r | 12.566370614 (r=2) | EXACT |
| Unit-square diagonal | √2·s | 1.41421356237 (s=1) | EXACT |
| 3-4-5 hypotenuse | √(a²+b²) | 5.0 (3,4) | EXACT |
| Regular hexagon perimeter | 6·s | 12.0 (s=2) | EXACT |

## The distinction the model keeps

Every mathematical Line is **EXACT**. An integer *is* its count; a length *is*
its formula. This is the reference face of a Line — the same 1D reasoning the
`/4` (Areas) model squares into 2D. A mathematical Line carries no provenance
beyond its definition; it follows from arithmetic and Euclid.

Each Line also carries a **Spark** (see [`../SPARKS.md`](../SPARKS.md)) — e.g.
*"6 = 1+2+3: a whole equal to the sum of its own parts is rare and worth
noticing."* The value is EXACT; the Spark is a guide, not a measurement.

## Provenance

Public-domain mathematics; integer sequence identifiers from the OEIS
(oeis.org); geometry from any standard reference. Worked values are computed from
the stated parameters, so they can be checked line by line.
