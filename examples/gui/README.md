# SleelaUI GUI examples

A practical, SLeeLa-source example collection for the native GUI API in
`/lib/user-interface` (the high-level `SL*` object model) and the related
`/lib/ui` SDK facade family.

These examples use the documented `#sleela 2.5` user-interface classes and
real SleelaUI bridge calls. They are intentionally organized around reusable
patterns: application/window lifecycle, theme selection, component groups,
layout, input, status/feedback, tabs, and interaction polling.

## Start here

1. [CLASSIC Hello Window](CLASSIC-hello-window.sleela)
2. [USED Full Widget Gallery](USED-widget-gallery.sleela)
3. [USED Theme Presets Gallery](USED-theme-presets.sleela)
4. [NEW Custom Theme Roles](NEW-custom-theme-roles.sleela)
5. [USED Form Layout](USED-form-layout.sleela)

## Theme examples

| Example | Theme |
|---|---|
| [Default / graphite](THEME-default-graphite.sleela) | `SLTheme.DEFAULT` |
| [White](THEME-white.sleela) | `SLTheme.WHITE` |
| [Blue](THEME-blue.sleela) | `SLTheme.BLUE` |
| [Black](THEME-black.sleela) | `SLTheme.BLACK` |
| [Gray](THEME-gray.sleela) | `SLTheme.GRAY` |
| [Legacy Slick Black](THEME-legacy-slick-black.sleela) | `SLTheme.SLICK_BLACK` |
| [Config-loaded theme](THEME-load-config.sleela) | Explicit `.conf` load |

The five current named presets are Default, White, Blue, Black, and Gray.
Legacy IDs Slick Black, Graphite, and Custom remain supported for ABI
compatibility. No theme configuration is silently loaded.

## Component examples

| Example | Components / APIs |
|---|---|
| [Containers and layout](COMPONENT-containers-layout.sleela) | Box, HeaderBar, Grid, Frame, Card, StatusBar, Separator, Spacer |
| [Buttons and actions](COMPONENT-buttons-actions.sleela) | Button, suggested/destructive, link button, icon |
| [Text and input](COMPONENT-text-input.sleela) | Label, Heading, Entry, SearchEntry, PasswordEntry |
| [Selection controls](COMPONENT-selection-controls.sleela) | Toggle, CheckBox, RadioButton, ComboBox, SpinButton |
| [Progress and indicators](COMPONENT-progress-indicators.sleela) | Slider, ProgressBar, LevelBar, Spinner, ScrollBar, Throbber |
| [Display and feedback](COMPONENT-display-feedback.sleela) | Avatar, Image, Badge, Chip, InfoBar, Tooltip |
| [Tabs and pages](COMPONENT-tabs-notebook.sleela) | Notebook, TabBar, VerticalTabs |
| [Widget common properties](COMPONENT-widget-properties.sleela) | Margins, alignment, expansion, size, visibility, sensitivity |
| [Button event polling](INTERACTION-button-events.sleela) | Click, hover, press/release latches |
| [Settings panel](FORM-settings-panel.sleela) | Composed real-world preference form |
| [SDK family scope](SDK-ui-family-scope.md) | Distinguishes `lib/ui` model classes from native `lib/user-interface` bindings |
| [SDK object model](SDK-ui-object-model.sleela) | Exercises the current `lib/ui` model methods without claiming native rendering |
| [Window lifecycle](WINDOW-lifecycle-buffering.sleela) | Show/hide, title, redraw, single/double/triple buffering |
| [Backend and display info](SYSTEM-backend-screen-info.sleela) | Backend, toolkit version, screen dimensions |

## Running

From the repository root after building/installing SLeeLa and SleelaUI:

```sh
make -C impl
./impl/build/sleela check examples/gui/CLASSIC-hello-window.sleela
./impl/build/sleela run examples/gui/CLASSIC-hello-window.sleela
```

For the component examples, substitute the desired file name. A real desktop
session is required for native windows; a headless build can validate/compile
source but cannot present an interactive native window.

## Theme and component design rules

- Use `SLTheme.DEFAULT` unless the application has a reason to choose another.
- Keep one suggested/accent action per visual group; reserve destructive styling
  for genuinely destructive actions.
- Use consistent 10px spacing as the toolkit's practical safe baseline; use
  smaller gaps only for compact structures such as tab strips.
- Keep interactive hit targets at least 32px; prefer 36px or more.
- Let widgets consume theme roles; do not hard-code individual widget colours
  to imitate a preset.
- Use `SLWidget` margin, alignment, expansion, and minimum-size APIs instead of
  positioning children beyond their parent.
- Poll one-shot `clicked()` / `changed()` / `take...` events from the UI
  event loop; each latch is consumed when read.
- Use VM-local handles only through the SLeeLa wrapper API. They are not native
  OS pointers.
- A successful source check is not proof that a display backend is available.

## API references

- [SLeeLa binding API](../../lib/user-interface/USER-INTERFACE.md)
- [Widget catalogue](../../user-interface/WIDGETS.DESCRIPTION.md)
- [Theme reference](../../user-interface/THEMING.md)
- [Tabs reference](../../lib/user-interface/TABS.md)
- [Native toolkit overview](../../user-interface/README.md)

The `lib/ui` directory is the separate unified SDK front-end family; its
classes and feature contracts should not be conflated with the 53-class native
`lib/user-interface` wrapper family. These runnable GUI examples target the
latter unless explicitly noted.
