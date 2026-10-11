<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Senior — CompilerPipeline

**Goal:** build a maintainable compiler pipeline with typed intermediate stages, symbol validation, diagnostics, and backend separation.

Separate the implementation into SourceManager, Lexer, Parser, SymbolTable, SemanticAnalyzer, SLeeLaIR, Lowerer, ArtifactWriter, and DiagnosticReporter. Each component should have a narrow contract. Use the repository's actual frontend and instruction-set definitions as the source of truth; do not create a competing opcode list.

See [CompilerPipeline.sleela](CompilerPipeline.sleela) for a fail-closed orchestration scaffold.

## Milestones

1. Parse a documented subset.
2. Resolve scopes and reject duplicate declarations and unresolved names.
3. Type-check before lowering.
4. Validate IR invariants before backend emission.
5. Add deterministic serialization and golden-file tests.
6. Test malformed input, resource limits, disallowed output paths, and interrupted writes.

## Quality gates

- No backend call after frontend or semantic failure.
- Stable diagnostic codes and source spans where possible.
- Versioned IR format and deterministic output.
- Native compiler invocation is opt-in and uses an explicit allowlisted toolchain.