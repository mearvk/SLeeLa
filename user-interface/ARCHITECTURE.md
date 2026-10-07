# SleelaUI™ Architecture

SleelaUI is an original cross-platform widget toolkit. This document explains
how the pieces fit so a contributor can add a widget, add a backend, or retheme
the toolkit with confidence.

## Layers

```text
include/sleela_ui.h      Stable C ABI. The entire installed surface.
src/slui_api.cpp         ABI implementation: App/Window lifecycle, widget
                         construction, marshalling the flat C calls onto C++.
src/slui_window.{hpp,cpp}  Per-window object: owns the widget tree + Canvas +
                         resolved theme; lays out, paints, routes input/focus.
src/slui_widget.{hpp,cpp}  Widget tree: measure/arrange/paint/on_event for every
                         built-in control; UTF-8 text helpers.
src/slui_theme.{hpp,cpp}   SLUITheme <-> internal Theme; the Slick Black /
                         Graphite presets.
src/slui_render.hpp      Canvas: the software rasterizer (AA rounded rects,
                         hairlines, gradients, glyph coverage blit).
src/slui_geometry.hpp    Rect + straight-alpha Color + compositing helpers.
src/slui_backend.hpp     The one seam to the OS: Backend + NativeWindow.
src/backend/*.cpp|.mm    Exactly one is compiled per build.
```

Everything above `slui_backend.hpp` is OS-neutral and makes all look-and-feel
decisions. Everything in `backend/` is OS-specific and makes none.

## The two-pass box layout

Layout follows the GTK/CSS box tradition:

1. **measure** — each widget reports a natural (preferred) size including its
   own margin. Containers recurse into children.
2. **arrange** — a container distributes its content box along the main axis:
   natural sizes first, then any surplus split among `expand` children, then
   each child is aligned on the cross axis (`start`/`center`/`end`/`fill`).

`Spacer` is simply an expanding zero-size child; a header bar is a horizontal
box that also paints the chrome fill and a bottom hairline.

## The software rasterizer

`Canvas` is a 32-bit `0xAARRGGBB` framebuffer. It provides analytic
antialiased rounded-rectangle fills and strokes (coverage from a signed-distance
test against the rounded boundary), vertical gradients, 1px hairlines, and an
8-bit coverage glyph blitter. The window buffer is kept opaque; widgets
composite straight-alpha source colours over it.

Drawing identical pixels everywhere is the whole point: the "Slick Black" look
does not depend on any native theme.

## The backend seam

A `Backend` owns the host connection and the font engine and manufactures
`NativeWindow`s. A `NativeWindow` shows/sizes/titles a real top-level window,
delivers native input to its portable `Window` as `SLUIEvent`, and blits the
`Canvas` on present. The font engine resolves a family+size and rasterizes a
unicode scalar to 8-bit coverage plus a pen advance.

To add a backend, implement `create_backend()` and the two interfaces, then add
a branch to the Makefile's platform detection. Nothing else changes — the core
and every widget are already backend-agnostic.

| Backend | Window | Blit | Glyph coverage | Family resolve |
|---|---|---|---|---|
| X11 | Xlib `XCreateWindow` | `XPutImage` (ZPixmap) | FreeType `FT_LOAD_RENDER` | Fontconfig |
| Win32 | `CreateWindowExW` | `SetDIBitsToDevice` (top-down DIB) | `GetGlyphOutlineW` `GGO_GRAY8` | GDI `CreateFontW` |
| Cocoa | `NSWindow` + custom `NSView` | `CGImage` draw | Core Text into gray `CGBitmapContext` | `CTFontCreateWithName` |
| Headless | in-memory | no-op | built-in 5×7 bitmap | n/a |

### Pixel byte order

The `Canvas` packs `0xAARRGGBB`. On little-endian hosts this is `BGRA` in
memory, which matches: X11 `ZPixmap` on a 24/32-bit TrueColor visual, a Win32
`BI_RGB` 32bpp DIB, and a CoreGraphics
`kCGBitmapByteOrder32Little | kCGImageAlphaNoneSkipFirst` image. No per-pixel
swizzle is needed on the common targets.

## Events and focus

The `Window` routes input: pointer events hit-test the tree front-to-back;
keyboard/text events go to the focused widget; `Tab`/`Shift-Tab` cycle focus
through focusable widgets; and unconsumed window events (notably `CLOSE`) fall
through to the host's `SLUIEventHandler`. The event model is a small tagged
union (`SLUIEvent`) so a C host or the SLeeLa VM bridge can handle input without
C++.

## Memory ownership

An `SLUIApp` owns its `Window`s; a `Window` owns its widget-tree root; each
widget owns its children (`unique_ptr`). Destroying the app frees everything in
one pass. The public ABI hands back *borrowed* raw pointers into this tree.

— SleelaUI™ · MEARVK LLC
