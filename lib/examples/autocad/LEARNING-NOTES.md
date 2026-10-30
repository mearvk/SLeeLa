# AutoCAD examples — what to learn here

These examples teach how SLeeLa represents and measures **2D drawing geometry**.
They are deliberately *calculation-only*: no AutoCAD program is launched and no
drawing API is called. The point is to see how you turn shapes into numbers — the
same numbers you would later hand to a real CAD backend through the native bridge.

If you are new, read them in order. Each one adds exactly one idea.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-geometry-area.sleela`](BASIC-geometry-area.sleela) | Plain arithmetic on `double` values: area and perimeter of a rectangle. |
| Moderate | [`MODERATE-polyline-bounds.sleela`](MODERATE-polyline-bounds.sleela) | Shapes become *data*: an array of `Vertex` objects, walked to find a bounding box and path length. |
| Advanced | [`ADVANCED-layer-takeoff.sleela`](ADVANCED-layer-takeoff.sleela) | Grouped totals: sum each entity's area *per layer* — a simple quantity "takeoff". |

The original [`CLASSIC-geometry-primitives.sleela`](CLASSIC-geometry-primitives.sleela)
is the shortest starter and still a fine first read.

## What we want you to take away

1. **Geometry is just arithmetic until a backend draws it.** SLeeLa 1.7 has no
   built-in math functions — no `sqrt`, no `abs`. You compute with the operators
   `+ - * / %`, and when you need something like absolute value you write a tiny
   helper method (see `abs()` in the moderate example). This keeps the examples
   honest about what the language gives you for free.
2. **Model the drawing as data.** A single point is an object with `x` and `y`;
   a polyline is an array of those objects; a layered drawing is an array of
   entities each tagged with a layer. Once the drawing is data, measuring it is
   just a loop.
3. **The numbers are a specification, not a picture.** Every example ends by
   printing values (areas, bounds, totals). Those are exactly what a CAD
   integration would consume. The examples stop at the boundary on purpose.

## The 1.7 "array of objects" idiom (important)

The moderate and advanced examples use a specific, reliable pattern for arrays
whose elements are objects. Learn it once and reuse it everywhere:

- Declare the array as a **static field** and allocate it inside `main`
  (`drawing = new Entity[5];`). Allocating an object array in a *local*
  declaration is not accepted at this language level.
- **Build each element in a typed local**, set its fields there, then store the
  local into the array (see the `make(...)` / `at(...)` helpers).
- **Read an element back through a typed local** (`Entity e = drawing[i];`)
  before touching its fields. Reading or writing `drawing[i].field` directly
  loses the element's type and is rejected.

This idiom appears throughout the standard library (for example
`lib/autocad/SLCadDocument.sleela` allocates `new SLCadEntity[MAX_ENTITIES]`), so
recognizing it here will help you read real SLeeLa code.

## Run them

```sh
sleela check lib/examples/autocad/BASIC-geometry-area.sleela
sleela run   lib/examples/autocad/BASIC-geometry-area.sleela
```

Expected output is deterministic. For example the advanced takeoff prints
`walls area (layer 0) = 50.5`, `glazing area(layer 1) = 19.5`,
`floor area (layer 2) = 96`, and `total area = 166`.
