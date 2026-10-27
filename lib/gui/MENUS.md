# SLeeLa GUI Menu Contract

## Scope

`SLGuiMenuBar`, `SLGuiMenu`, and `SLGuiMenuItem` are toolkit-neutral menu state models available in `version-mual`, `version-gual`, `version-thual`, `version-dual`, and `version-military`.

## Widget Vocabulary

- `menubar` / `menu-bar`: horizontal menu-bar surface.
- `menu`: menu container.
- `menuitem` / `menu-item`: actionable button-like entry.
- `menuseparator` / `menu-separator`: horizontal separator.

The factories use existing `uiBox`, `uiButton`, and `uiSeparator` primitives. The menu and menu-bar tags are recognized as container surfaces; `menuitem`/`menu-item` tags are included in host activation polling and raise the ordinary `CLICK` event through `SLGuiHostAdapter`. This is the portable custom-widget fallback; native OS menu rendering still requires the Java/native host to implement the corresponding bridge behavior.

## Model Responsibilities

- **Menu bar:** stable name, menu count, visibility, focus, opened menu index, next/previous navigation.
- **Menu:** stable id and label, item count, visibility, open/dismiss, bounded selection, next/previous navigation.
- **Menu item:** id/label, named action and operation, optional shortcut/icon metadata, enabled state, checkable/checked state, separator state, and one-shot activation latch.

A menu item is activatable only if enabled, not a separator, and configured with a non-empty action name. The host/runtime should consume `takeActivation()` once and dispatch the item’s named action/operation through the existing action system. Menu model state does not itself execute arbitrary operations.

## Backend Capability

`SLGuiBackend.supportsNativeMenus()` returns true for JavaFX (`javafx`/`fx`) and the native backend. It is an eligibility/capability signal, not proof that a given host binary currently renders native menus. Swing and any host without native menu integration must use the custom-widget fallback.

## Interaction Contract

The shared `SLGuiMenuBar` model in all five GUI versions exposes the same
controller-level state transitions:

- `toggle(index)`: open the requested top-level menu, switch to another open
  menu, or close the same menu when it is clicked again.
- `dismissIfOutside(pointerInsideMenuRegion)`: close only when the pointer is
  outside the entire menu-bar **and popup** region. The host must classify this
  region as a whole; a click on a menu item must not trigger outside dismissal.
- `escape()`: dismiss an open menu and preserve menu-bar focus for predictable
  keyboard continuation.
- `activated()`: close after a successful ordinary menu-item activation.
- `navigate(direction)`: move between top-level menus, wrapping at either end.

These methods are toolkit-neutral state transitions. They do not manufacture
pointer coordinates or keyboard events; the host must call them from real input
events. This keeps the model testable and avoids inventing a premature
`OFFCLICK` event type. Existing `CLICK` remains the normal activation event.

## Integration Checklist

1. Mount the menu bar above the window content and preserve menu ordering.
2. Keep exactly one active top-level popup per menu bar.
3. Route pointer activation and Enter/Space through the same named-action latch.
4. Send outside-click classification to the menu controller once per pointer
   event, using the combined bar-and-popup bounds.
5. Support Escape dismissal, arrow-key navigation, and focus restoration.
6. Close menus after successful ordinary activation; preserve checked state for
   checkable items.
7. Render disabled entries and separators distinctly and expose accessible
   names/roles.
8. If native menus are unavailable, render the same model with custom widgets.
9. Do not execute the operation string as code; resolve it through the
   application's registered action table.
10. Test the same behavior on each supported host/backend before claiming native
    menus are complete.

## Visual Quality Baseline

- Use a full-width, single-row menu bar; dropdown contents flow vertically.
- Preserve the shared base palette from `SLGuiTheme`; depth tokens are a
  transitional layer and must not fork the chrome colors between GUI versions.
- Give dropdowns restrained elevation: a soft shadow, a fine one-pixel edge,
  and a slightly lifted surface. The effect should separate the popup from the
  page without looking like a floating card detached from its menu heading.
- Use a small corner radius (nominally 6 px) and a short transition (nominally
  120 ms) for opacity/elevation where the host supports it. Avoid bounce,
  large blur, perspective transforms, and long animations.
- Give menu items a minor pressed-depth response: a subtle surface shift and
  at most a one-pixel visual depression/raise. Do not move layout geometry or
  cause neighboring items to jump.
- Keep the popup surface `#292D35`, edge `#505866`, hover `#343A44`,
  pressed `#252A32`, and highlight `#3A404B` consistent across versions.
  These are menu-specific transitional tokens, not replacements for base
  chrome colors.
- Provide clear hover, keyboard-focus, pressed/open, disabled, checked, and
  separator states. Hover highlights; it must not be required to open a menu.
  Keyboard focus must remain visible without relying on shadow or color alone.
- Keep popup edges and item hit targets consistent, prevent clipping at window
  edges, and avoid popup geometry exceeding the parent/window bounds.
- Honor reduced-motion preferences when available; if not available, keep the
  transition short and purely decorative.
- Treat shadow, edge light, and depth as presentation only. They must not imply
  disabled/enabled state, security status, or action success.
- Preserve readable contrast and use spacing/alignment rather than decorative
  effects to establish hierarchy.

The five `SLGuiTheme` definitions now expose shared menu depth tokens and
accessors. These define the intended design language; a host renderer must
consume them before the effect is visible. Native and Java hosts should be
verified separately, and unsupported effects should degrade cleanly to a clear
border and hover/focus state.

## Host Activation Status

The factories recognize menu surfaces as containers. `menuitem` and `menu-item`
are included in `SLGuiWidgetFactory.interactive()` and map to `CLICK`, so the
existing `SLGuiHostAdapter` polls their activation latches and dispatches the
same DOM event type as ordinary buttons. The military command-console example
and the dedicated menu showcase both demonstrate visible, collapsible dropdown
panels backed by the `ui*` widget bridge.

## Limits

The SLeeLa library models now provide explicit toggle, outside-dismissal,
Escape, activation-dismissal, and top-level navigation state transitions.
These are not by themselves proof that every host wires those transitions to
real native pointer/keyboard input. Native OS menu widgets, popup positioning,
full keyboard event routing, outside-click dismissal, resolution of the model's
named action/operation fields, and accessibility semantics remain host/runtime
integration responsibilities and must be tested in each backend before being
described as complete.
