# SLeeLa IDE Architecture

## Principle

The primary SLeeLa IDE is a native desktop application authored in SLeeLa on
SleelaUI. It is a client of the SLeeLa compiler/runtime contracts and must not
create a second, incompatible definition of the language.

The IntelliJ Platform plugin is optional interoperability for users who already
have that platform installed. It is not the native application's runtime,
distribution, or build dependency.

## Native application pipeline

```
SLeeLa UI / Workbench
  -> Project tree + document tabs + SLTextView editor
  -> language profile (SLeeLa / C / C++ / Java)
  -> compiler and toolchain services
  -> diagnostics / build output
  -> run / debugger adapters
```

## Native layers

1. **Presentation** — SleelaUI window, menus, toolbar, project explorer,
   editor notebook, language selector, diagnostics, and status.
2. **Document model** — open documents, dirty state, tabs, project files, and
   editor buffers.
3. **Language** — SLeeLa compiler bridge; host-discovered C/C++ toolchains; JDK
   discovery and Java execution.
4. **Build and execution** — invokes installed tools and repository-supported
   commands; does not reimplement compiler behavior in the UI.
5. **Debugging** — connects to the SLeeLa debugger and appropriate native/JVM
   debugger adapters.
6. **Desktop bridge** — SLeeLa's native GUI ABI for Linux, Windows, and macOS.

## Optional IntelliJ integration

`ide/sleela-intellij` remains a separately built plugin. It may reuse the
language and project contracts, but the native application must not require
IntelliJ, Kotlin IDE infrastructure, or the JetBrains source tree.

## Non-goals

- replacing the SLeeLa compiler;
- inventing a second SLeeLa grammar;
- bundling entire C, C++, and Java toolchains in the base IDE;
- hiding compiler or runtime failures behind IDE success states;
- requiring a multi-gigabyte third-party IDE installation to edit SLeeLa.
