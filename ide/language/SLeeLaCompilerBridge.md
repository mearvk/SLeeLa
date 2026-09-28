# SLeeLa Compiler Bridge

Version: 0.2.0-dev

The compiler bridge is the authoritative boundary between the IntelliJ integration and SLeeLa itself.

## Pipeline

`.sleela source` → SLeeLa lexer → SLeeLa parser → SLeeLa AST → semantic analysis → compiler diagnostics

The IntelliJ layer consumes those compiler results and adapts them to PSI, references, indexes, inspections, completion, and navigation.

## Rules
- Do not create an independent language meaning model in the IDE.
- Do not silently accept syntax rejected by the compiler.
- Do not silently reject syntax accepted by the compiler.
- Preserve compiler source offsets and line/column locations.
- Preserve compiler diagnostic severity and diagnostic identity where available.
- Keep compiler-version information with cached IDE analysis.

## Bridge operations

`lex(source, file)`
`parse(source, file)`
`analyze(ast, project)`
`diagnostics(source, file, project)`
`index(ast, file, project)`

The implementation may call native C++, a command-line compiler service, or an in-process library depending on the existing SLeeLa build. The adapter must not assume a new compiler implementation.

**Max Rupplin — MEARVK LLC — 2026**