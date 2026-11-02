<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">





# Slecompiler™

**Max Rupplin - MEARVK LLC - 2026**

Slecompiler™ is the SLeeLa native-binary analysis, decompilation, library inspection, driver investigation, and refactoring subsystem.

## Build

Requirements: CMake 3.20 or newer, a C++20-capable compiler, and GNU Make (for the Makefile wrapper).

From this directory:

```sh
make                 # Configure and build
make test            # Build and run the registered tests
make check           # Alias for make test
make stage           # Install into ./build/stage
make clean           # Remove generated build files
```

Useful overrides:

```sh
make BUILD_TYPE=Debug
make JOBS=4
make BUILD_EXAMPLES=OFF
make GENERATOR=Ninja
make PREFIX=/your/staging/path stage
```

The same CMake project can be built directly without GNU Make:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build --output-on-failure
```

The wrapper and CMake project target Linux, macOS, and Windows toolchains. On Windows, use a shell/environment that provides GNU Make and CMake, or invoke CMake directly. A build has not been validated on every platform merely by providing these build definitions.

## Linux-first native coverage

Slecompiler™ treats Linux native software as a first-class target, including:

- ELF executables and PIE programs;
- ELF shared objects (.so and versioned .so.*);
- GNU/Linux system libraries such as libc, libm, libdl, libpthread compatibility interfaces, libstdc++, and other ELF DSOs;
- plugin and extension libraries loaded through dynamic-loader mechanisms;
- static archives (.a) as collections of object files;
- relocatable ELF objects (.o);
- Linux kernel modules (.ko);
- compressed or packaged kernel modules when safely identifiable and decompressed for analysis;
- firmware and raw binary images;
- PE/COFF and Mach-O libraries and drivers where applicable.

The API models both individual artifacts and library families: dependencies, imported/exported symbols, relocation records, ABI clues, calling conventions, symbol versions, build identifiers, section permissions, and provenance can be represented as related records.

## Coverage status

Coverage is described honestly as three distinct capability levels. A format may be
*detected* (identity and architecture recognized), *parsed* (sections, symbols, imports,
exports, and metadata recovered), and/or *disassembled* (machine code lifted to
instructions and a control-flow graph).

| Container | Detected | Parsed | Disassembled |
|---|---|---|---|
| ELF (exe, PIE, `.so`/`.so.*`, `.o`, `.ko`) | yes | full: sections, segments, symbols, imports, exports, relocations, dynamic metadata, symbol versions, build-id, kernel-module `.modinfo` | x86, x86-64 (control flow); ARM64 (control flow) |
| PE/COFF (`.exe`, `.dll`, `.sys`) | yes | sections, import directory (DLL deps + named/ordinal imports), export name table, base-relocation presence, image base, entry point, subsystem/driver class | x86, x86-64 (control flow); ARM64 (control flow) |
| Mach-O thin (32/64-bit) and FAT/universal | yes | load commands: sections, dylib dependencies, symbol table (imports/exports); first analyzable FAT slice lifted | x86, x86-64 (control flow); ARM64 (control flow) |
| GNU static archive (`.a`) | yes | member enumeration with per-member format identification | per-member (ELF members) |
| Raw / firmware | yes | recorded as an opaque byte artifact with SHA-256 provenance | linear byte view |

Disassembly recovers the control-flow-relevant instruction set (branches, calls,
returns, conditional branches, indirect transfers) on x86/x86-64 and AArch64, which is
what the CFG, function-candidate, and SLIR stages consume. Instruction encodings outside
that subset are represented conservatively (x86 `db`, AArch64 `.inst`) with low confidence
rather than guessed, preserving the provenance guarantee. Extending the decoded opcode set
and adding a 32-bit ARM (A32/T32) decoder are the natural next increments.

## Native artifact classes

1. **Executable** — a native program image.
2. **DynamicLibrary** — an ELF DSO, PE DLL, Mach-O dylib, or equivalent loadable library.
3. **StaticArchive** — an archive containing relocatable object members.
4. **RelocatableObject** — an object intended for later linking.
5. **KernelModule** — Linux .ko and comparable kernel-loadable ELF artifacts.
6. **RawArtifact** — firmware, dumps, blobs, or other data whose container is unknown.

For Linux .ko files, Slecompiler™ records kernel-module evidence such as module name, vermagic when present, exported/imported symbols, section layout, relocation information, module metadata strings, license/author/description fields when present, dependency metadata, and kernel-version/build identifiers. This is static evidence; Slecompiler™ does not load the module into a kernel.

## Documentation and examples

The product documentation is divided into the API surface, architecture, terminology, format notes, and the product tutorial. Runnable API exemplars are maintained under `decompiler/examples/` and are built by default through `SLEE_LA_BUILD_API_EXAMPLES`.

Start with:

- `decompiler/docs/TUTORIAL.md` — build and API tutorial;
- `decompiler/API.md` — C++ API layer map;
- `decompiler/TERMINOLOGY.md` — Slecompiler terminology;
- `decompiler/ARCHITECTURE.md` — analysis architecture;
- `api/sleela/2.1/decompiler/API.html` — browser-oriented API summary.

## Analysis pipeline

artifact → container → symbols/metadata → decoder → instructions → CFG → functions → SLIR → analysis → C/C++/report

For libraries, the pipeline additionally models dependency relationships and public interfaces so collections of .so, .a, .o, or .ko files can be analyzed as a software family rather than isolated byte streams.

## Safety and evidence

Analysis is read-only by default. Slecompiler™ never executes an analyzed artifact as part of parsing or lifting. Driver and kernel-module images are treated as untrusted input. Kernel-mode loading, device I/O, memory writes, and execution of recovered native code are outside the default operation.

Use the subsystem only with software you are authorized to inspect.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
