# AutoCAD Renderer — Plan Format & DXF Mapping

This is the contract between the SLeeLa renderer and any front end (the JavaFX
GUI, a CLI, or an HTTP caller). It defines the input **plan** and how the
renderer maps it to **AutoCAD DXF** output.

## 1. The plan (input)

A plan is UTF-8 text, one directive per line. Blank lines and lines starting
with `#` are ignored. Tokens are whitespace-separated; coordinates and sizes are
numbers in the drawing's units.

```
title   <text...>                 drawing title (placed as a header note)
units   <mm|cm|m|in|ft>           drawing units (metadata)
at      <x> <y>                   insertion point applied to the next `room`
room    <name> <w> <h>            rectangular room w x h at the current `at`
wall    <x1> <y1> <x2> <y2>       a single wall segment
door    <x> <y> <w>               a door opening of width w at (x,y)
dim     <x1> <y1> <x2> <y2>       a linear dimension between two points
note    <text...>                 a free-text annotation
```

Semantics the renderer evaluates (the "equation"):

- A `room` expands to four `wall` segments forming its rectangle, offset by the
  current `at` insertion point. Successive rooms can be placed by changing `at`.
- A `door` subtracts an opening from the nearest coincident wall (represented in
  output as a gap in the wall line).
- A `dim` is laid out as a dimension with extension lines offset from the two
  points and a measured text label equal to the point distance.
- A `note` is placed on the notes layer; `title` is a note pinned at the top.

## 2. DXF output (output)

The renderer emits a minimal but valid AutoCAD **DXF R12** text file. R12 is the
most widely interoperable DXF level and needs no handles or object database.

Structure:

```
0 SECTION / 2 HEADER                 units + extents
0 SECTION / 2 TABLES / LAYER table   layers: walls, dims, notes
0 SECTION / 2 ENTITIES               the geometry below
  LINE   (layer walls)   room/wall segments
  LINE   (layer dims)    dimension witness/extension lines
  TEXT   (layer dims)    measured dimension value
  TEXT   (layer notes)   notes + title
0 ENDSEC / 0 EOF
```

Layer conventions:

| Layer   | Color | Contents                                  |
|---------|-------|-------------------------------------------|
| `walls` | 7     | Room and wall `LINE` entities.            |
| `dims`  | 1     | Dimension extension lines + measured text.|
| `notes` | 3     | Annotation and title `TEXT`.              |

The output is plain text and opens directly in AutoCAD, LibreCAD, and other CAD
tools with no conversion step.

## 3. Front-end contract

A front end submits a plan (text) and receives DXF (text). In SLeeLa terms:

- `planParse(text)` → a `Model`
- `modelAddRoom(model, name, w, h, x, y)` and one builder per directive
- `dxfEmit(model)` → the full DXF string

The JavaFX GUI mirrors these through `SleelaJavaConnector` and additionally asks
for a lightweight entity list to draw the on-screen preview; it never computes
geometry or writes DXF itself. See `gui/`.

**Max Rupplin — MEARVK LLC — 2026**
