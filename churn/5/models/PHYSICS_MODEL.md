# /5 · Physics model — physical lengths

The **physics model** here reads *a Line as a real distance*: a length, in
metres, that reaches across space. Where the mathematical Lines are **EXACT**
(defined by an integer or a formula), a physical Line is **EXACT** only when it
is *defined* (the metre, the astronomical unit); otherwise it is **MEASURED**
(an experimental length with an uncertainty) or **OBSERVED** (a surveyed extent).

## Data

[`../data/physical-lengths.xml`](../data/physical-lengths.xml) — physical lengths
in metres, each with its kind and a provenance note.

| Name | Value (m) | Kind | Note |
|---|---:|---|---|
| metre (SI base unit) | 1 | EXACT | defined via the speed of light and the second |
| light travels in 1 second | 299,792,458 | EXACT | c is exact by definition |
| Bohr radius | 5.29177210903×10⁻¹¹ | MEASURED | CODATA atomic length scale |
| human hair (typical width) | 7.5×10⁻⁵ | OBSERVED | ~50–100 µm |
| Earth mean radius | 6,371,000 | OBSERVED | IUGG mean radius |
| Earth equatorial circumference | 40,075,017 | OBSERVED | WGS-84 |
| Earth–Moon mean distance | 384,400,000 | OBSERVED | NASA mean centre-to-centre |
| astronomical unit | 149,597,870,700 | EXACT | IAU 2012 exact definition |

## The distinction the model keeps

A physical Line separates the **defined** from the **found**. The metre, the
light-second, and the astronomical unit are **EXACT** — humanity *defined* them,
so they carry no uncertainty. The Bohr radius is **MEASURED** — nature sets it
and we approach it with a stated uncertainty. Earth's radius and the Earth–Moon
distance are **OBSERVED** — surveyed extents for a stated model of the world.
The order-of-magnitude span here is enormous (a hair's width to the astronomical
unit), which is exactly why `lines.sleela` reports each Line's order of
magnitude alongside its value.

Each Line carries a **Spark** (see [`../SPARKS.md`](../SPARKS.md)) — e.g. *"a
unit is a line everyone agrees to share; agreement is what makes measurement
possible."* The value is EXACT/MEASURED/OBSERVED; the Spark is a guide, not
evidence.

## Provenance

Defined lengths: BIPM SI Brochure (2019) and IAU (2012). Measured lengths:
CODATA / NIST. Astronomical distances: NASA / IAU. Retrieved 2026-09. Values in
metres; scientific-notation figures are shown in the data file's XML text (the
`lines.sleela` model uses plain-decimal Lines so it runs on the current lexer).
