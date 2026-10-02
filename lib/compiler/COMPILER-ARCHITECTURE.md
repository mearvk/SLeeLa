<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-compiler-logo-001.jpeg" alt="SLeeLa Compiler" width="100%"></p>

# SLeeLa Compiler Architecture

## Design

The compiler is a staged, source-driven pipeline. Each stage has a declared SLeeLa object and a native implementation boundary.

### Stages

- Source: load and preserve SLeeLa source identity.
- Lexer: convert source to tokens with source locations.
- Parser: construct a syntax tree and declaration inventory.
- Symbols: resolve names, scopes, imports, classes, functions, and VM-facing symbols.
- Semantics: validate declarations, types, capabilities, and dependencies.
- IR: create target-neutral SLeeLa IR.
- Lowering: translate IR to VM-ready operations without changing language meaning.
- Code generation: emit SLVM or SLJVM artifacts.
- Diagnostics: retain errors, warnings, dependencies, and emitted module status.

### Single pipeline

`.sleela -> frontend -> IR -> VM lowering -> SLVM/SLJVM`

There is no second compiler language or hidden native source language.

### VM readiness

The compiler must reject an artifact when required target capabilities, security policy, memory policy, certificate requirements, or declared dependencies are missing. It must report the reason rather than silently weakening the target.

### Relationship to /lib/vm

`/lib/compiler` implements compilation.

`/lib/vm` implements the target model, Compiler Manager completeness review, memory/security management, linking, challenge, and reports services.

The compiler may ask the VM package whether a requested target feature is valid; it may not grant itself VM authority.

### Output

A compile operation should identify:

- source modules
- symbols
- object/category counts
- semantic status
- dependencies
- capabilities
- target architecture
- VM profile
- generated artifact kind
- diagnostics

A successful result means the declared pipeline completed. It does not mean an undeclared feature was added.
