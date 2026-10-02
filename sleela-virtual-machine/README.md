<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine

The SLeeLa Virtual Machine (SLVM) is the execution engine for SLeeLa programs.

This directory is the dedicated home for the VM itself. It is intentionally separate from `runtime/`, which provides supporting runtime services such as garbage collection, security supervision, runtime parameters, and Java interoperability.

## Execution Pipeline

`SLeeLa source → compiler → SLVM bytecode/instructions → SLVM execution engine → runtime services / native system`

## Initial Responsibilities

The SLVM will provide:

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
- integration with the existing garbage collector
- a lazy, configurable memory budget with a 512 MiB default
- GC-backed managed content allocation without eagerly reserving 512 MiB
- integration with the existing security supervisor
- controlled native/OS boundaries
- integration points for the SLeeLa compiler and loader
- testable, deterministic execution primitives

## Directory Layout

- `include/` — public VM headers
- `src/` — VM implementation
- `tests/` — VM execution tests
- `docs/` — architecture and instruction-set documentation
- `build/` — platform build entry points

## Relationship to runtime/

The VM is the execution engine. The existing `runtime/` directory remains the supporting runtime-services layer. The VM may call those services, but the two directories should not be conflated.

## Status

This directory establishes the dedicated SLVM implementation boundary. The VM instruction set and execution core will be developed here and connected to the compiler and loader deliberately rather than replacing existing runtime services.

Copyright (c) Max Rupplin - MEARVK LLC - 2026

## Command-Line .sleela execution

The repository's `sleela` CLI supports two `.sleela` input forms:

1. **Textual source:** `sleela run program.sleela` uses the authoritative frontend and compiles the source in memory before executing the Core representation in SLVM.
2. **Persistent artifact:** `sleela compile program.sleela -o program.sleela` creates a runnable Core artifact; `sleela run program.sleela` detects and loads that artifact directly into SLVM.

This is a single execution architecture. Native SLeeLa means the C/C++ SLeeLa toolchain and Core are used directly; SLVM is the execution engine underneath both command-line source and persistent-artifact execution. There is no parallel SLeeLa grammar or second language interpreter.

See `docs/COMMAND-LINE-EXECUTION.md` and `docs/SLEELA-SOURCE-VM-MAP.md` for the source/artifact boundary and full mapping.
