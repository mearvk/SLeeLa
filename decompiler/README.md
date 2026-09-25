# SLeeLa Decompiler

**Max Rupplin - MEARVK LLC - 2026**

SLeeLa Decompiler is an analysis and refactoring subsystem for legally obtained software artifacts. It is designed to preserve evidence while producing normalized representations of native binaries for investigation, documentation, interoperability, maintenance, and technology recovery.

## Scope

The subsystem accepts native artifacts such as Windows PE files (including DLL and SYS/driver images), ELF executables/shared objects/modules, and raw binary images. It records:

- container and architecture metadata;
- sections, segments, imports, exports, relocations, symbols and strings;
- instruction bytes and normalized instruction records;
- control-flow graphs and basic blocks;
- function candidates and call relationships;
- data references and memory regions;
- compiler/runtime fingerprints when evidence supports them;
- a SLeeLa intermediate representation (SLIR);
- deterministic JSON and text reports;
- hashes and provenance for every input and generated artifact.

The decompiler does **not** claim to reconstruct original source exactly. Names, types, comments, macros, templates, build flags and source layout are generally unavailable unless retained in symbols or metadata.

## Layout

- `include/sleela/decompiler/` — public C++ API and class definitions.
- `src/` — implementation.
- `cli/` — command-line entry point.
- `tests/` — parser/IR/VM tests and fixtures.
- `docs/` — descriptor and API documentation.
- `output/` — optional analysis output; generated files should not be committed.

## Safety and evidence

Analysis is read-only by default. The engine never executes an analyzed artifact as part of parsing or lifting. Driver images are treated as untrusted input. Kernel-mode loading, device I/O, memory writes, or execution of recovered code are outside the decompiler's default operation.

Use the subsystem only with software you are authorized to inspect.

## Build

```sh
cmake -S decompiler -B decompiler/build
cmake --build decompiler/build --parallel
```

The resulting `sleela-decompiler` command supports `inspect`, `disassemble`, `lift`, `cfg`, `functions`, and `report` operations.
