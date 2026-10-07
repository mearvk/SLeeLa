# SleelaUI™ Layout & Flow Manager

A simple, nameable flow/layout manager over the widget tree. It treats a UI as
an **orthogonal conflation** of two independent concerns:

- **Unit parts** — *what* each item measures (a size/mass in one standardized
  unit vocabulary).
- **Organizations** — *how* those parts are arranged (named groups, each placed
  by a chosen flow).

You can re-measure an item without changing its organization, and re-organize
it without changing its measure. Everything is **nameable**: items have names,
groups have names, and you adjust a **single** item, a whole **group**, or an
**n-ary** selection (any subset of a group of some size).

## Standardized units (US + Eurasian + px)

One measurement vocabulary across US customary, Eurasian metric, and device
pixels, plus relative units. Each resolves to pixels at a DPI.

| Family | Units |
|---|---|
| **Device** | `px` |
| **Eurasian / metric** | `mm`, `cm`, `m` |
| **US customary** | `in`, `ft`, `pt` (1/72 in), `pica` |
| **Relative** | `%` (of the extent), `fr` (flexible fraction), `em` |

```c
slui_measure_to_px(slui_in(1.0),   96, 0, 16);   /* 96  (1 inch)   */
slui_measure_to_px(slui_mm_u(25.4),96, 0, 16);   /* 96  (metric)   */
slui_measure_to_px(slui_px(50),    96, 0, 16);   /* 50             */
slui_measure_to_px(slui_pct(50),   96, 200, 16); /* 100 (of 200)   */
SLUIMeasure m; slui_measure_parse("3mm", &m);     /* "3mm","0.5in","2fr"... */
```

An `fr` measure is a **flexible fraction** the flow distributes from leftover
space (like CSS `fr`); `slui_measure_is_flex` reports it.

## Flow solutions

More than one or two — pick the organization that fits:

| Flow | Arrangement |
|---|---|
| `SLUI_FLOW_STACK` | one row/column (the box model); `fr` items share leftover space |
| `SLUI_FLOW_WRAP` | a flow that wraps to new lines when the extent is exceeded |
| `SLUI_FLOW_GRID` | a fixed-column grid of equal cells |
| `SLUI_FLOW_DOCK` | the first/last items dock to the edges, the rest fills the centre |
| `SLUI_FLOW_CENTRAL` | **weight/mass** packing — heavier items migrate toward the centre of mass |

## Named ergonomics of placement

Placement is **Named** for the ergonomics of publics:

| Name | Meaning |
|---|---|
| `SLUI_PLACE_START` | leading edge |
| `SLUI_PLACE_MEDIUM` | **center** — *medium implies center* |
| `SLUI_PLACE_END` | trailing edge |
| `SLUI_PLACE_FILL` | stretch to fill |
| `SLUI_PLACE_CENTRAL` | **weight/mass** — *central implies weight/mass* (centre-of-mass pull) |

And named **ergonomic presets** tune a group for a common public use at once:
`READING`, `GALLERY`, `DASHBOARD`, `CONSOLE`, `PORTRAIT`, `TOOLBAR`.

## Using it (C)

```c
#include "sleela_ui_layout.h"

SLUILayout *lay = slui_layout_create();

/* a named group with a flow */
slui_layout_group(lay, "sidebar", SLUI_FLOW_STACK, SLUI_AXIS_VERTICAL);

/* named items; sizes in any standardized unit */
slui_layout_item(lay, "sidebar", "home",  home_widget,  slui_px(40));
slui_layout_item(lay, "sidebar", "files", files_widget, slui_mm_u(12));
slui_layout_item(lay, "sidebar", "body",  body_widget,  slui_fr(1)); /* flexible */

/* adjust a single item, a whole group, or an n-ary subset */
slui_item_set_size(lay, "home", slui_px(60));
slui_group_set_item_align(lay, "sidebar", SLUI_PLACE_FILL);
const char *pair[2] = { "home", "files" };
slui_nary_set_size(lay, pair, 2, slui_px(56));   /* just those two */

/* lay it out and read resolved rects to place the real widgets */
slui_layout_arrange(lay, "sidebar", (SLUIRect){0,0,200,600});
SLUIRect r; slui_layout_item_rect(lay, "body", &r);

/* a named ergonomic preset for a public use */
slui_layout_group(lay, "tiles", SLUI_FLOW_WRAP, SLUI_AXIS_HORIZONTAL);
slui_group_set_ergonomics(lay, "tiles", SLUI_ERGO_GALLERY);

/* CENTRAL = weight/mass: a heavier item pulls toward the centre */
slui_layout_group(lay, "stage", SLUI_FLOW_CENTRAL, SLUI_AXIS_HORIZONTAL);
slui_layout_item(lay, "stage", "hero", hero, slui_px(120));
slui_item_set_mass(lay, "hero", 8.0);

slui_layout_destroy(lay);
```

## Using it (SLeeLa)

```sleela
SLLayout lay = new SLLayout(); lay.create();
lay.group("sidebar", SLLayout.FLOW_STACK, SLLayout.AXIS_VERTICAL);

SLMeasure px40 = new SLMeasure(); px40.px(40.0);
SLMeasure flex = new SLMeasure(); flex.fraction(1.0);
lay.item("sidebar", "home", -1, px40);
lay.item("sidebar", "body", -1, flex);   // flexible: absorbs leftover space

// MEDIUM implies center; CENTRAL implies weight/mass.
lay.setAlign("sidebar", SLLayout.MEDIUM, SLLayout.FILL);
lay.setErgonomics("sidebar", SLLayout.ERGO_READING);

lay.arrange("sidebar", 0, 0, 200, 600);
int bodyY = lay.itemY("body");
int bodyH = lay.itemHeight("body");
```

## SLeeLa bindings

`SLMeasure` (the standardized units) and `SLLayout` (the manager: named groups,
named items, n-ary adjustment, the flows, named placement, and ergonomics)
under [`/lib/user-interface`](../lib/user-interface/USER-INTERFACE.md).

— SleelaUI™ · MEARVK LLC · 2026
