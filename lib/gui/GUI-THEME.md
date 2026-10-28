# SLeeLa GUI Theme Contract

Scope: version-mual 1.0, version-gual 2.0, version-thual 3.0, version-dual 4.0, and version-military 5.0.

The five GUI lines are related, progressively extended implementations—not five independent theme systems. Their shared visual contract is defined here and in gui/native/sleela_gui_theme.h.

## Required Chrome Colors

| Token | Value | Purpose |
|---|---|---|
| title-bar | #1D2025 | Base title-bar background |
| menu-bar | #1D2025 | Menu-bar background |
| url-bar | #383C47 | URL/navigation field background |

These values are fixed across all five GUI lines. Do not fork or silently reinterpret them per version.

## Shared Supporting Palette

| Token | Value | Purpose |
|---|---|---|
| panel | #292D35 | Secondary panels and sidebars |
| surface | #22262D | Main application surfaces |
| text | #E8EAED | Primary text on dark surfaces |
| muted-text | #B0B7C3 | Secondary text |
| border | #444A56 | Dividers and outlines |
| accent | #4F8CFF | Interactive accent |
| focus | #8AB4F8 | Keyboard focus indication |
| danger | #D9534F | Error/destructive state |
| success | #3FA776 | Positive state |

The supporting palette is a shared default. Products may add semantic tokens, but must retain the required title-bar and URL-bar colors and provide visible keyboard focus, readable text, and non-color-only status cues.

## Transitional Menu Depth Tokens

The five `SLGuiTheme.sleela` files share menu-specific presentation tokens for
the custom-widget fallback and hosts that can render elevated popups:

| Token | Default | Intended use |
|---|---|---|
| `MENU_POPUP_SURFACE` | `#292D35` | Dropdown surface |
| `MENU_POPUP_EDGE` | `#505866` | Fine popup outline |
| `MENU_ITEM_HOVER` | `#343A44` | Hovered item |
| `MENU_ITEM_PRESSED` | `#252A32` | Pressed item |
| `MENU_ITEM_HIGHLIGHT` | `#3A404B` | Active/open emphasis |
| `MENU_POPUP_SHADOW` | `0 8px 20px rgba(0,0,0,0.28)` | Soft elevation |
| `MENU_POPUP_RADIUS` | `6` | Nominal corner radius in px |
| `MENU_ITEM_RAISE` | `1` | Maximum subtle depth response in px |
| `MENU_TRANSITION_MS` | `120` | Short transition duration in ms |

These are transitional design tokens, not new mandatory native theme-color
entries. The values are source-defined in every GUI version to keep the design
consistent. Host renderers should translate them to native toolkit properties
where supported; they must not pass CSS-like shadow strings to a native API
that expects a different representation. If a host cannot render a shadow or
transition, it should retain the popup edge, item states, and focus indicator
instead of failing or silently changing layout.

## Responsibilities

- SLeeLa owns GUI state and the meaning of actions.
- The Java host owns JavaFX/Swing presentation and desktop lifecycle.
- The native C/C++ layer provides toolkit-neutral contracts.
- Each version line may extend behavior, but should consume the same base tokens.
- Theme values are presentation settings, not security or authorization controls.

## Verification

Run make clean all test from gui/native. The native test checks required colors, lookup bounds, null handling, and every registered token. GUI CI should validate the source library for all five version lines before release.

**Source-of-Truth Rule:** The five GUI examples and `SLGuiTheme.sleela` define the palette directly in SLeeLa source code. The `SLGuiApp` implementations apply those tokens to the live window theme. Do not depend on external theme files, environment variables, or generated settings to select these colors; editing the source palette is the authoritative change.
