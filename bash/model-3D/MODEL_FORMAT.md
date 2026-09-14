# PHRAIGN-CITY model format

City 3D serializes a city to a small, diff-friendly, line-oriented text format
called **PHRAIGN-CITY**. It is deliberately plain ASCII so it lives well on
GitHub or a public server and produces readable diffs.

## Layout

```text
PHRAIGN-CITY 1
user <name>
seed <u64>
grid <cols> <rows>
row 0 <h0> <h1> ... <h(cols-1)>
row 1 <h0> <h1> ... <h(cols-1)>
...
row <rows-1> <h0> <h1> ... <h(cols-1)>
END
```

- **`PHRAIGN-CITY 1`** — magic + format version. A reader requires the magic and
  a version `>= 1`.
- **`user`** — the per-user identity the model was generated for.
- **`seed`** — the 64-bit seed used to generate the model. `(user, seed, grid)`
  fully determines the city, so the model can be regenerated exactly.
- **`grid <cols> <rows>`** — grid dimensions. The default city is `64 64`
  (4096 blocks, "~4000 square blocks").
- **`row <y> ...`** — one line per grid row `y`, listing `cols` integer building
  **heights** (world units). A height of `0` is an empty lot / street.
- **`END`** — terminates the model.

## Compatibility

Reading is forward-tolerant: lines whose leading token is unknown are ignored,
so future additive fields do not break older readers. Writers should keep the
magic/version line first.

## Example

```text
PHRAIGN-CITY 1
user demo
seed 10862587209389482623
grid 64 64
row 0 0 0 0 0 0 0 0 2 0 ...
row 1 0 5 6 5 3 4 8 5 0 ...
...
END
```

Heights per row are separated by single spaces. Values are clamped to
`0..65535` on read.
