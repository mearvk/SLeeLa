# SLeeLa IDE Contract

Version: 0.1.0-dev

## Authoritative sources
- LANGUAGE.SPEC.md
- impl/frontend/lexer.*
- impl/frontend/parser.*
- impl/frontend/ast.h
- impl/frontend/semantic.*
- COMPILER.md
- ABI.md and impl/core/sleela_core.h
- /lib
- /debugger

## Required services
| Service | Contract |
|---|---|
| Lexing | Use the SLeeLa token inventory |
| Parsing | Preserve compiler grammar and source locations |
| PSI | Represent declarations, references, expressions, types and modules |
| Symbols | Resolve source plus indexed /lib objects |
| Types | Consume compiler semantic results |
| Diagnostics | Preserve category, severity, range, message and origin |
| Formatting | Never alter semantics |
| Build | Invoke repository build contract |
| Run | Invoke repository runtime contract |
| Test | Discover supported test suites |
| Debug | Map source locations to debugger locations |
| Versioning | Respect #sleela language-version rules |

Compiler positions are one-based at the boundary; IDE offsets must convert explicitly.

## Failure rule
Compiler, build, runtime and debugger failures remain IDE-visible failures. Adapters must not convert unsuccessful operations into successful task results.

## Mixed-language rule
C, C++, and Java language semantics remain delegated to their host tooling. SLeeLa tooling owns source discovery, dependency metadata, native/JVM invocation, ABI metadata, and declared interop navigation.
