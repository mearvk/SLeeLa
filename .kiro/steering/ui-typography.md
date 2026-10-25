---
inclusion: always
---

# Typographic Specification: Font Architectures for Long-Term UI Occupancy

The governing typography standard for SleelaUI, the companion to
`ui-coherent-dignity.md`. Typography is a structural grid of system states, not
editorial decoration: it must prioritise structural stability, mathematical
legibility, and resistance to design trends — "overall dignity of method."

Applies to the `user-interface/` toolkit (a FreeType-backed software rasterizer
on X11/Cocoa/Win32, plus a built-in-glyph headless backend), the `ui*` bridge,
and the `example/` demos.

## 1. The four geometric requirements of a UI typeface

A viable UI typeface must meet all four:

- **Proportional x-height (≥ 65% of cap height).** Lowercase keeps a large
  vertical footprint so technical readouts and micro-text stay readable at
  9–11px.
- **Open apertures.** The openings of `c e s a` stay wide and distinct so
  glyphs do not blend into blobs at low contrast / low resolution.
- **Homoglyph differentiation.** Clear visual distinction between confusable
  glyphs — numeral `1` ≠ lowercase `l` ≠ uppercase `I` ≠ pipe `|`, and numeral
  `0` (slashed/dotted) ≠ uppercase `O` — to prevent catastrophic operational
  errors.
- **Tabular, monospaced numerals.** Even in a proportional family, digits share
  one width (tabular lining) so live-updating data columns from the C/C++ core
  never jitter.

## 2. Taxonomy of indestructible UI fonts

1. **Neo-Grotesque workhorses (modern generalist UI).** Neutral, objective,
   structurally invisible. Preferred: **Inter** (built for screens: high
   x-height, homoglyph disambiguation, micro-text alternates), then the
   platform standards **SF Pro / Segoe UI / Roboto**, and **SF Compact** for
   dense containers.
2. **Micro-technical & monospaced engines (data-dense / terminal).** Rigid,
   mechanical, every glyph one width — for tabular arrays, logs, matrices, and
   diagnostic panels tied to procedural C/C++ metrics. Preferred: **JetBrains
   Mono**, then **SF Mono / Consolas**, then **IBM Plex Mono**.
3. **Humanist geometric hybrids (high-comfort workspace UI).** Warm, structured,
   organic; varied widths and open shapes reduce multi-hour fatigue. Preferred:
   **IBM Plex Sans**, **Plus Jakarta Sans**.

**Toolkit default family preference.** The X11 backend resolves the `"system"`
family through Fontconfig against this ordered chain, first real match wins,
with a guaranteed `sans-serif` fallback: **Inter → SF Pro Text → SF Pro →
Segoe UI → Roboto → sans-serif** (`resolve_face` in
`user-interface/src/backend/x11.cpp`). A host may still request any specific
family by name.

## 3. The Fixed Scale Array (snap to the 8px baseline grid)

Fonts scale in discrete integer steps; fluid/arbitrary sizes are **banned**
(they cause fractional-pixel rendering defects). Line-heights land on the 8px
grid so wraps align with layout boxes.

| Level | Size | Line height | Tracking | Purpose |
|---|---|---|---|---|
| 0.5X | 11px | 16px (2× grid) | +0.03em (looser) | dense status readouts, metadata tags |
| 1.0X | 14px | 24px (3× grid) | 0.00em (neutral) | default interface text, labels, inputs |
| 1.5X | 18px | 24px (3× grid) | −0.01em (tighter) | subsection headers, card titles |
| 2.0X | 24px | 32px (4× grid) | −0.02em (tight) | top-level window headers, canvas anchors |

- **Inverse tracking rule:** as size ↓, tracking ↑ (small text needs breathing
  room); as headers grow, tracking contracts to hold word shapes.
- **Integer baseline snapping:** a text container's BBox resolves to an even
  multiple of 8px; lines are centred with explicit software padding, not loose
  margin flow.
- **Density note:** the toolkit's theme default `font_size` is 11 (level 0.5X),
  appropriate for the dense console UIs. Choose the level by density; do not
  interpolate between levels.
- **Alignment:** numerical columns right-aligned with tabular digits; textual
  labels left-aligned (consistent vertical scanning axis).

## 4. Backend font rendering

- **Sub-12px monochrome hinting (implemented).** The FreeType path renders
  glyphs **monochrome** (`FT_LOAD_TARGET_MONO`, 1-bpp expanded to full coverage)
  when the pixel size is `< 12`, and antialiased at 12px and up. This keeps tiny
  technical text ultra-sharp and removes coloured fringing on dark backgrounds
  (`rasterize_glyph` in `user-interface/src/backend/x11.cpp`). The headless
  backend uses built-in crisp glyphs and is unaffected.
- **MSDF (deferred, not applicable to the current engine).** The spec's
  Multi-Channel Signed Distance Field pipeline targets a GPU/shader text path
  (OpenGL/Vulkan vertex shaders). SleelaUI is a **CPU software rasterizer** that
  blits a pixel buffer, so MSDF is intentionally NOT implemented here; it is the
  intended approach only if/when a GPU backend is added.

## 5. Deferred (require a real display / larger effort)

- **Non-Latin fallback chains** (CJK logographs, Arabic shaping + RTL) beyond
  what Fontconfig resolves by default — a substantial, separate effort; map it
  out with the same layout proportions when targeting those scripts.
- Visual verification of glyph rendering is not possible in a headless sandbox;
  confirm hinting/family choices on a desktop (X/Cocoa/Win32) build.
