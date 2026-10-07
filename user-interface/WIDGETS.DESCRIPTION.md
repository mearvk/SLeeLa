# SleelaUI™ Widget Collection

The complete catalogue of widgets in **SleelaUI™** — SLeeLa's own, original
cross-platform UI toolkit (not GTK/Qt). Every widget is drawn by the toolkit's
own software rasterizer and reads only theme roles, so each is **pixel-identical
on Windows, macOS, and Linux/Unix** and obeys the project's
[UI principles](UI-PRINCIPLES.md): one palette, one accent per group, an
always-visible focus ring, WCAG AA contrast, and ≥32px hit targets. The default
look is **Slick Black** (see [THEMING.md](THEMING.md)).

Each row lists the native C ABI constructor (`include/sleela_ui.h`), the SLeeLa
class (`/lib/user-interface`), what it is, and whether it takes keyboard focus.
This table is the one-stop **find** for "which widget do I want, and what is it
called in C and in SLeeLa?"

## Containers & structure

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Box** | `slui_box` | `SLBox` | Vertical/horizontal box; GTK/CSS box layout (natural sizes, then expand, then align). | — |
| **Grid** | `slui_grid` | `SLGrid` | Fixed row/column grid; children fill cells row-major, cells sized to the largest child. | — |
| **Frame** | `slui_frame` | `SLFrame` | Titled, 1px-bordered group container with a vertical child stack. | — |
| **Card** | `slui_card` | `SLCard` | Raised rounded surface panel with a hairline and padding. | — |
| **Header Bar** | `slui_header_bar` | `SLHeaderBar` | Title-bar strip: chrome fill + bottom hairline; holds brand, spacer, actions. | — |
| **Status Bar** | `slui_status_bar` | `SLStatusBar` | Footer strip: chrome band + top hairline; horizontal child layout. | — |
| **Separator** | `slui_separator` | `SLSeparator` | 1px hairline rule, horizontal or vertical. | — |
| **Spacer** | `slui_spacer` | `SLSpacer` | Invisible flexible gap that absorbs surplus space along its box axis. | — |

## Buttons & actions

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Button** | `slui_button` | `SLButton` | Push button; `suggested` (accent) and `destructive` (danger) variants. | ✓ |
| **Link Button** | `slui_link_button` | `SLLinkButton` | Hyperlink-style accent text that underlines on hover/focus. | ✓ |
| **Toggle** | `slui_toggle` | `SLToggle` | On/off switch (pill track + sliding knob) with a trailing label. | ✓ |

## Selection & boolean

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Check Box** | `slui_check_box` | `SLCheckBox` | Labelled square with an accent tick when checked. | ✓ |
| **Radio Button** | `slui_radio_button` | `SLRadioButton` | Labelled circle with an accent dot; mutually exclusive within a group. | ✓ |
| **Combo Box** | `slui_combo_box` | `SLComboBox` | Drop-down showing the current option and a chevron; cycles options. | ✓ |
| **Spin Button** | `slui_spin_button` | `SLSpinButton` | Numeric value with − / + steppers, clamped to `[min,max]`. | ✓ |

## Text input

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Entry** | `slui_entry` | `SLEntry` | Single-line UTF-8 text field with caret, placeholder, and editing keys. | ✓ |
| **Search Entry** | `slui_search_entry` | `SLSearchEntry` | Entry with a leading magnifier glyph and a "Search" placeholder. | ✓ |
| **Password Entry** | `slui_password_entry` | `SLPasswordEntry` | Entry that renders bullet dots instead of its characters. | ✓ |

## Ranges & indicators

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Slider** | `slui_slider` | `SLSlider` | Horizontal slider over a `[min,max]` range with a draggable knob. | ✓ |
| **Scroll Bar** | `slui_scroll_bar` | `SLScrollBar` | Thin track with a draggable thumb covering a `page` fraction. | ✓ |
| **Progress Bar** | `slui_progress_bar` | `SLProgressBar` | Determinate accent fill proportional to a fraction in `[0,1]`. | — |
| **Level Bar** | `slui_level_bar` | `SLLevelBar` | Segmented level (battery/volume style) over `[0,1]`. | — |
| **Spinner** | `slui_spinner` | `SLSpinner` | Indeterminate activity: an accent arc riding a faint ring. | — |

## Text & display

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Label** | `slui_label` | `SLLabel` | Non-interactive run of body text; alignable within its cell. | — |
| **Heading** | `slui_heading` | `SLHeading` | Title text at a larger point size for sections/pages. | — |
| **Image** | `slui_image` | `SLImage` | Fixed-size rounded tile with a centred glyph (picture/icon placeholder). | — |
| **Avatar** | `slui_avatar` | `SLAvatar` | Round accent disc showing an initial, for a person/account. | — |

## Tags & feedback

| Widget | C ABI constructor | SLeeLa class | Description | Focus |
|---|---|---|---|---|
| **Badge** | `slui_badge` | `SLBadge` | Small accent count/status pill to annotate a control. | — |
| **Chip** | `slui_chip` | `SLChip` | Rounded surface pill with a hairline, for tags/filters/selections. | — |
| **Info Bar** | `slui_info_bar` | `SLInfoBar` | Inline tinted notice strip with a left severity bar (info/warning/error). | — |
| **Tooltip** | `slui_tooltip` | `SLTooltip` | Small raised bubble with short explanatory text. | — |

## Totals

- **31 widget types** across containers, actions, selection, input, ranges,
  display, and feedback.
- **Keyboard-focusable:** Button, Link Button, Toggle, Check Box, Radio Button,
  Combo Box, Spin Button, Entry, Search Entry, Password Entry, Slider, Scroll
  Bar (12 interactive controls, all operable with Tab / Shift-Tab + Enter /
  Space / arrows).
- Every widget ships both as a native C ABI call and as a one-class-per-file
  SLeeLa Master Class under
  [`/lib/user-interface`](../lib/user-interface/USER-INTERFACE.md).

## See also

- [`README.md`](README.md) — overview, build, and quick start.
- [`UI-PRINCIPLES.md`](UI-PRINCIPLES.md) — the enforced look-and-feel rules.
- [`THEMING.md`](THEMING.md) — the Slick Black palette and custom themes.
- [`ARCHITECTURE.md`](ARCHITECTURE.md) — layers, layout, rasterizer, backends.
- [`lib/user-interface/USER-INTERFACE.md`](../lib/user-interface/USER-INTERFACE.md)
  — the SLeeLa-source binding and the `ui*` built-in surface.

— SleelaUI™ · MEARVK LLC · 2026
