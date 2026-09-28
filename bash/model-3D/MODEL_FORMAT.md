# PHRAIGN-CITY model format

> Part of **SleelaTerminal™** (Phraign™ City 3D), SLeeLa's terminal product,
> built on GNU Bash (GPL, © Free Software Foundation). The `PHRAIGN-CITY`
> keyword is the on-disk model tag and is left unchanged for compatibility.

City 3D serializes a city to a small, diff-friendly, line-oriented text format
called **PHRAIGN-CITY**. It is deliberately plain ASCII so it lives well on
GitHub or a public server and produces readable diffs.

The current format is **version 4** (adds the general cityscape-graph
dimensions: radix / diameter / randomness, alongside the Year / IQ /
Legislature drivers). Versions 3, 2, and 1 are still read for
back-compatibility.

## Layout (v4, current)

```text
PHRAIGN-CITY 4
user <name>
seed <u64>
year <u32>
iq <u32>
legislature <name>
radix <u32>
diameter <double>
randomness <double 0..1>
finality <city finality, 0..1>
grid <cols> <rows>
cell    0 <c0> <c1> ... <c(cols-1)>
row     0 <h0> <h1> ... <h(cols-1)>
floors  0 <f0> <f1> ... <f(cols-1)>
windows 0 <w0> <w1> ... <w(cols-1)>
fin     0 <q0> <q1> ... <q(cols-1)>
... (the five per-row lines repeat for each row y) ...
END
```

- **`PHRAIGN-CITY 4`** — magic + format version. A reader requires the magic and
  a version `>= 1`.
- **`user`** — the per-user identity the model was generated for.
- **`seed`** — the 64-bit seed. `(user, seed, year, iq, legislature, radix,
  diameter, randomness)` determines the city and its cityscape graph.
- **`year`** — the city's Year, which drives modernity.
- **`iq`** — the person's IQ, which drives design quality.
- **`legislature`** — the native regime shaping the lines: `federal`,
  `parliamentary`, `municipal`, `bicameral`, `unicameral`, or `direct`.
- **`radix`** — the cityscape-graph branching base.
- **`diameter`** — the radial reach from the center of centricity (blocks).
- **`randomness`** — seeded graph/driver variability (`0..1`).
- **`finality`** — the city-wide quality-of-condition mean, `0..1`
  (informational; recomputed on regeneration).
- **`grid <cols> <rows>`** — grid dimensions. The default city is `64 64`
  (4096 blocks, "~4000 square blocks").

Then, for each grid row `y`, five lines:

- **`cell <y> ...`** — the cell kind per column: `0` = building, `1` = road,
  `2` = bridge.
- **`row <y> ...`** — building **heights** in world units (`0` = empty lot).
- **`floors <y> ...`** — number of floors per building.
- **`windows <y> ...`** — window count (a detailing cue) per building.
- **`fin <y> ...`** — per-building finality as an integer `0..1000`
  (i.e. finality × 1000).

- **`END`** — terminates the model.

The cityscape graph (nodes, mating cylinder pairs, and the spheres they grace)
is **not** dumped node-by-node: it is regenerated deterministically from the
seed and the fields above.

## Layout (v3 / v2 / v1, legacy — still read)

**v3** omits the `radix`/`diameter`/`randomness` lines (graph dimensions fall
back to defaults). **v2** further omits `iq` and `legislature`. **v1** is
height-only:

```text
PHRAIGN-CITY 1
user <name>
seed <u64>
grid <cols> <rows>
row <y> <h0> <h1> ... <h(cols-1)>
...
END
```

A v1 model carries heights only; the other attributes default to zero when read.
A v2 model omits IQ and legislature, which fall back to their defaults.

## Compatibility

Reading is forward-tolerant: lines whose leading token is unknown are ignored,
so future additive fields do not break older readers. Writers keep the
magic/version line first. Values are clamped on read (heights/floors/windows to
`0..65535`, cell to `0..2`, `fin` to `0..1000`).

## Example (v4)

```text
PHRAIGN-CITY 4
user demo
seed 10862587209389482623
year 2807
iq 165
legislature unicameral
radix 3
diameter 24
randomness 0.35
finality 0.601236
grid 64 64
cell    0 1 0 0 0 0 0 0 1 ...
row     0 0 18 21 17 ...
floors  0 0 12 14 11 ...
windows 0 0 84 98 77 ...
fin     0 0 612 655 588 ...
...
END
```

Values per row are separated by single spaces.
