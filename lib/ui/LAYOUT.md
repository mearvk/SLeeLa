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
| `SLLayoutFrame` | **the organizing root** — owns a layout + named components; every component is placed in reference to the Frame | `f.useColumn(8,12); f.add("body",0); f.place(...)` |

## The Frame organizes (components in reference to the basic UI interface)

A **Frame** is the basic UI interface and the organizing root: it owns **one**
layout and a set of **named components**, and every component exists in excellent
reference to the Frame — its rectangle is computed by the Frame's layout and
measured from the Frame's own origin. Add components by name, place the Frame
once, and read each component's rectangle back by name.

```sleela
SLLayoutFrame f = new SLLayoutFrame(); f.create();
f.title("Settings");
f.useColumn(8, 12);          < the Frame's layout: a padded vertical stack >
f.add("header", 40);         < a 40px component >
f.add("body",   0);          < flexible: fills the rest >
f.add("footer", 32);
f.place(0, 0, 320, 600);     < the Frame's own rectangle; children arrange to it >

SLLayoutBox body = f.boxOf("body");   < that component's rect, relative to the Frame >
```

Choose the Frame's layout with `useColumn` / `useRow` / `useGrid`. The Frame is
where components live; the layout is how they are arranged within it.

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

## Driving real native widgets (`SLLayoutDriver`)

The Standard Layouts are the **planning** layer: pure SLeeLa that computes the
exact pixel rectangle of every component (`SLLayoutFrame` + the `SL*Layout`
family). `SLLayoutDriver` is the **realizing** layer: it takes a *placed*
`SLLayoutFrame` and configures live SleelaUI widgets through the `ui*` bridge so
the real, on-screen toolkit reproduces the plan.

### Why it is faithful, not lossy

SleelaUI — like GTK or Qt layouts — is a **container/pack** model: you declare
structure (a box with an orientation and spacing, or a grid of N columns) and
the toolkit solves geometry on every backend (X11 / Cocoa / Win32). There is no
"place this widget at absolute x,y" call, by design, so a view stays correct
when the window resizes. The Standard **stack** and **grid** layouts use that
same model, so realizing them is exact:

| Plan (`SLLayoutFrame`) | Realized native container |
| --- | --- |
| `useColumn(gap, pad)` | `uiBox(parent, 0 /*vertical*/, gap)` |
| `useRow(gap, pad)`    | `uiBox(parent, 1 /*horizontal*/, gap)` |
| `useGrid(cols, gap, pad)` | `uiGrid(parent, cols, gap)` |

Each real control is sized from the plan with `uiWidgetSizeRequest(w, h)`, and a
**flexible** component (added with size `0`) is marked `uiWidgetExpand` +
`uiWidgetAlign(FILL, FILL)` so it shares the leftover space exactly as the
planner computed.

### Usage

```sleela
< 1. PLAN in pure SLeeLa (always checks and runs, headless included) >
SLLayoutFrame f = new SLLayoutFrame(); f.create();
f.useColumn(8, 12);
f.add("header", 40);     < fixed 40px >
f.add("body",   0);      < flexible: fills the rest >
f.add("footer", 32);     < fixed 32px >
f.place(0, 0, 320, 600);

< 2. REALIZE onto a real window root (needs a display) >
SLLayoutDriver d = new SLLayoutDriver(); d.create();
int box = d.realize(root, f);         < one uiBox matching the plan >

int header = uiLabel(box, "Settings");
d.applyNamed(f, "header", header);    < sized from the plan >

int body = uiScrolledView(box, 1, 1);
d.applyNamed(f, "body", body);        < flexible -> expands to fill >

int footer = uiButton(box, "Close");
d.applyNamed(f, "footer", footer);
```

`SLLayoutDriver` is **native, bridge-backed** (it calls `ui*`), so unlike the
pure planners it runs only where a display/toolkit is present. The runnable
demo `lib/ui/layout-driver-demo.sleela` shows the full flow and degrades
gracefully on a headless host (it prints the computed plan and exits).

### Reading the plan from a realizer

`SLLayoutFrame` now exposes its plan so any realizer (or inspector) can read it:
`getLayoutKind()`, `isRowLayout()`, `getGap()`, `getColumns()`, and
`isFlexibleAt(index)`, alongside the existing `boxAt(index)` / `boxOf(name)`
rectangles. The underlying `SLStackLayout` gained `getGap()` and
`isFlexibleAt(index)`; `SLGridLayout` gained `getGap()`.

## Describing a component's quality (`SLComponentQuality`)

A layout gives every component its exact **size** and pixel-perfect
**placement** (`SLLayoutBox`: x/y/w/h). `SLComponentQuality` completes the
picture with each component's **quality**, so a component is fully described by
**quality + size + placement**.

Quality is expressed in the standard themes' own vocabulary, so it maps cleanly
to a flavor (blue/gray/black/white) and to what the native toolkit can honor per
widget:

- **role** — `asBody()` / `asSurface()` / `asChrome()` / `asAccent()`: which
  themed surface/text the component reads as.
- **emphasis** — `normal()` / `primary()` / `danger()`: `primary` is the single
  suggested action, `danger` is destructive.
- **interactivity** — `enable()` / `disable()`.

Every component added to an `SLLayoutFrame` gets a default quality
(`body / normal / enabled`) you can customize by name:

```sleela
SLLayoutFrame f = new SLLayoutFrame(); f.create();
f.useColumn(8, 12);
f.add("title", 40);
f.add("list", 0);
f.add("save", 36);
f.place(0, 0, 320, 600);

f.qualityOf("title").asChrome();
f.qualityOf("list").asSurface();
f.qualityOf("save").asAccent(); f.qualityOf("save").primary();

< now fully described: quality + size + placement >
< e.g. "save" -> quality:accent/primary/enabled + [12,508 296x36] >
```

When realized by `SLLayoutDriver`, the honorable parts are applied to the live
widget (`primary` -> `uiWidgetSuggested`, `danger` -> `uiWidgetDestructive`,
`disable()` -> `uiWidgetSensitive`); the role is a hint the themed renderer uses
when drawing the surface (the toolkit has no per-widget surface call, so role is
honored through the window theme).

`lib/ui/verify-pixel-perfect.sleela` is a runnable self-check (37 assertions)
proving exact placement/size (column, row, grid) and the quality descriptor.
