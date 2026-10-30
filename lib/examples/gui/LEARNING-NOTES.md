# GUI examples — what to learn here

These examples teach the **arithmetic of layout** — how a GUI decides the pixel
rectangle of every widget before anything is drawn. They are calculation-only:
**no window is opened**, so they run anywhere and print exact coordinates. This
is the same math the standard UI library performs in `lib/ui` (SLStackLayout,
SLGridLayout, SLLayoutBox).

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-clamp-to-parent.sleela`](BASIC-clamp-to-parent.sleela) | The fundamental rule: a child is clamped to its parent's size. |
| Moderate | [`MODERATE-stack-layout.sleela`](MODERATE-stack-layout.sleela) | A horizontal stack: fixed children keep their width, a flexible child absorbs the leftover space. |
| Advanced | [`ADVANCED-grid-layout.sleela`](ADVANCED-grid-layout.sleela) | A uniform grid: divide the area into equal cells separated by gutters, and compute each cell's rectangle. |

The original [`CLASSIC-layout-bounds.sleela`](CLASSIC-layout-bounds.sleela) is
the shortest starter.

## What we want you to take away

1. **Layout is solved before it is painted.** A layout engine assigns every
   widget an exact `(x, y, width, height)`. If you can compute those numbers, you
   understand the layout — the drawing step just fills in the rectangles.
2. **Flexible space is "what's left over."** The moderate example gives two
   children fixed widths and lets the middle one take the remainder after
   subtracting the fixed widths and the gaps. The built-in check — the last
   child's right edge equals the row width (400) — is how you prove the
   distribution is exact with no gap or overlap.
3. **A grid is repeated subtraction and multiplication.** Cell size is
   `(area − gutters) / count`; each cell's origin is `index * (cell + gutter)`.
   The advanced example walks rows and columns to emit a pixel-perfect lattice,
   exactly as `SLGridLayout` does.

## Practical notes for SLeeLa 1.7

- **Integer pixels.** Layout uses `int` arithmetic; integer division means cell
  sizes truncate (the 300-wide, 3-column, 10-gutter grid yields 93-pixel cells).
  A real engine distributes the remainder; these examples keep it simple to show
  the core formula.
- **No GUI backend here.** Opening a real window and placing native widgets is a
  separate native-bridge step the GUI subsystem documents. These files stop at
  the numbers on purpose, which is why they are deterministic.
- For the relationship between layout (size + placement) and *quality* (role,
  emphasis, interactivity), see the standard UI system under `lib/ui`.

## Run them

```sh
sleela run lib/examples/gui/MODERATE-stack-layout.sleela
sleela run lib/examples/gui/ADVANCED-grid-layout.sleela
```

Output is deterministic: the stack layout's right edge lands exactly at the row
width (400), and the grid prints six 93×95 cells on a 3×2 lattice.
