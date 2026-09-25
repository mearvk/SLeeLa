# Slecompiler™ API Surface

**Max Rupplin - MEARVK LLC - 2026**

Slecompiler™ is the native-binary analysis API within SLeeLa. The API is intended for applications, shared libraries, static archives, relocatable objects, kernel modules, drivers, and raw native artifacts.

## Core classes

| Class | Purpose |
|---|---|
| Artifact | Immutable input bytes, container, artifact class, architecture and identity |
| LibraryMetadata | ELF/PE/Mach-O dynamic-library identity and dependency evidence |
| KernelModuleMetadata | Linux .ko module metadata and dependency evidence |
| Decoder | Instruction decoding and operand recovery |
| Section | File/memory region description |
| Symbol | Named or inferred address |
| Import / Export | External and public interface records |
| Relocation | Relocation evidence |
| ControlFlowGraph | Basic blocks and edges |
| Analyzer | CFG/function/data-flow analysis |
| Module | SLIR analysis unit and artifact metadata |
| Function | Recovered function representation |
| SlirBlock | Intermediate basic block |
| Operation | Typed SLIR operation |
| Value | SSA-like value/type handle |
| Vm | Deterministic SLIR validation VM |
| VmTrap | Explicit VM failure state |

## Linux coverage

The Linux target model includes ELF executables, .so and versioned shared libraries, .o relocatable objects, .a archives, and .ko kernel modules. Dynamic-library analysis includes dependency relationships, SONAME, DT_NEEDED, dynamic symbols, symbol versions, PLT/GOT and relocation evidence, TLS, init/fini arrays, GNU build IDs, and section/program-header permissions when present.

For kernel modules, the API provides a place for vermagic, module identity, license/author/description, aliases, dependencies, exports, relocation evidence and other .modinfo/ELF evidence. It does not load or execute modules.

## Library-family analysis

A future LibraryGraph layer should connect executable imports to provider exports and connect shared objects to their DT_NEEDED dependencies, permitting whole-library-family analysis while preserving per-file provenance.

## Refactoring

Consumers can walk the SLIR graph, attach type hints and provenance, emit C/C++, generate API descriptions, and preserve a sidecar mapping from recovered source back to binary addresses.
