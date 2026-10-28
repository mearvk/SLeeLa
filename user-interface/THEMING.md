# SleelaUI™ Theming

SLeeLa provides five named, coordinated themes: **Default**, **White**, **Blue**,
**Black**, and **Gray**. The default is graphite/charcoal with a restrained
steel-blue accent; brown and amber are not part of the standard palette.

## Selection chain

1. **Native default:** a window opened with no explicit theme uses
   `SLUI_THEME_SLEELA_BASE` (the same palette as `SLTheme.DEFAULT`).
2. **Source selection:** use `slui_theme_preset(&theme, SLUI_THEME_WHITE)` in C,
   or `theme.preset(SLTheme.WHITE)` in SLeeLa source.
3. **Runtime configuration:** call `slui_theme_load_config` or
   `SLTheme.loadConfig(path)` explicitly. The config's `theme=` value selects
   one of the five presets, then optional role overrides are applied. The legacy
`slick-black` name is also accepted for existing configurations.
4. **Explicit role setters:** source code may set individual colours after
   choosing a preset. Those overrides are intentional and take precedence.

No config file is silently auto-loaded. This avoids an unknown home-directory
file unexpectedly changing application appearance.

## Named presets

| Name | Character |
|---|---|
| `default` | Charcoal/graphite, neutral light text, steel-blue accent |
| `white` | Light surface, dark text, restrained blue accent |
| `blue` | Deep blue surfaces, light text, brighter blue accent |
| `black` | Matte near-black surfaces, white text, blue accent |
| `gray` | Neutral mid-gray surfaces, light text, cool accent |

Legacy preset ids `SLICK_BLACK`, `GRAPHITE`, `CUSTOM`, and `SLEELA_BASE`
remain supported for ABI compatibility. `SLEELA_BASE` is the new cool default;
`black` selects the Standard Pattern Black palette, while `slick-black` retains
the legacy Slick Black palette.

## Select in C/C++

```c
SLUITheme theme;
slui_theme_preset(&theme, SLUI_THEME_BLUE);

SLUIWindowConfig cfg = {0};
cfg.title = "SLeeLa";
cfg.width = 900;
cfg.height = 600;
cfg.theme = &theme;
```

## Select directly in SLeeLa source

```sleela
SLTheme theme = new SLTheme();
theme.preset(SLTheme.DEFAULT); // Change to WHITE, BLUE, BLACK, or GRAY.
int window = ui.openWindow("SLeeLa", 900, 600, theme.handle());
```

The five `/lib/gui/version-*/SLGuiApp.sleela` sources expose the same choice as
`THEME_PRESET = SLTheme.DEFAULT`; changing that constant chooses a theme for
that GUI family without changing the toolkit.

## Runtime configuration

Call the loader explicitly after creating a theme. The config first selects a
preset, then applies optional overrides:

```sleela
SLTheme theme = new SLTheme();
theme.preset(SLTheme.DEFAULT);
boolean loaded = theme.loadConfig("user-interface/themes/blue.conf");
```

The repository includes `user-interface/themes/default.conf`,
`white.conf`, `blue.conf`, `black.conf`, and `gray.conf`. Installed
applications should use their installed configuration path rather than assume
the repository-relative example path.

Example config:

```ini
theme=blue
font_family=system
font_size=11
radius=8
```

Optional `base_color` and `accent` keys are supported for deliberate custom
palettes. A custom `base_color` derives neutral surfaces and a steel-blue
accent; it does not silently reintroduce the old amber accent.

## Palette roles and accessibility

| Field | Meaning |
|---|---|
| `bg` | Window/root background |
| `surface` | Panels, cards, menus |
| `surface_hi` | Hovered/raised surface |
| `chrome` | Title bar/header strip |
| `border` | Hairline separators |
| `fg` | Primary text and glyphs |
| `fg_dim` | Secondary/disabled text |
| `accent` | Focus ring, selection, suggested action, slider fill |
| `accent_fg` | Text drawn on the accent |
| `danger` | Destructive actions |
| `hover`, `active` | Hover and pressed-state tints |

Keep one accent per visual group, maintain at least 32px control hit targets, and
verify readable contrast on every role pairing after any custom overrides.
