# SLeeLa Tabs

Author: Max Rupplin — MEARVK LLC  
Date: 2026-10-26  
Family: `lib/user-interface`

SLeeLa provides three related tab patterns:

- `SLNotebook`: browser-style horizontal tabs with one visible page per tab.
- `SLTabBar`: a horizontal tab strip for applications that manage their own content.
- `SLVerticalTabs`: a vertical navigation rail beside a page stack.

All components use existing `SLButton`, `SLBox`, and `SLWidget` APIs. They do not hard-code palette colors: inactive, hover, pressed, focus, and selected appearance comes from the active native SleelaUI theme. The default theme is graphite/charcoal with a steel-blue accent. White, Blue, Black, and Gray are also available; tabs inherit the active preset rather than hard-coding colors.

## Browser-style notebook

```sleela
SLNotebook tabs = new SLNotebook();
tabs.createIn(root);

int general = tabs.addPage("General");
int editor = tabs.addPage("Editor");

SLLabel note = new SLLabel();
note.createIn(tabs.pageContent(general), "General settings");

SLLabel editorNote = new SLLabel();
editorNote.createIn(tabs.pageContent(editor), "Editor settings");

tabs.select(general);
tabs.setDirty(editor, true);
```

After the application's event loop dispatches input, poll `takeClickedTab()`. It returns the selected tab index, or `-1` if no tab was activated since the previous check. `selectPrevious()` and `selectNext()` wrap at either end.

## Standalone horizontal tab bar

```sleela
SLTabBar bar = new SLTabBar();
bar.createIn(root);
int home = bar.addTab("Home");
int files = bar.addTab("Files");
bar.select(home);
```

The caller owns the content region and reacts to `takeClickedTab()`. Use `setDirty(index, true)` for an unsaved-document marker.

## Vertical navigation

```sleela
SLVerticalTabs sections = new SLVerticalTabs();
sections.createIn(root);
int account = sections.addPage("Account");
int appearance = sections.addPage("Appearance");

SLLabel accountText = new SLLabel();
accountText.createIn(sections.pageContent(account), "Account preferences");

SLLabel appearanceText = new SLLabel();
appearanceText.createIn(sections.pageContent(appearance), "Appearance preferences");
```

Poll `takeClickedTab()` after input dispatch to learn which section was selected. Page content is returned by `pageContent(index)`.

## Button interaction state

`SLButton` keeps its original `clicked()` method. The native widget and VM bridge additionally expose:

- `hovered()`: current pointer-over state.
- `takePressed()`: one-shot pointer-down event.
- `takeReleased()`: one-shot pointer-up event, including release outside the button after a press.
- `takeHoverEntered()` and `takeHoverLeft()`: one-shot hover transition events.

These are polling/latch APIs, consistent with existing `clicked()` and the SLeeLa UI event-loop style. Call each method from the application's event/pump loop; each `take...` event is consumed once. `clicked()` remains activation on release inside the button, while `takeReleased()` reports the physical release independently.

## Design and layout notes

- Keep tab labels concise and consistent.
- Use the theme's suggested/accent state only for the selected tab.
- Preserve a minimum hit target of approximately 32–36 px.
- The native widget renderer supplies hover/pressed/focus feedback across platform backends.
- Parent clipping and box allocation remain responsible for preventing child overflow.
