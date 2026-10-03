<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa AutoCAD Renderer

Take in **descriptions, dimensions, plans, and notes**, and have SLeeLa render
the output as an AutoCAD drawing.

SLeeLa is authoritative: it parses a human plan (rooms, walls, dimensions,
annotations) into a geometric model, evaluates it (the "equation" — layout math,
offsets, dimension strings), and emits a real AutoCAD-compatible **DXF** drawing
(the open AutoCAD Drawing Interchange Format, readable by AutoCAD, LibreCAD,
etc.). A JavaFX GUI is provided as a thin surface for entering the plan and
previewing the rendered geometry.

## Architectural rule

> **JavaFX presents; SLeeLa decides.**

The GUI never computes geometry or writes DXF. It collects the description,
dimensions, plan, and notes, submits them to the SLeeLa renderer through the
common Java connector (process / RMI / HTTP), and previews the geometry the
renderer returns.

```text
   Description + Dimensions + Plan + Notes
                     |
                     v
          SLeeLa AutoCAD Renderer            <-- authoritative
   (cad/*.sleela : parse -> model -> evaluate -> emit DXF)
                     |
         +-----------+-----------+
         |                       |
     drawing.dxf             live preview
   (AutoCAD / LibreCAD)           |
                              JavaFX GUI
                           (autocad/gui)
```

## What goes in

A **plan** is a small, line-oriented description a person can type. Each line is
a directive:

| Directive  | Example                                  | Meaning                               |
|------------|------------------------------------------|---------------------------------------|
| `title`    | `title Office Floor`                     | Drawing title (header note).          |
| `units`    | `units mm`                               | Drawing units.                        |
| `room`     | `room Lobby 6000 4000`                   | Rectangular room: name W H.           |
| `at`       | `at 0 0`                                 | Insertion point for the next room.    |
| `wall`     | `wall 0 0 6000 0`                        | A wall segment: x1 y1 x2 y2.          |
| `door`     | `door 2000 0 900`                        | A door opening: x y width on a wall.  |
| `dim`      | `dim 0 0 6000 0`                         | A linear dimension between two points.|
| `note`     | `note Fire exit on north wall`           | A text annotation.                    |

Dimensions, plans, and notes together are the "equation" SLeeLa evaluates: it
resolves room placement, derives wall rectangles, lays dimension strings off the
geometry, and places notes — then emits the result as DXF.

## What comes out

A single `drawing.dxf` file containing:

- A `walls` layer with the room/wall `LINE` entities.
- A `dims` layer with linear dimensions rendered as witness/extension lines plus
  measured `TEXT`.
- A `notes` layer with annotation `TEXT`.

DXF is plain text and self-contained; it opens directly in AutoCAD and other CAD
tools without conversion.

## Layout

```text
autocad/
  README.md                 This document.
  FORMAT.md                 The plan directives + DXF mapping in detail.
  Makefile                  Build/render helpers (dispatch into SLeeLa).
  cad/                      SLeeLa renderer (authoritative).
    model.sleela            Geometry model: points, rooms, walls, dims, notes.
    plan.sleela             Parses plan directives into the model.
    dxf.sleela              Emits the model as AutoCAD DXF text.
    render.sleela           Top-level: build a demo plan and render drawing.dxf.
  samples/
    office.plan             A sample plan (description + dimensions + notes).
  gui/                      JavaFX plan-entry + geometry preview surface.
    pom.xml
    src/main/java/module-info.java
    src/main/java/com/mearvk/sleela/autocad/gui/
      CadStudioApp.java             JavaFX application (presentation only).
      CadPlanModel.java             Presentation model + connector bridge.
      Entity.java                   Geometry primitive used by the preview.
```

## Status

The SLeeLa renderer defines the full parse → model → evaluate → emit-DXF
pipeline and a runnable demo (`cad/render.sleela`). The JavaFX GUI is a
complete, buildable plan-entry shell wired to call the renderer through
`SleelaJavaConnector`; the live connector transport is the next integration
point and is clearly marked.

**Max Rupplin — MEARVK LLC — 2026**