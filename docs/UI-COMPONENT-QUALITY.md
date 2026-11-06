# UI component quality — visibility, margin, and icon

**Author:** Kiro Agent · SLeeLa project · 2026
**Status:** Shipped. The classes described here are in `lib/ui`
(`#sleela 2.5`), check and run with the source-built compiler, and are covered
by the runnable self-check `lib/ui/verify-pixel-perfect.sleela` (47 assertions,
all passing). Every native call named below exists in the `ui*` bridge
(`impl/core/sleela_ui_ids.h`); no capability is invented.

---

## Summary

In the `lib/ui` standard layout model a component is fully described by
**quality + size + placement**:

- **size** and **placement** come from `SLLayoutBox` (exact `x/y/w/h`) computed
  by the layout planners and organized by `SLLayoutFrame`.
- **quality** comes from `SLComponentQuality`, stored per component in
  `SLLayoutFrame` (`qualityOf(name)` / `qualityAt(index)`), and realized onto
  live native widgets by `SLLayoutDriver.applyQuality`.

`SLComponentQuality` now carries **six** faithfully-realizable quality axes. The
original three were role, emphasis, and interactivity; this edition adds
**visibility**, **margin**, and **icon**. Each axis maps 1:1 to a real `ui*`
bridge call, so the number of quality aspects the standard model can deliver to
the native toolkit grows without inventing any toolkit capability.

---

## The quality axes

| Axis | `SLComponentQuality` API | Realized by `SLLayoutDriver` via |
| --- | --- | --- |
| role | `asBody()` / `asSurface()` / `asChrome()` / `asAccent()`, `getRole()` | window theme (the toolkit has no per-widget surface call, so role is a themed-renderer hint) |
| emphasis | `normal()` / `primary()` / `danger()`, `isPrimary()` / `isDanger()` | `uiWidgetSuggested`, `uiWidgetDestructive` |
| interactivity | `enable()` / `disable()` / `setInteractive(on)`, `isInteractive()` | `uiWidgetSensitive` |
| **visibility** | `show()` / `hide()` / `setVisible(on)`, `isVisible()` | `uiWidgetVisible` |
| **margin** | `margin(px)` / `setMargins(top, right, bottom, left)`, `getMargin*()`, `hasMargin()` | `uiWidgetMargin` |
| **icon** | `icon(symbol)` / `iconFromAsset(path)` / `clearIcon()`, `getIconSymbol()` / `getIconAsset()`, `hasIcon()` | `uiWidgetSetIcon` |

Defaults for a freshly added component: `body / normal / enabled / shown`, no
margin, no icon. Negative margins are clamped to `0`.

`describe()` reports the whole quality in one line, e.g.:

```
quality:accent/primary/enabled/shown/margin[4,6,4,0]/icon(save)
```

---

## Example

```sleela
SLLayoutFrame f = new SLLayoutFrame(); f.create();
f.useColumn(8, 12);
f.add("title", 40);
f.add("list", 0);          < flexible: fills the rest >
f.add("save", 36);
f.place(0, 0, 320, 600);

f.qualityOf("title").asChrome();
f.qualityOf("list").asSurface(); f.qualityOf("list").margin(8);
f.qualityOf("save").asAccent(); f.qualityOf("save").primary();
f.qualityOf("save").icon("save");

< realize onto native widgets >
SLLayoutDriver d = new SLLayoutDriver(); d.create();
int box = d.realize(root, f);
int save = uiButton(box, "Save");
d.applyNamed(f, "save", save);   < size + suggested + icon applied >
```

When realized, `SLLayoutDriver.applyQuality` applies every honorable part:
`primary` → `uiWidgetSuggested`, `danger` → `uiWidgetDestructive`, `disable()`
→ `uiWidgetSensitive`, `hide()` → `uiWidgetVisible`, a margin →
`uiWidgetMargin`, and an icon → `uiWidgetSetIcon`. The role is honored through
the window theme.

---

## References

- Source: `lib/ui/SLComponentQuality.sleela`, `lib/ui/SLLayoutDriver.sleela`,
  `lib/ui/SLLayoutFrame.sleela`.
- Prose reference: `lib/ui/LAYOUT.md`.
- Self-check: `lib/ui/verify-pixel-perfect.sleela`
  (`sleela run lib/ui/verify-pixel-perfect.sleela` → `47 passed, 0 failed`).
- Generated API pages: `sleeladoc-api/package/ui.html` and the 2.9 snapshot
  `api/sleela/2.9/version-002/package/ui.html`.
- Native bridge ids: `impl/core/sleela_ui_ids.h`.
