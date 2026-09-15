# /4 · Math model — planar Areas

The **math model** here reads *area as mathematics defines it*: the exact rule
by which a shape spans a two-dimensional extent. Where a geographic area is
surveyed, a planar area is **EXACT** — it is fixed by a formula, with no
uncertainty and no date.

## Data

[`../data/geometry-areas.json`](../data/geometry-areas.json) — the area formulas,
each with a worked value for given parameters (π = 3.14159265358979).

| Shape | Formula | Worked value | Kind |
|---|---|---|---|
| Circle | π·r² | 12.566370614 (r=2) | EXACT |
| Square | s² | 9 (s=3) | EXACT |
| Rectangle | w·h | 12 (3×4) | EXACT |
| Triangle | ½·b·h | 12 (b=6, h=4) | EXACT |
| Ellipse | π·a·b | 18.849555922 (a=3, b=2) | EXACT |
| Regular hexagon | (3√3/2)·s² | 10.392304845 (s=2) | EXACT |
| Sphere surface | 4·π·r² | 12.566370614 (r=1) | EXACT |

## The distinction the model keeps

Every planar area is **EXACT**: the shape *is* its formula, and the worked value
is an exact consequence of the parameters (truncated only in its decimal
display). This is the reference face of Area — the same `π·r²` that lets the
physics model compute the Earth's surface as `4πR²`. A geometric area carries no
provenance beyond its definition; it simply follows from Euclid.

## Provenance

Standard Euclidean geometry (public domain); the formulas appear in any
mathematics reference. Worked values are computed from the stated parameters, so
they can be checked line by line.
