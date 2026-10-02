# SLeeLa Source → SLVM Execution Map

The existing SLeeLa frontend remains authoritative. The new `/sleela-virtual-machine` effort does not create a parallel grammar or semantic compiler.

Pipeline:

.sleela source → Lexer → Parser → authoritative AST → semantic/compiler lowering → SLeeLa Core VM representation → artifact or execution → SLVM runtime/security layers

## Authoritative frontend

`impl/frontend` supplies the lexer, parser, AST, semantic compiler, and artifact compiler boundary. `compiler.cpp` lowers the AST into the existing C Core instruction representation. `artifact.cpp` provides the persistent runnable artifact path.

## Source constructs represented

- imports, annotations, structs
- classes, interfaces, enums, records and Java metadata
- fields, methods, constructors and parameters
- locals, assignment, print, return and control flow
- integer, double, boolean, string and null values
- variables, unary/binary expressions, calls, method calls, `new`, member access
- protected/static semantics and bounded struct handles

## Core execution map

| SLeeLa area | Core operations |
|---|---|
| values/stack | CONST, POP, DUP |
| variables | LOADG, STOREG, LOADL, STOREL |
| arithmetic | ADD, SUB, MUL, DIV, MOD, NEG |
| comparison/boolean | EQ, NE, LT, LE, GT, GE, AND, OR, NOT |
| control flow | JMP, JMPF |
| functions | CALL, RET |
| output/concurrency | PRINT, SPAWN, JOINALL |
| synchronization | LOCK, UNLOCK, SEND, RECV |
| sockets | LISTEN, ACCEPT, CONNECT, SOCKREAD, SOCKWRITE, SOCKCLOSE |
| IPC/files | PIPE, PIPEPEER, FIFO_MK, FILEOPEN, FILEREAD, FILEWRITE, FILECLOSE, FILEUNLINK |
| time | TIME_UTC_MS, TIME_UTC_NS, TIME_MONO_NS, TIME_PRECISION_MS, TIME_LOCATION, TIME_HTTP_DATE, TIME_JSON, TIME_NTP, TIME_SET_LOCATION |
| structs | NEWSTRUCT, GETFIELD, SETFIELD, STRUCTPACK, STRUCTUNPACK |
| Synchro | SYN_OPEN, SYN_DISPATCH, SYN_STAT, SYN_REPORT, SYN_CLOSE |
| Munction | MUN_START, MUN_CONNECT, MUN_ENABLE, MUN_SEND, MUN_THATCH, MUN_CONSUME, MUN_LATCH, MUN_RECEPTION, MUN_CLOSE |
| Best-of | BEST_NEW, BEST_WEIGHT, BEST_MINVER, BEST_BUDGET, BEST_CAND, BEST_RECORD, BEST_SCORE, BEST_BEST, BEST_STAT, BEST_CHOICE, BEST_REPORT, BEST_ARCH, BEST_ARCH_STATE, BEST_CLOSE |
| audio | AUDIO_NEW, AUDIO_ADD, AUDIO_CONTROLS, AUDIO_VALIDATE, AUDIO_RENDER, AUDIO_CLOSE, AUDIO_PLATFORM |

## Artifact contract

The existing `.sleela` artifact is compiled Core execution data. It contains code, constants, globals, functions, and struct layout information. Its loader validates opcode ranges, indexes, function entries, struct counts, and ABI constraints before execution.

The new VM must therefore preserve the existing Core VM/artifact ABI as its compatibility target instead of substituting the small bootstrap opcode set.

## Adapter

`slvm_sleela.hpp/.cpp` provides the VM effort with three entry points:

- `execute_source()` — real lexer → parser → AST → compiler → Core VM.
- `execute_source_file()` — same authoritative path from a `.sleela` file.
- `execute_artifact()` — validates and executes a compiled artifact.

## Completeness invariant

A source construct is not complete until its lexer/tokenization, AST, semantic validation, compiler lowering, Core operation, artifact handling, security/capability boundary, and VM execution path are all mapped. This prevents accepted SLeeLa syntax from becoming an unmapped runtime operation.