# SLeeLa IDE Workbench — Architecture & Gap-Closure Plan

**Revision:** 0.1 · **Family:** `ide-workbench`

This package is the **composition layer** that turns the existing SLeeLa library
contracts into a fully functional IDE for SLeeLa development. It does **not**
reimplement windowing, compilation, or the document model — it *wires together*
the authoritative packages that already exist:

```
            +---------------------------- IDE Workbench ----------------------------+
            |                                                                        |
  edit ---> | SLIdeEditor  --+                                                       |
            | SLIdeTabGroup  |  SLIdeCommandRegistry  (Save / Compile / Run / Open)  |
            | SLIdeProjectTree                                                        |
            | SLIdeDiagnosticsPanel                                                   |
            | SLIdeWorkbench  (owns the window, panels, lifecycle)                    |
            +----|-----------------|------------------|----------------|-------------+
                 |                  |                  |                |
          lib/gui (window,   lib/user-interface  lib/nordshrift   lib/sldocument
          actions, 1..14     + lib/ui (widgets)   (.sst -> .sleela) lib/compiler
          doc listener)                            = "SST Nordshrit" (compile/save/load)
```

## Division of authority (unchanged from the repo's posture)

| Concern | Owner | Package |
|---|---|---|
| Window / actions / desktop lifecycle | the GUI integration layer | `lib/gui`, `lib/user-interface`, `lib/ui` |
| `.sst` authoring → `.sleela` → compile | Nordshrift + the SLeeLa compiler | `lib/nordshrift`, `lib/compiler` |
| Save / load / document model | the document layer | `lib/sldocument` |
| IDE composition (editor, tabs, tree, commands, panels) | **this package** | `lib/ide-workbench` (new) |

The IDE never becomes a second business-logic implementation: SLeeLa owns
meaning, the compiler stays authoritative for the language, and Nordshrift stays
authoritative for `.sst`.

## What already existed (why we are not starting from scratch)

- **`lib/gui`** — `SLGuiBackend`, `SLGuiWindow`, `SLGuiAction`, `SLGuiHost`,
  `SLGuiRuntime`, `SLGuiBridge`, `SLGuiDocument`, `SLGuiDocumentListener`,
  `SLGuiIntegration`. Gives us a window, bindable actions, and an OS-driven
  file-change watch (**hard-bounded to 1..14 documents**).
- **`lib/user-interface`** (~55 widgets) and **`lib/ui`** (16 widgets) —
  `SLTreeView`, `SLTableView`, `SLScrollBar`, `SLStatusBar`, `SLHeaderBar`,
  `SLMenu`, `SLDialog`, `SLTextArea`, etc.
- **`lib/nordshrift`** — the `.sst` → `.sleela` pipeline
  (`NordshriftSource`, `NordshriftCompilerBridge`): the "SST Nordshrit" path.
- **`lib/compiler`** — `SLCompiler`, pipeline, diagnostics, build artifacts.
- **`lib/sldocument`** — `SLDocument`, compiler, converter, result.
- **`/ide`** — the IntelliJ-Platform integration layer and the IDE contract.

## The gaps this package closes

| # | Gap in the pre-existing libraries | New class |
|---|---|---|
| 1 | No real **code editor** (`SLTextArea` is a stub: no caret/selection/dirty/undo) | `SLIdeEditor` |
| 2 | No **multi-document / tab** model; `SLGuiWindow` is single-text; listener caps at 1..14 | `SLIdeTabGroup` |
| 3 | No **project / file explorer** composition bound to a tree widget | `SLIdeProjectTree` |
| 4 | No **command registry** mapping Save/Compile/Run/Open to services | `SLIdeCommandRegistry` |
| 5 | No **diagnostics-in-editor** surface for compiler output | `SLIdeDiagnosticsPanel` |
| 6 | No **top-level composition** owning window + panels + the edit→compile→save loop | `SLIdeWorkbench` |

### The 1..14 document limit

`lib/gui`'s `SLGuiDocumentListener` enforces a hard **1..14** watched-document
contract. An IDE routinely has more files open than that. The workbench does
**not** break that contract — instead `SLIdeTabGroup` owns an unbounded set of
open editors and only ever registers the **active + recently-focused** editors
(≤14) with the OS watch, re-subscribing as focus changes. The 1..14 bound
remains the OS-watch contract; the IDE's open-file count is separate.

## The core edit → compile → save loop

```
SLIdeEditor.setText(...)        -> buffer dirty
SLIdeCommandRegistry "compile"  -> NordshriftCompilerBridge.prepare()/compileToSleela()
                                   (.sst) OR SLCompiler directly (.sleela)
compiler diagnostics            -> SLIdeDiagnosticsPanel.publish(...)
SLIdeCommandRegistry "save"     -> SLDocument / SLDocumentConverter persist
SLGuiDocumentListener           -> external change -> active editor refresh
```

## The GUI is written in SLeeLa (SleelaUI)

The IDE's user interface is authored **entirely in SLeeLa** on top of
**SleelaUI™** (`/lib/user-interface`), SLeeLa's own cross-platform toolkit whose
`ui*`/`draw*` built-ins drive real native windows (X11/Cocoa/Win32). Nothing in
the GUI is deferred to a foreign toolkit.

| Layer | Class | Role |
|---|---|---|
| **GUI shell** | `SLIdeWorkbenchUI` | Builds the real window: header-bar commands, a split body (tree + editor notebook), a status bar; runs the SleelaUI event loop and polls widget activations. |
| state model | `SLIdeEditor`, `SLIdeTabGroup`, `SLIdeDiagnosticsPanel`, `SLIdeProjectTree` | The IDE's logic/state (dirty tracking, the 1..14 watch cap, the diagnostics failure rule, language routing). |
| service contract | `SLIdeWorkbench` | The headless `openFile -> compileActive -> saveActive` loop and the Nordshrift/compiler handoff (toolkit-independent). |

The GUI shell is built from these **competent SleelaUI widgets**, all written in
SLeeLa over the toolkit primitives (no new C ABI):

- `SLTreeView` — the project explorer (depth, expand/collapse, selection);
- `SLNotebook` — the tabbed, dirty-aware editor surface;
- `SLTextView` — a real multi-line **code editor** (line buffer, caret,
  selection, gutter line numbers, drawn via the Draw API);
- `SLScrollView` — the scrollable viewport the tree/editors sit in.

Run it: `impl/build/sleela run lib/ide-workbench/ide-workbench-gui-demo.sleela`
(exits cleanly on a headless host with no display, like `SLGalleryDemo`).

## Scaffolding status

The GUI shell and widgets are complete SLeeLa source. The remaining host work is
the toolchain glue the service layer calls out to (Nordshrift + `sleela check`,
and the real OS `WatchService`), which the `/ide` IntelliJ layer and the `gui/`
native host provide; the SLeeLa side no longer defers any *widget* work to a
foreign toolkit.

## Build / verify

Every `lib/ide-workbench/*.sleela` is intended to pass `impl/build/sleela check`
at library parity, exactly like `lib/gui`. The runnable walkthrough is
`ide-workbench-demo.sleela`.

**SLeeLa — MEARVK LLC — 2026**
