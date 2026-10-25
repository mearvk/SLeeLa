# The Standard UI Layout family (`lib/ui`)

A small, consistent, **professional set of layout classes designed for people of
all skill levels**. The canonical definitions live here in `lib/ui` and are
usable from anywhere in `/lib` (including `lib/user-interface` and `lib/gui`) —
class names are global in `/lib`, so they are defined in exactly one place to
stay unambiguous.

Every class is **pure SLeeLa** (no native handle required), computes **real
device-pixel rectangles** into `SLLayoutBox[]`, carries the Metadata Class
metapattern, and checks + runs on the stock compiler.

## The family

| Class | What it does | Easiest call |
|---|---|---|
| `SLLayoutBox` | one computed rectangle (x, y, w, h) — the shared output unit | `b.describe()` |
| `SLStackLayout` | a single row or column (the 90% case); `add(0)` = flexible | `s.column(); s.add(40); s.add(0); s.arrange(...)` |
| `SLGridLayout` | an evenly divided rows × cols grid | `g.columns(3); g.add(); g.arrange(...)` |
| `SLDockLayout` | dock to edges (top/bottom/left/right) + one `fill()` centre | `d.top(32); d.fill(); d.arrange(...)` |
| `SLAbsoluteLayout` | pixel-exact x/y/w/h placement | `a.add(10,10,120,24); a.arrange(...)` |
| `SLLayoutKit` | **beginner-first facade** — one call returns the right layout | `kit.column(8,12)` / `kit.grid(3,10,10)` / `kit.dock()` |

## For beginners (start here)

```sleela
SLLayoutKit kit = new SLLayoutKit(); kit.create();

< a vertical page: a 40px header, then a body that fills the rest >
SLStackLayout page = kit.column(8, 12);   < gap 8, padding 12 >
page.add(40);        < header, fixed >
page.add(0);         < body, flexible (0 = share the leftover space) >
page.arrange(0, 0, 360, 640);

SLLayoutBox header = page.boxOf(0);
SLLayoutBox body   = page.boxOf(1);
```

## For professionals

Use the concrete classes directly for full control (grid columns, dock order,
exact rects). Every layout validates its inputs (clamps negatives, bounds gaps)
and exposes `honored()` for a consistency re-check. Sizes flow in device pixels;
pair with `SLMeasure` (unit conversion) when you need physical/relative units.

## Relationship to the other layout surfaces

- **`SLMeasure` / the native `SLLayout` flow manager** (`lib/user-interface`,
  backed by `user-interface/slui_layout.cpp`) is the *expert, native-backed*
  engine with named groups, ergonomic presets, and flexible `fr` units. The
  standard family here is the *pure-SLeeLa, beginner-first* layer; use whichever
  fits the task.
- **`SLGuiLayout`** (`lib/gui`) is the GUI-node geometry *contract* read by the
  reconciler; it is a per-widget style, not a multi-child solver.

The standard family is the recommended default for everyday placement.
