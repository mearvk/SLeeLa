<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa IDE Integration (IntelliJ Platform plugin)

First-class **SLeeLa** language support for the IntelliJ Platform, with
mixed-language project integration for C, C++, and Java.

SLeeLa is a Java-like native systems language that compiles to bytecode on a
Turing-complete C/C++ execution core. This plugin keeps the native SLeeLa
compiler (**Sleelvac™**) authoritative: syntax, parsing, and diagnostics come
from the real compiler, never a second grammar (see
[`../SLEELA.IDE.CONTRACT.md`](../SLEELA.IDE.CONTRACT.md)).

## Features (v0.3.0-dev)

**Editor**
- Recognizes `.sleela` source files (branded Slick-Black / brand-green icon).
- Syntax highlighting derived from the normative `SLEELA.syntax` grammar
  (keywords, types, strings, numbers, comments, the `#sleela` version directive).
- Live diagnostics from the native `sleela check` compiler bridge
  ([`NativeSleelaCompilerBridge`](src/main/kotlin/com/mearvk/sleela/ide/NativeSleelaCompilerBridge.kt)).
- Line (`//`) and block (`/* */`) comment support.
- Customizable color scheme (Settings → Editor → Color Scheme → SLeeLa).

**Developer actions (buttons, menus, shortcuts)**
- A top-level **SLeeLa** main menu, an editor context menu, a project-view
  context menu, and main-toolbar buttons — all branded.
- **Run SLeeLa File** (`Ctrl+Shift+F10`) → `sleela run`
- **Check SLeeLa File** (`Ctrl+Shift+K`) → `sleela check`
- **Build SLeeLa Artifact** → `sleela compile … -o …`
- **Transpile with Nordshrift** → `sleela nordshrift --emit --target=java`
- **SLeeLa Compiler Version** → `sleela version` (balloon)
- **Language Syntax Reference** / **SLeeLa on GitHub** doc links.

**Run configurations**
- A first-class **SLeeLa** run configuration type (file + program arguments).
- A context producer so the gutter Run icon and right-click → Run work with
  zero setup — the smooth path for novices.

**Tool window & onboarding**
- A dockable **SLeeLa** tool window with Run/Check/Build/Transpile buttons and
  a live compiler-status readout.
- **New → SLeeLa File** with Class / Program (with `main`) / Empty templates.
- Balloon notifications under the **SLeeLa** group (e.g. missing compiler).

## Architecture

```
Editor
  └─ SleelaLexer ──► compiler bridge tokens  (fallback: SleelaFallbackLexer, from SLEELA.syntax)
  └─ SleelaDiagnosticAnnotator ──► NativeSleelaCompilerBridge ──► `sleela check`
                                                                    └─ SleelaDiagnosticParser
```

The IDE never reimplements SLeeLa semantics. When the native compiler is not
present, diagnostics report that fact rather than claiming the source is clean
(SLEELA.IDE.CONTRACT.md, "Failure rule").

## Requirements

- **JDK 21** (the build toolchain is pinned to 21; the IntelliJ Platform Gradle
  Plugin 2.x and its bundled Kotlin compiler reject JDK 25+).
- **Internet access** on first build to download the IntelliJ Platform SDK
  (IC 2026.1), Kotlin, and the IntelliJ Platform Gradle Plugin.
- The native SLeeLa compiler built at `impl/build/sleela`
  (`make -C impl`) for live diagnostics. Override its location with the
  `SLEELA_COMPILER` environment variable if needed.

## Building

The Gradle **wrapper jar** is not committed (it is a binary that Gradle
materializes on demand). On a networked machine, generate it once:

```sh
cd ide/sleela-intellij
gradle wrapper --gradle-version 9.8.0   # creates gradle/wrapper/gradle-wrapper.jar
```

Then build and run a sandbox IDE with the plugin:

```sh
./gradlew build          # compile + test
./gradlew runIde         # launch IntelliJ with the SLeeLa plugin loaded
./gradlew buildPlugin     # produce the installable plugin ZIP under build/distributions/
```

If `./gradlew` is unavailable before the wrapper jar exists, use a local
Gradle 9.x: `gradle build` (the IntelliJ Platform Gradle Plugin 2.19.0 requires
Gradle 9.0.0 or newer).

## Status

See [`../VERSION.md`](../VERSION.md) for the IDE integration roadmap. Current:
**0.3.0-dev** — compiler diagnostics, highlighting, file-type/branding.

**SLeeLa — MEARVK LLC — 2026**
