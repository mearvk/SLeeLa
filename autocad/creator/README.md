<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa AutoCAD Creator™

Where the sibling [AutoCAD Renderer](../README.md) takes a plan and renders a
drawing in one shot, the **AutoCAD Creator™** lets you **create AND alter** a 2D
AutoCAD document interactively — add rooms, walls, dimensions and notes, then
move, re-label, re-layer or delete what you placed — and **save** the result as
a real AutoCAD **DXF** drawing (R12 text, opens in AutoCAD / LibreCAD).

Both the editing engine and the graphical interface are written in **SLeeLa**.
The GUI is built from **SleelaUI™ — SLeeLa's own cross-platform UI toolkit**
(under [`/lib/user-interface`](../../lib/user-interface)) — **not JavaFX**. It
raises a real native window (X11 / Cocoa / Win32) with the signature Slick Black
look, identical on every platform.

## Architectural rule

> **SleelaUI presents; SLeeLa decides.**

The GUI never computes geometry and never writes DXF. Every tool issues a
*create / alter / delete / undo / save* intent to the authoritative
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela); the Creator owns the
editable document and the edit history, and the canvas only *previews* the
geometry the Creator holds.

```text
        Tools + property fields + plan text         (SleelaUI window)
                         |
                         v  create / alter / delete / undo / save
            SLCadCreator  ->  SLCadDocument          <-- authoritative
         (edit history)       (editable entities)
                         |
            +------------+------------+
            |                         |
       drawing.dxf              live preview
     (AutoCAD / LibreCAD)      (SLCadCanvas, Draw API)
```

## What you can do

| Action  | Creator call                        | Effect                                   |
|---------|-------------------------------------|------------------------------------------|
| Create  | `wall` / `line` / `room` / `dimension` / `note` | Add geometry on the active layer. |
| Select  | `select(id)` / `pickAt(x,y,r)`      | Choose the entity a tool acts on.        |
| Move    | `move(id,dx,dy)` / `moveSelection`  | Translate an entity.                     |
| Reshape | `reshape(id,x1,y1,x2,y2)`           | Re-point a line/dimension's endpoints.   |
| Relabel | `relabel(id,text)`                  | Change a note / dimension label.         |
| Restyle | `restyle(id,layer)`                 | Move an entity onto another layer.       |
| Delete  | `delete(id)` / `deleteSelection`    | Soft-delete (ids stay stable).           |
| Undo    | `undo()`                            | Reverse the most recent recorded edit.   |
| Inspect | `inspect(id)` / `inspectSelection()` | One-line readout of an entity (kind, layer, coords, label). |
| Report  | `report()` / `listEntities()` / `listHistory()` | Full content + audit dump (metadata, entities, time-ordered edits). |
| Save    | `save()`                            | Serialise the whole document to DXF.     |

Every create and every alteration is captured as a reversible
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela), so editing is a
first-class, inspectable, undoable operation — that is what makes this a
*creator* rather than a one-shot renderer.

## Timestamping & inspection

Content is **timestamped** and **inspectable**:

- **Creation / update timestamps.** The document records when it was created and
  when it was last changed. Every edit advances the update stamp, and both land
  in the saved DXF HEADER as AutoCAD's standard `$TDCREATE` and `$TDUPDATE`
  variables (Julian date, group code 40, written as plain fixed-point decimals —
  never scientific notation). Time is **host-supplied** — the Creator carries a
  clock you set with `setClock(julian, label)` and `newDocumentAt(title, units,
  julian, label)` — because the runtime exposes no live wall clock; this keeps
  the engine deterministic and testable. A host with a real clock feeds its own
  Julian date in.
- **Per-edit audit trail.** Each [`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela)
  is stamped with a sequence number and the edit time, so `listHistory()` /
  `report()` print a time-ordered record of what changed and when.
- **Content inspection.** `inspect(id)` and `inspectSelection()` return a
  one-line description of an entity (kind, layer, coordinates, label, and a
  `[deleted]` flag for tombstones); `listEntities()` dumps them all and
  `report()` adds the document metadata, timestamps, and the audit trail. The
  GUI shows the selected entity in a live readout and has **Inspect** and
  **Report** buttons.

The runnable [`creator.sleela`](creator.sleela) demo ends with a headless
**SELF-CHECK** asserting these invariants (content created, create/update
timestamps present, an edit advanced `$TDUPDATE`, edits recorded, inspection
resolves an entity); `make creator` additionally greps the saved DXF to confirm
the `$TDCREATE`/`$TDUPDATE` records and a well-formed `EOF`.

## The classes

### Editing engine — reusable SLeeLa standard-library classes (`/lib/autocad`)

| Class | Role |
|---|---|
| [`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela) | One individually addressable, **mutable** 2D entity (line / text / dim): move, reshape, relabel, re-layer, soft-delete, and emit its own DXF. |
| [`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela) | The **editable** 2D document: an indexable entity store with create / alter / delete, a click hit-test (`pick`), extents recomputed from live geometry, and DXF serialisation. |
| [`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) | One recorded, **reversible** edit (create / move / reshape / relabel / restyle / delete) for undo + audit. |
| [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) | The **AutoCAD Creator™ facade**: owns the document, the selection, the active layer and the command history; exposes create / alter / delete / undo / save and plan import. |

These join the existing one-shot renderer classes in the same family
(`SLCadModel`, `SLCadPlan`, `SLCadDxf`, `SLCadRenderer`).

### Graphical interface — SleelaUI classes (this module)

| Class | Role | Built on |
|---|---|---|
| [`cad/SLCadCanvas.sleela`](cad/SLCadCanvas.sleela) | The 2D drawing surface: projects the editable document onto a canvas, fits the extents, flips Y, strokes walls/dimensions, draws text, and highlights the selection. | `SLCanvasView` + `SLDrawContext` + `SLColor` |
| [`cad/CadCreatorApp.sleela`](cad/CadCreatorApp.sleela) | The application window: header bar, a tool column (Select / Line / Room / Dim / Note / Move / Delete / Undo + layer chooser), a property strip, the canvas, a plan editor (`SLTextView`), and Save — each wired to the Creator. | `SLUserInterface`, `SLWindow`, `SLTheme`, `SLHeaderBar`, `SLBox`, `SLButton`, `SLEntry`, `SLComboBox`, `SLTextView`, `SLStatusBar`, `SLSeparator`, `SLLabel`, `SLSpacer` |

## Build & run

The split per-class files are the package's **discovery surface** (indexed by
the SLeeLa compiler / Loader at library parity, like every `/lib` class and like
the renderer's own `SLCadModel`/`SLCadDxf`/…). The executable counterpart is the
self-contained aggregate:

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
# or directly:
SLeeLa run creator.sleela > out/altered.dxf
```

[`creator.sleela`](creator.sleela) inlines the whole editing engine plus a
`main` that **creates** a drawing, **alters** it (move + relabel), **deletes**
and **undoes** an edit, and **saves** the altered document as DXF — the full
"alter the 2D documents and then save them" story, headless.

The SleelaUI GUI (`cad/CadCreatorApp.sleela`) opens a real window on a display
and drives the same editing API live; on a headless host it builds the drawing
in memory, prints a report plus the DXF, and exits cleanly (exactly like
`SLGalleryDemo` and the `/lib/gui` demos).

## Status

The editing engine (`/lib/autocad/SLCad{Entity,Document,Command,Creator}`) is a
complete create-alter-undo-save pipeline and runs end to end via
`creator.sleela`. The SleelaUI interface is a complete, buildable plan-entry +
tool + canvas shell wired to the Creator; a live display runs the event loop,
and persistence of the emitted DXF to a `.dxf` file is handled by the host the
same way the renderer captures its output.

**Max Rupplin — MEARVK LLC — 2026**
