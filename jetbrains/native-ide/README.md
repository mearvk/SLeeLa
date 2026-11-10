# SLeeLa Native IDE — first GUI milestone

This is the native desktop IDE direction associated with `/jetbrains`. The
JetBrains source-acquisition scripts remain available as a separate research
and interoperability path; they are **not** a runtime dependency of this IDE.

## Objective

Build a compact, cross-platform SLeeLa desktop IDE for SLeeLa, C, C++, and Java.
The application UI is authored in SLeeLa on SleelaUI and uses the existing
native window / widget ABI. It does not embed IntelliJ IDEA, Kotlin IDE
infrastructure, or a JetBrains runtime.

## Existing foundation reused

- `lib/ide-workbench/SLIdeWorkbenchUI.sleela`: native workbench shell,
  project tree, notebook, status, loading state, event routing.
- `lib/ide-workbench/SLIdeEditor.sleela` and `SLIdeTabGroup.sleela`:
  editor/document and multi-tab models.
- `lib/user-interface/SLTextView.sleela`: SLeeLa-authored multi-line code
  editor with line numbers, caret, selection, editing operations and rendering.
- `lib/user-interface/SLTreeView.sleela`, `SLNotebook.sleela`,
  `SLStatusBar.sleela`, `SLButton.sleela`, `SLBox.sleela`: native controls.
- `lib/compiler`, `lib/nordshrift`, `lib/gui`: compiler and desktop service
  contracts.

## First GUI class set

| Class | Responsibility |
|---|---|
| `SLIdeMenuBar` | File/Edit/View/Navigate/Code/Run/Tools/Help menu groups and menu actions |
| `SLIdeToolbar` | New/Open/Save/Build/Run/Debug/Stop actions |
| `SLIdeLanguageBar` | SLeeLa/C/C++/Java language selection |
| `SLIdeWorkbenchUI` | Main window, project explorer, tabs, editor, diagnostics and status |

The first pass provides menu groups and actionable controls. File dialogs,
filesystem traversal, keybindings, language servers, debugger adapters and
compiler process execution remain service integrations rather than fake
button-side implementations.

## Layout

- Menu row and action toolbar at the top.
- Left project explorer.
- Center multi-document editor notebook.
- Bottom diagnostics / build output and status area.
- Language selector for SLeeLa, C, C++, and Java.

## Footprint policy

Keep the base application native and small. Do not ship four complete compiler
toolchains inside the IDE. Discover installed tools at runtime and enable
language services on demand:

- SLeeLa: repository compiler/runtime.
- C: discover `cc` / `clang` / `gcc`.
- C++: discover `c++` / `clang++` / `g++`.
- Java: discover JDK tools (`javac`, `java`, `jdb`).

The IDE's own installed footprint should be measured after native packaging;
the 6.2 GB comparison is a target to beat, not a verified SLeeLa package size.

## Build / run

The existing workbench GUI walkthrough is:

```sh
impl/build/sleela run lib/ide-workbench/ide-workbench-gui-demo.sleela
```

This is the starting point for the integrated native shell. Build and runtime
verification should be recorded separately from source creation.

**SLeeLa — MEARVK LLC — 2026**
