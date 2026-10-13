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

## Scaffolding status

These classes are **composition scaffolding with real bindings to the existing
contracts** — they model IDE state and the service handoffs, in the same
contract-first style as `lib/gui`. They are the skeleton to flesh out against
the live Java host / native bridge; they are not yet a shipping desktop binary.

## Build / verify

Every `lib/ide-workbench/*.sleela` is intended to pass `impl/build/sleela check`
at library parity, exactly like `lib/gui`. The runnable walkthrough is
`ide-workbench-demo.sleela`.

**SLeeLa — MEARVK LLC — 2026**
