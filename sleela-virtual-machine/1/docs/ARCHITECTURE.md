# SLVM Architecture

The SLVM is the execution core of SLeeLa. It is deliberately separated from the existing `runtime/` support layer.

## State

The first implementation contains:

- code buffer
- code size
- program counter
- operand stack
- stack capacity
- halt state

## Instruction Model

Instructions begin with a one-byte opcode. Instructions requiring operands encode those operands immediately after the opcode.

The initial prototype supports:

- NOP / HALT
- CONST / POP / DUP
- ADD / SUB / MUL / DIV / NEG
- EQ / LT / GT
- JUMP / JUMP_IF_FALSE
- RETURN

This is an initial execution kernel, not yet the complete SLeeLa language instruction set.

## Integration

Compiler and loader integration will be added only after the instruction representation is established and tested. Existing `runtime/` services remain reusable dependencies.

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


## Memory Manager Security

Managed memory is a security boundary. The default 512 MiB ceiling is enforced before GC allocation, with policy states for normal use, pressure, restricted operation, and denial. Large or burst allocations are controlled independently of OS capabilities. Reclaimed bytes are returned to memory-security accounting so garbage collection and security policy remain synchronized.

See MEMORY-SECURITY.md.


## Library Object Coverage

The VM execution boundary is designed around the complete /lib class vocabulary rather than a fixed prototype object list. Source classes enter through the authoritative SLeeLa compiler and become VM-managed values, native capability handles, or authenticated broker proxies. See LIB-SLVM-COVERAGE.md.

## Remote JVM / JavaFX

The SLVM may delegate presentation objects to a remote JVM through the SLVM↔JVM Object Broker. JavaFX owns GUI thread confinement; SLVM owns program semantics, authorization, memory/security policy, and object identity. See JVM-OBJECT-BROKER-PROTOCOL.md.

## Official Observer

The Official Observer layer can attach to function entry, parameters, returns, object lifecycle, memory, I/O, broker events, and certificate events. It is read-only by default and cannot grant capabilities. Secret values are redacted before observer callbacks. See OFFICIAL-OBSERVER-SECURITY.md.
