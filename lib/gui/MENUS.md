# SLeeLa GUI Menu Contract

## Scope

`SLGuiMenuBar`, `SLGuiMenu`, and `SLGuiMenuItem` are toolkit-neutral menu state models available in `version-mual`, `version-gual`, `version-thual`, `version-dual`, and `version-military`.

## Widget Vocabulary

- `menubar` / `menu-bar`: horizontal menu-bar surface.
- `menu`: menu container.
- `menuitem` / `menu-item`: actionable button-like entry.
- `menuseparator` / `menu-separator`: horizontal separator.

The factories use existing `uiBox`, `uiButton`, and `uiSeparator` primitives. This is a portable custom-widget fallback; native OS menu rendering requires the Java/native host to implement the corresponding bridge behavior.

## Model Responsibilities

- **Menu bar:** stable name, menu count, visibility, focus, opened menu index, next/previous navigation.
- **Menu:** stable id and label, item count, visibility, open/dismiss, bounded selection, next/previous navigation.
- **Menu item:** id/label, named action and operation, optional shortcut/icon metadata, enabled state, checkable/checked state, separator state, and one-shot activation latch.

A menu item is activatable only if enabled, not a separator, and configured with a non-empty action name. The host/runtime should consume `takeActivation()` once and dispatch the item’s named action/operation through the existing action system. Menu model state does not itself execute arbitrary operations.

## Backend Capability

`SLGuiBackend.supportsNativeMenus()` returns true for JavaFX (`javafx`/`fx`) and the native backend. It is an eligibility/capability signal, not proof that a given host binary currently renders native menus. Swing and any host without native menu integration must use the custom-widget fallback.

## Integration Checklist

1. Mount the menu bar above the window content and preserve menu ordering.
2. Route pointer activation and Enter/Space through the same named-action latch.
3. Support Escape dismissal, arrow-key navigation, and focus restoration.
4. Close menus after successful ordinary activation; keep checked state for checkable items.
5. Render disabled entries and separators distinctly and expose accessible names/roles.
6. If native menus are unavailable, render the same model with custom widgets.
7. Do not execute the operation string as code; resolve it through the application's registered action table.

## Limits

The SLeeLa library model and factory tags are implemented. Native OS menu widgets, full keyboard event routing, submenu rendering, and action dispatch are host/runtime integration responsibilities and must be tested in each backend before being described as complete.
