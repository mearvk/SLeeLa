# SleelaUI™ Theming

SleelaUI is **configurable but ships Slick Black by default**. A theme is a flat
`SLUITheme` struct: a set of named role colours (0xRRGGBBAA words), one corner
radius, the 4px spacing unit, a minimum control height, and a font family/size.
Because every widget reads these roles — and nothing else — restyling the whole
UI is a single struct edit.

## Presets

```c
SLUITheme t;
slui_theme_preset(&t, SLUI_THEME_SLICK_BLACK);  /* the default, matte black  */
slui_theme_preset(&t, SLUI_THEME_GRAPHITE);     /* a lighter neutral palette */
```

Pass the theme in `SLUIWindowConfig.theme` at window creation, or swap it live:

```c
slui_window_set_theme(win, &t);   /* repaints immediately */
```

Passing `NULL` for `cfg.theme` uses Slick Black.

## Role colours

| Field | Meaning |
|---|---|
| `bg` | window floor (the matte black) |
| `surface` | panels, cards, entries |
| `surface_hi` | hovered surface / gradient top / slider trough |
| `chrome` | header-bar strip |
| `border` | 1px hairline separators and surface outlines |
| `fg` | primary text and glyphs |
| `fg_dim` | secondary / disabled text |
| `accent` | focus ring, suggested action, selection, toggle-on, slider fill |
| `accent_fg` | text/knob drawn on top of the accent |
| `danger` | destructive action / close-hover |
| `hover` | additive hover tint (small alpha, composited over the base) |
| `active` | additive pressed tint |

Metrics: `radius` (corner radius, px), `unit` (spacing unit, 4), and
`control_height` (minimum hit target; keep ≥ 32). Typography: `font_family`
(`"system"` resolves to the host's default UI face) and `font_size` (points).

## A custom theme

Start from a preset and override the roles you care about, or build one from
scratch and set `id = SLUI_THEME_CUSTOM`:

```c
SLUITheme t;
slui_theme_preset(&t, SLUI_THEME_SLICK_BLACK);
t.accent = slui_rgb(0x3d, 0xd6, 0x8c);   /* a green accent instead of blue  */
t.radius = 12;                           /* rounder corners                 */
strcpy(t.font_family, "Inter");
```

## Keeping within the principles

Any custom palette should still clear the [UI principles](UI-PRINCIPLES.md):
AA-contrast text on every surface, one accent per group, a visible focus ring,
and ≥ 32px hit targets. The presets are tuned to clear these; verify a custom
palette the same way.

— SleelaUI™ · MEARVK LLC
