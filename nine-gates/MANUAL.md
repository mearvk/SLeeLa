# MANUAL.md — the Nine Gates set

A short guide to the **Nine Gates**: what it is, how the pieces fit together, how to view and
render the emblems, and how to extend the set. Everything here is **original work authored for
this repository** (see [Provenance](#5-provenance-and-licensing)).

---

## 1. What this is

The Nine Gates is a small, self-contained set of **nine original emblems** with a short written
description for each. It is a thematic/artwork asset for the project — a sequence of nine
"gates," each a distinct symbol and idea, meant to be used as section markers, milestone badges,
step indicators, or simply as a reference motif.

It is **inspired by the generic idea** of a nine-stage passage (a motif found across many myths,
games, and stories). It is **not** a copy or adaptation of any specific book, film, or other
copyrighted artwork, and it deliberately does not reproduce such material. The specific fictional
work the directory name alludes to is documented, as description and attribution only, in
[`REFERENCE.md`](./REFERENCE.md).

---

## 2. Layout

```text
nine-gates/
  README.md            index + the gate table
  MANUAL.md            this guide
  gates/
    gate-01.md         The Threshold
    gate-02.md         The Lantern
    gate-03.md         The Bridge
    gate-04.md         The Mirror
    gate-05.md         The Keystone
    gate-06.md         The Tide
    gate-07.md         The Ember
    gate-08.md         The Compass
    gate-09.md         The Horizon
  images/
    gate-01.svg .. gate-09.svg        one emblem per gate
    nine-gates-contact-sheet.svg      all nine on one sheet
```

**Naming convention (deliberate, so the set stays sortable and greppable):**

- Documents: `gates/gate-0N.md` — zero-padded two-digit index, one per gate, matching the emblem.
- Emblems: `images/gate-0N.svg` — same index, so `gate-03.md` pairs with `gate-03.svg`.
- The combined sheet: `images/nine-gates-contact-sheet.svg`.

The zero-padding (`01`..`09`) keeps lexical order equal to numeric order in every file browser and
`ls`, and makes a tenth-or-later gate (`10`) sort correctly if the set is ever extended.

---

## 3. The nine gates at a glance

| # | Gate | One-line idea |
|---|------|---------------|
| 1 | The Threshold | the decision to begin; crossing in |
| 2 | The Lantern | sight in the dark; the first guidance |
| 3 | The Bridge | spanning the gap; commitment over a void |
| 4 | The Mirror | self-knowledge; seeing what you brought |
| 5 | The Keystone | the load-bearing center; what holds it together |
| 6 | The Tide | forces beyond control; timing and patience |
| 7 | The Ember | what survives; the persistent spark |
| 8 | The Compass | direction regained; choosing a true course |
| 9 | The Horizon | arrival and the next beginning; the open end |

Each gate's own document (`gates/gate-0N.md`) gives its name, the emblem's visual motif, and a
described meaning.

---

## 4. Viewing and rendering the emblems

The emblems are **hand-written SVG** (plain vector line-art, no external fonts or images), so they
are portable and diffable.

**View one emblem:**
- Open `images/gate-01.svg` in any web browser, or
- Embed in Markdown/HTML: `![The Threshold](images/gate-01.svg)`

**View all nine:** open [`images/nine-gates-contact-sheet.svg`](./images/nine-gates-contact-sheet.svg).

**Convert to PNG** (optional, if you have one of these installed):

```sh
# with rsvg-convert (librsvg)
rsvg-convert -w 512 -h 512 images/gate-01.svg -o gate-01.png

# or with ImageMagick
convert -background none -resize 512x512 images/gate-01.svg gate-01.png

# or with Inkscape
inkscape images/gate-01.svg --export-type=png -w 512 -o gate-01.png
```

Each emblem is drawn on a `0 0 240 240` viewBox with a transparent background, so it scales to any
size and recolors cleanly (the strokes use `currentColor` where a host stylesheet is present, with
a dark default otherwise).

---

## 5. Provenance and licensing

- **Original.** All nine emblems and descriptions were authored for this repository.
- **Not derived from any copyrighted source.** This is not a reproduction, tracing, recreation, or
  adaptation of any film, novel, game, or other protected artwork. Thematic resemblance is limited
  to generic, non-copyrightable ideas (the number nine; a gate/passage motif).
- **Self-contained.** SVGs use only inline paths/shapes — no external fonts, images, or scripts.
- **Re-licensable with the repo.** Being plain text SVG + Markdown, the whole set carries the
  repository's license and is easy to review in a diff.

---

## 6. Extending the set

To add or revise a gate:

1. Keep the paired naming: a new `gates/gate-0N.md` **and** `images/gate-0N.svg` with the same `N`.
2. Author original art only — no copying from protected sources.
3. Add the row to the tables in [`README.md`](./README.md) and §3 above.
4. If you add a tenth gate, the naming already sorts correctly (`gate-10.*`), but update the
   "Nine" framing in the docs so the name stays truthful to the count.
