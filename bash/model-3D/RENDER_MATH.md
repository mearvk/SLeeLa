# Rendering math & the rendering group

> Part of **SleelaTerminal™** (Phraign™ City 3D), SLeeLa's terminal product,
> built on GNU Bash (GPL, © Free Software Foundation).

Phraign™ City 3D's rendering is split into three small, layered pieces so the
geometry/color math is reusable and testable on its own, separate from both
Phraign and the city model:

```text
render_math    pure geometry + color + projection + scanline fill  (no Phraign)
    ▲
render_group   RenderSink over a Phraign frame + an ordered item container
    ▲
city_renderer  turns a City into a group of quads and draws it
```

## 1. Rendering math (`render_math.hpp/.cpp`, namespace `sleela::render`)

Basic, dependency-free primitives.

### Vectors

- `Vec2 { x, y }`, `Vec3 { x, y, z }` with `+`, `-`, and scalar `*`.
- `dot(a,b)`, `cross(a,b)`, `length(v)`, `normalize(v)` (zero-safe: a zero
  vector normalizes to `{0,0,0}`).
- `signedArea2(a,b,c)` — twice the signed area of a 2D triangle; positive means
  counter-clockwise (useful for winding / facing tests).
- `clampd(v,lo,hi)`, `lerpd(a,b,t)`.

### Color

`Rgba { r, g, b, a }` packed as `0xRRGGBBAA` to match the Phraign pixel word.

- `pack()` / `unpack(u32)` — convert to/from the Phraign `Pixel::value`.
- `shade(c, factor)` — scale RGB by a factor (alpha preserved), clamped to
  `[0,255]`. `factor > 1` lightens (lit faces); `factor < 1` darkens (shaded
  faces).
- `lerp(a, b, t)` — per-channel linear blend.

### Projection — `ObliqueCamera`

An oblique/dimetric camera: the scene is seen from above and slightly to the
side. World axes are **x = east**, **y = depth**, **z = up**. A world point maps
to the screen by:

```text
screen.x = origin_x + x*scale_x + y*tilt_x
screen.y = origin_y + y*tilt_y - z*scale_height
```

| Field | Meaning |
|-------|---------|
| `tilt_x` | screen-x skew per unit of depth (the side angle) |
| `tilt_y` | screen-y advance per unit of depth (how top-down it looks) |
| `scale_x` | screen px per unit east |
| `scale_height` | screen px per unit up (taller skyline) |
| `origin_x`, `origin_y` | screen px where world `(0,0,0)` lands |

`project(Vec3) -> Vec2` performs the mapping. It is intentionally affine (no
perspective divide), which keeps parallel building edges parallel — the classic
"toy city" look.

### Screen geometry & fill

- `BBox` — an accumulating axis-aligned bounds (`add(p)`).
- `Quad` — four screen-space corners in perimeter order; `bounds()` and
  `centroid()`.
- `RenderSink` — an abstract target: `width()`, `height()`, and
  `plot(x, y, rgba) -> bool`. This keeps the math free of any Phraign
  dependency; tests fill into a plain buffer.
- `fillQuad(sink, quad, rgba) -> pixelsWritten` — a scanline polygon fill,
  exact for convex quads, clipped to the sink. Pixel centers `(x+0.5, y+0.5)`
  are sampled, and each scanline uses half-open edge tests so shared edges are
  not double-filled.

## 2. The rendering group (`render_group.hpp/.cpp`)

A **rendering group** is an ordered collection of drawable items submitted to a
target and drawn in a controlled order. Because the city view has **no
z-buffer**, correct occlusion comes from draw order (the painter's algorithm).

### `PhraignSink`

A `RenderSink` implemented over a Phraign `PixelTerminal`. Each `plot()` writes
one pixel of the Phraign frame via `setPixel`, keeping rendering strictly
**per pixel** on the Phraign surface.

### `QuadItem` and `RenderGroup`

`QuadItem { Quad geom; uint32 color; double depth; }`.

`RenderGroup` collects items and draws them:

- `addQuad(quad, color, depth)` — color as packed `uint32` or `Rgba`.
- `addQuadAutoDepth(quad, color)` — uses the quad centroid's screen-y as the
  depth key (lower on screen ⇒ nearer ⇒ drawn later).
- `draw(sink, sortByDepth = true) -> pixelsWritten` — with `sortByDepth`, the
  items are **stably** sorted by ascending `depth` and drawn far→near
  (painter's algorithm); otherwise they draw in insertion order.

> Note: `draw(sortByDepth = true)` reorders the group's items **in place**, so a
> later unsorted `draw()` observes the already-sorted order.

## 3. How `city_renderer` uses them

For each block with height `h`, the renderer builds three world-space quads —
the **lit side** (east face), the **shaded side** (front face), and the **roof**
(top face) — projects them with the `ObliqueCamera`, and adds them to a
`RenderGroup`:

- The **ground plane** is one quad at depth `-1e18` (always behind everything).
- Each building's walls get a painter `depth` equal to the projected screen-y of
  the near base corner (plus a tiny height term); the **roof** gets `depth + 0.5`
  so it covers its own walls.
- The group is drawn through a `PhraignSink`, so far/short buildings are
  correctly overdrawn by near/tall ones.

The camera is derived from the City `Viewpoint`: `block_pitch` is folded into
`tilt_x`/`tilt_y` so one grid cell maps to one world unit while preserving the
on-screen spacing.

## Testing

`render_smoke.cpp` (built by `make` and run by `make smoke`) exercises the math
against a plain buffer sink and the group ordering/`PhraignSink` against a real
Phraign frame: vector ops, color pack/shade/lerp, projection direction checks,
quad bounds/centroid, `fillQuad` pixel counts and clipping, and depth-sorted vs.
insertion-order draw.
