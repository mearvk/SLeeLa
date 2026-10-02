<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine

The SLeeLa Virtual Machine (SLVM) is the execution engine for SLeeLa programs.

This directory is the dedicated home for the VM itself. It is intentionally separate from `runtime/`, which provides supporting runtime services such as garbage collection, security supervision, runtime parameters, and Java interoperability.

## Execution Pipeline

`SLeeLa source → compiler → SLVM bytecode/instructions → SLVM execution engine → runtime services / native system`

## Initial Responsibilities

- VM state and lifecycle
- instruction and opcode definitions
- program counter / instruction pointer
- operand stack
- call frames and locals
- constants and values
- instruction decoding and dispatch
- arithmetic, comparison, branching, loading and storing
- function/method invocation and return
- exception/error propagation
- integration with garbage collection and security services
- controlled native/OS boundaries
- compiler and loader integration

## Layout

- `include/` — public VM headers
- `src/` — VM implementation
- `tests/` — execution tests
- `docs/` — architecture and instruction-set documentation
- `build/` — platform build entry points

## Status

This establishes the dedicated SLVM implementation boundary. The initial execution kernel is intentionally small and will be expanded as the compiler and loader instruction representation is integrated.

Copyright (c) Max Rupplin - MEARVK LLC - 2026

## Command-Line .sleela execution

The repository's `sleela` CLI supports two `.sleela` input forms:

1. **Textual source:** `sleela run program.sleela` uses the authoritative frontend and compiles the source in memory before executing the Core representation in SLVM.
2. **Persistent artifact:** `sleela compile program.sleela -o program.sleela` creates a runnable Core artifact; `sleela run program.sleela` detects and loads that artifact directly into SLVM.

This is a single execution architecture. Native SLeeLa means the C/C++ SLeeLa toolchain and Core are used directly; SLVM is the execution engine underneath both command-line source and persistent-artifact execution. There is no parallel SLeeLa grammar or second language interpreter.

See `docs/COMMAND-LINE-EXECUTION.md` and `docs/SLEELA-SOURCE-VM-MAP.md` for the source/artifact boundary and full mapping.
