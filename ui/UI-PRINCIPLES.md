# SleelaUI™ UI Principles

A short, enforceable set of rules for the look and feel of the SleelaUI™
toolkit. The goal is a **clean, calm, legible** interface where content is the
hero and the chrome is a quiet frame — the same discipline the SleelaTerminal™
GUI follows, applied to a general widget set. These describe *presentation
intent*; they do not change the widget API or behaviour.

## 1. Content is the hero

Widgets serve the content. Chrome — header bars, separators, borders — is a
quiet frame, never attention-grabbing. No blinking, flashing, or gratuitous
motion. Controls read as controls; labels read as labels.

## 2. One palette, one source of truth

All colours come from a single named palette: the `SLUITheme` struct, resolved
once per window. No raw colours are scattered through individual widgets.
Changing the theme means editing one struct (or loading one `.conf`). The
default palette is **Slick Black**:

| Role | Slick Black | Use |
|---|---|---|
| `bg` | `#0d0d0f` | window floor (matte near-black) |
| `surface` | `#161619` | panels, cards, entries |
| `surface_hi` | `#202125` | hovered surface / gradient top |
| `chrome` | `#101013` | header-bar strip |
| `border` | `#2a2b30` | 1px hairline separators |
| `fg` | `#ffffff` | primary text / glyphs |
| `fg_dim` | white @ .70 | secondary / disabled text |
| `accent` | `#5e9cff` | focus ring, suggested action, selection |
| `danger` | `#e54b4b` | destructive action / close-hover |

The window floor is a hair above pure black so the rasterizer's antialiasing
has somewhere to blend; surfaces step up in small, even increments so panels
read without hard borders.

## 3. Restraint over effects

- At most a **1px** hairline border for separation. No stacked shadows, insets,
  or glows. Legibility comes from contrast, not halos.
- Hover/active states are a single small additive tint (≈`.07` → `.14`),
  nothing more.
- Corners use one radius (`theme.radius`, default 8px) everywhere.

## 4. Consistent spacing

Use the 4px scale: **4, 8, 12, 16, 20**. `theme.unit` is 4. Container padding is
`12`–`20`; related controls are spaced `8`; section gaps `12`. Do not invent
one-off margins.

## 5. Type hierarchy, minimal weights

- One UI font family (`theme.font_family`, resolved to the host's system face).
- Secondary/explanatory text uses `fg_dim`, never a new colour.
- Avoid letter-spacing tricks.

## 6. Clear, honest affordances

- Clickable things look clickable (a surface tint + hover step); non-interactive
  labels do not.
- Exactly **one** `suggested` (accent) action per group — never two competing
  primaries. Destructive actions use `danger` on hover.
- Focus is always visible: an `accent` focus ring on keyboard focus.

## 7. Accessibility baseline

- Body/control text clears **WCAG AA** (≈4.5:1) on its background. White on
  `#0d0d0f` is ~19:1; dim text is reserved for non-essential notes.
- Nothing is conveyed by colour alone.
- Hit targets are at least **32px** tall (`theme.control_height` default 34).
- Every control is reachable and operable by keyboard (Tab / Shift-Tab, Enter /
  Space, arrow keys for sliders).

## 8. Identical on every platform

The look is defined by the portable core and the software rasterizer, so it is
pixel-identical on Windows, macOS, and Linux/Unix. A backend never styles
anything — it only shows the pixels the core produced and reports input. This
is the deliberate difference from toolkits that inherit three native themes.

## 9. Maintenance rule

When the look changes, update the palette (the preset in
[`src/slui_theme.cpp`](src/slui_theme.cpp)) — not individual widgets — keep this
document in sync, and run `make smoke` to confirm the core still builds and
renders. If a change adds motion, a new colour, or a new spacing value, it must
either fit these principles or update them deliberately.

— SleelaUI™ · MEARVK LLC
