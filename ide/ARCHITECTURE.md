# SLeeLa IDE Architecture

## Principle
The IDE is a client of SLeeLa compiler and runtime contracts. It must not create a second, incompatible definition of the language.

## Pipeline
```
Editor -> IntelliJ Lexer / Parser / PSI -> SLeeLa semantic bridge
       -> Compiler diagnostics / symbol model -> Build and Run bridge
       -> Runtime -> Debugger / DAP bridge
```

## Layers
1. **Platform** — editor, project services, indexing, actions, inspections, refactoring, tests, debugger extension points.
2. **Language** — token, AST, PSI, references, type and diagnostic contracts.
3. **Semantic bridge** — maps IDE positions to impl/frontend lexer, parser, AST and semantic analysis.
4. **Project model** — SLeeLa, C, C++, Java sources, generated roots, dependencies, native/JVM tools and tests.
5. **Execution** — invokes repository-supported commands rather than reproducing compiler behavior in Kotlin.
6. **Debugging** — maps IntelliJ actions to the existing SLeeLa debugger model and, where appropriate, DAP.

## Non-goals
- replacing the SLeeLa compiler;
- inventing a second SLeeLa grammar;
- claiming C/C++/Java language support beyond the host IDE/tooling;
- hiding compiler or runtime failures behind IDE success states.
