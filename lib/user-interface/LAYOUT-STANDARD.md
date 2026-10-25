# The Standard UI Layout family — from `lib/user-interface`

`lib/user-interface` gives you **two** complementary layout surfaces:

1. **The native flow manager** — `SLLayout` + `SLMeasure` (here), backed by
   `user-interface/slui_layout.cpp`: named groups/items, flow solutions
   (stack/wrap/grid/dock/central), standardized units (US/metric/px/`fr`/`em`),
   named ergonomic presets, and n-ary adjustment. The *expert, native-backed*
   engine. See [`../../user-interface/LAYOUT.md`](../../user-interface/LAYOUT.md).

2. **The standard layout family** — `SLStackLayout`, `SLGridLayout`,
   `SLDockLayout`, `SLAbsoluteLayout`, the `SLLayoutKit` facade, and
   `SLLayoutBox`. These are the **professional, beginner-first** classes for
   everyday placement: pure SLeeLa, they compute real device-pixel rectangles
   and always check + run. They are **defined once in `lib/ui`** (class names
   are global in `/lib`, so they are not duplicated) and are **usable directly
   from here** — just `new SLStackLayout()` / `new SLLayoutKit()`.

   Full reference: [`../ui/LAYOUT.md`](../ui/LAYOUT.md).

## Which do I use?

- **Just placing a few things?** Use the standard family (`SLLayoutKit.column`,
  `.row`, `.grid`, `.dock`). Easiest, works everywhere, no native bridge needed.
- **Need named groups, ergonomic presets, physical units, or `fr` solving by the
  native engine?** Use `SLLayout` + `SLMeasure` here.

Both resolve to device pixels and can be used in the same program.
