# SLeeLa Workbench — IntelliJ UI wiring

This is the **host-side UI** that implements the `lib/ide-workbench` contracts
inside the IntelliJ Platform plugin. It is the "presentation" half of the
authority split from `lib/gui/ARCHITECTURE.md`: the SLeeLa classes own the IDE's
*meaning*; this Kotlin/Swing layer renders it and performs the real OS/compiler
work.

## Where it lives

```
ide/sleela-intellij/src/main/kotlin/com/mearvk/sleela/ide/workbench/
  SleelaWorkbenchModel.kt     host impl of SLIdeEditor / SLIdeTabGroup /
                              SLIdeDiagnosticsPanel + language routing
  SleelaWorkbenchService.kt   SLIdeWorkbench.compileActive loop over the real
                              `sleela` toolchain (SST Nordshrit for .sst)
  SleelaWorkbenchPanel.kt     the Swing UI (tree + tabs + problems + commands)
toolwindow/SleelaToolWindowFactory.kt   hosts the panel as the "Workbench" tab
src/test/.../workbench/SleelaWorkbenchModelTest.kt   model unit tests
```

## Contract → UI mapping

| `lib/ide-workbench` class | Host UI element |
|---|---|
| `SLIdeProjectTree` | the **Project** tree (real project files, source-filtered) |
| `SLIdeTabGroup` | the **Open files** list, with `*` dirty markers |
| `SLIdeDiagnosticsPanel` | the **Problems** list (click to jump to line/col) |
| `SLIdeCommandRegistry` | the **Open / Compile / Save / Close** buttons |
| `SLIdeEditor` | one `WorkbenchEditor` per open file; the real text comes from the IntelliJ document |
| `SLIdeWorkbench` | `SleelaWorkbenchPanel` composes all of the above |

## The edit → compile → save loop

1. **Open** — double-click a tree file (or press *Open*). The file opens both in
   a workbench tab and in the real IntelliJ editor so the worker can type.
2. **Edit** — typing happens in the IntelliJ editor; the workbench reads the live
   document text at compile time, so unsaved edits are honored.
3. **Compile** — `SleelaWorkbenchService.compile`:
   - `.sst` → `sleela nordshrift --emit --target=sleela …` (**SST Nordshrit**),
     then `sleela check …`;
   - `.sleela` → `sleela check …` directly.
   Output is parsed by the existing `SleelaDiagnosticParser`; any ERROR fails the
   run (the `SLEELA.IDE.CONTRACT.md` failure rule).
4. **Save** — persists documents and marks the tab clean.

## The 1..14 document watch

`lib/gui`'s `SLGuiDocumentListener` is hard-bounded to **1..14** watched files.
The workbench may have many more tabs open, so `WorkbenchTabGroup.watchSetSize()`
caps the OS-watch registration at 14 while the open-file count stays unbounded —
the bound is never violated. The status bar shows `watch set N/14`.

## Building / testing

Requires network access for the IntelliJ Platform SDK (Gradle):

```sh
cd ide/sleela-intellij
./gradlew test            # runs SleelaWorkbenchModelTest (no SDK runtime needed)
./gradlew buildPlugin     # full plugin build
```

> The compile commands invoke the authoritative native compiler located by
> `SleelaCommandRunner` (`impl/build/sleela`, or `$SLEELA_COMPILER`). When the
> compiler is absent, Compile reports a single ERROR rather than silently
> succeeding.

**SLeeLa — MEARVK LLC — 2026**
