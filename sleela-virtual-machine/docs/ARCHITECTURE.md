# SLVM Architecture

The SLVM is the execution core of SLeeLa and is deliberately separate from `runtime/`.

The initial kernel contains VM state, a program counter, operand stack, opcode decoding, arithmetic/comparison operations, constants, control flow, and halt/return behavior.

The initial instruction set is intentionally a foundation rather than the final SLeeLa instruction set. Compiler/loader integration, call frames, locals, object references, exceptions, and richer values will be added against an explicit instruction-set contract.

Copyright (c) Max Rupplin - MEARVK LLC - 2026


## Command-Line relationship

The `sleela` CLI has two `.sleela` input paths that converge on the same native C Core SLVM:

```text
textual .sleela
    → authoritative Lexer/Parser/compiler
    → Core representation
    → SLVM

persistent .sleela Core artifact
    → ABI/artifact validation and loader
    → SLVM
```

`./impl/build/sleela run program.sleela` handles either representation. For textual source it compiles in memory; for a persistent artifact it loads the already-compiled Core representation. The word **native** refers to the C/C++ SLeeLa executable and native Core/runtime boundary, not to a separate interpreter that bypasses SLVM.

See `COMMAND-LINE-EXECUTION.md` for the user-facing command contract.
