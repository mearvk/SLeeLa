# SLeeLa Decompiler API Surface

**Max Rupplin - MEARVK LLC - 2026**

The SLeeLa source API exposes the decompiler as a normal C++ subsystem.

## Core classes

| Class | Purpose |
|---|---|
| Artifact | Immutable input bytes, format, architecture, identity |
| Decoder | Instruction decoding and operand recovery |
| Section | File/memory region description |
| Symbol | Named or inferred address |
| Import / Export | External and public interface records |
| Relocation | Relocation evidence |
| ControlFlowGraph | Basic blocks and edges |
| Analyzer | CFG/function/data-flow analysis |
| Module | SLIR analysis unit |
| Function | Recovered function representation |
| SlirBlock | Intermediate basic block |
| Operation | Typed SLIR operation |
| Value | SSA-like value/type handle |
| Vm | Deterministic SLIR validation VM |
| VmTrap | Explicit VM failure state |

## Native artifact coverage

The API is designed for DLLs, Windows SYS drivers, EXEs, ELF shared objects/modules, Mach-O libraries and raw images. Driver analysis is static and read-only.

## Refactoring

Consumers can walk the SLIR graph, attach type hints and provenance, emit C/C++, generate API descriptions, and preserve a sidecar mapping from recovered source back to binary addresses.

## ABI direction

A future C ABI wrapper should expose opaque handles so SLeeLa applications written in C, C++, Java, Python and other FFI-capable languages can consume the same analysis engine without depending on C++ layout.