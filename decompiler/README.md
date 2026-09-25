# Slecompiler™

**Max Rupplin - MEARVK LLC - 2026**

Slecompiler™ is the SLeeLa native-binary analysis, decompilation, library inspection, driver investigation, and refactoring subsystem.

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
- `api/decompiler/API.html` — browser-oriented API summary.

## Analysis pipeline

artifact → container → symbols/metadata → decoder → instructions → CFG → functions → SLIR → analysis → C/C++/report

For libraries, the pipeline additionally models dependency relationships and public interfaces so collections of .so, .a, .o, or .ko files can be analyzed as a software family rather than isolated byte streams.

## Safety and evidence

Analysis is read-only by default. Slecompiler™ never executes an analyzed artifact as part of parsing or lifting. Driver and kernel-module images are treated as untrusted input. Kernel-mode loading, device I/O, memory writes, and execution of recovered native code are outside the default operation.

Use the subsystem only with software you are authorized to inspect.
