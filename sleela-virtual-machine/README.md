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
