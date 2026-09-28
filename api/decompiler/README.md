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

`LibraryGraph` connects analyzed artifacts and currently provides conservative dependency/provider evidence. It should be treated as an evidence graph, not a complete dynamic-loader or ABI simulation.

## Refactoring

Consumers can walk the SLIR graph, attach type hints and provenance, emit C/C++, generate API descriptions, and preserve a sidecar mapping from recovered source back to binary addresses.


## API exemplars

Runnable C++ examples are maintained in `decompiler/examples/`:

- `api_inspect.cpp` — artifact identity and native interface inspection.
- `api_decode_cfg.cpp` — instruction decoding and CFG construction.
- `api_library_metadata.cpp` — shared-library and kernel-module metadata.
- `api_library_graph.cpp` — library-family graph construction.
- `api_slir_vm.cpp` — SLIR construction and bounded VM validation.

The examples are enabled by default by the `SLEE_LA_BUILD_API_EXAMPLES` CMake option.

## Product tutorial

See:

`decompiler/docs/TUTORIAL.md`

The tutorial covers installation/building on Linux, Windows 10+, and macOS, the C++ API layers, artifact inspection, native decoding, CFG analysis, library and kernel-module metadata, LibraryGraph, SLIR, the bounded VM, function recovery, identity digests, CLI use, reproducible analysis records, and current product limitations.
