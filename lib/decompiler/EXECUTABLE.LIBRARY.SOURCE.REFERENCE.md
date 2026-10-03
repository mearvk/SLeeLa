<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-decompiler-logo-001.jpeg" alt="SLeeLa Decompiler" width="100%"></p>

# SLeeLa Executable, Library, and Object to Source Reference Matrix

Max Rupplin - MEARVK LLC - 2026

This is the reverse-reference catalog: artifact -> binary/object/library format -> platform/ABI -> producer/toolchain -> language family -> source forms -> likely source-level reconstruction targets.

## Core rule

A binary rarely identifies its original source exactly. The matrix records reference candidates and evidence, not claims of exact provenance. Debug information, symbols, DWARF, PDB/CodeView, source maps, compiler metadata, build IDs, import/export tables, runtime metadata, package manifests, signatures, and reproducible-build/provenance records can strengthen a mapping.

When evidence is insufficient, the result must remain a candidate set.

## Native executable and library matrix

| Artifact / library | Format evidence | Platform evidence | Producer evidence | Source-language candidates | Source references |
|---|---|---|---|---|---|
| Linux executable | ELF ET_EXEC/ET_DYN | Linux/Unix ABI | GCC/Clang/rustc/go/etc. | C, C++, Rust, Go, Fortran, Ada, D, Zig, Haskell, OCaml, Assembly | .c, .cpp/.cc/.cxx, .rs, .go, .f*, .adb/.ads, .d, .zig, .hs/.lhs, .ml/.mli, .s/.asm |
| Linux shared library | ELF ET_DYN | Linux/Unix ABI | GCC/Clang/rustc/go/etc. | C, C++, Rust, Go, Fortran, Ada, D, Zig, Haskell, OCaml, Assembly | Same native source families |
| Windows executable | PE | Windows PE/COFF + subsystem | MSVC, Clang, GCC/MinGW, rustc, Go, .NET | C, C++, Rust, Go, C#, F#, VB.NET, Assembly and others | .c, .cpp, .rs, .go, .cs, .fs, .vb, .asm |
| Windows DLL | PE DLL | Windows | MSVC/Clang/GCC/rustc/Go/.NET | C, C++, Rust, Go, C#, F#, VB.NET | Native source or managed assembly references |
| macOS executable | Mach-O | Darwin/macOS | Clang, Swift, rustc, Go and others | C, C++, Objective-C, Objective-C++, Swift, Rust, Go, Assembly | .c, .cpp, .m, .mm, .swift, .rs, .go, .s/.asm |
| macOS dylib | Mach-O dylib | Darwin/macOS | Clang/Swift/rustc/etc. | C-family, Swift, Rust, Go, Assembly | Corresponding source families |
| Unix static library | AR archive containing objects | Unix/toolchain | GCC/Clang/other native toolchains | Language determined from contained objects and metadata | Source family depends on member objects |
| JVM class | Class-file magic/version | JVM | javac, kotlinc, scalac, Eclipse/compiler plugins | Java, Kotlin, Scala and JVM languages | .java, .kt/.kts, .scala plus debug/source attributes |
| JAR | ZIP + JVM metadata | JVM | javac/kotlinc/scalac/build systems | Java/Kotlin/Scala/other JVM languages | Source mappings depend on contained classes and metadata |
| .NET assembly | PE + CLR metadata | .NET | csc, fsc, vbc, dotnet | C#, F#, Visual Basic .NET and other CLI languages | .cs, .fs/.fsi/.fsx, .vb and metadata/PDB |
| BEAM module | BEAM VM metadata | Erlang/BEAM | erlc/elixirc | Erlang, Elixir and BEAM languages | .erl/.hrl, .ex/.exs and debug chunks |
| Lua bytecode | Lua bytecode signature/version | Lua runtime | luac / Lua toolchain | Lua | .lua |
| WebAssembly module | WASM magic/version | WASM runtime | rustc, clang/LLVM, Go, language-specific compiler | Rust, C/C++, Go, AssemblyScript and other WASM producers | Depends on custom/name/debug sections and producer metadata |
| LLVM bitcode | LLVM bitcode | LLVM target metadata | clang/LLVM frontends | C, C++, Rust, Swift, Fortran, and other LLVM-backed languages | Source mapping strengthened by debug metadata |
| Raw firmware/image | no universal container | architecture/platform evidence required | toolchain-specific | C, C++, Rust, Assembly and firmware-specific languages | Requires external build/provenance evidence |

## What can map an executable back to source?

The decompiler should inspect these evidence classes:

1. Embedded source/debug metadata — DWARF, CodeView/PDB references, JVM source/debug attributes, .NET metadata/PDB, BEAM debug chunks, WASM DWARF/name sections.
2. Build and provenance metadata — build IDs, reproducible-build identifiers, package metadata, compiler version strings, linker records, signed provenance.
3. Symbol and linkage information — symbol names, mangling, exports, imports, relocation patterns, runtime helper names.
4. Compiler fingerprints — code-generation patterns, section conventions, runtime libraries, exception/RTTI models, ABI conventions.
5. Binary format and OS/ABI — ELF/PE/Mach-O/managed runtime plus architecture and calling convention.
6. Code structure — control-flow graphs, type recovery, data-flow, calling relationships, strings, constants, and recognizable library implementations.
7. Filename/extension alone — weak evidence and never sufficient.

## Library-to-source resolution

A library should be decomposed before language attribution:

container -> members -> object/code/data members -> symbols/debug/provenance -> producer -> language candidates -> source candidates

An AR archive does not itself imply C. Its members may have been produced from C, C++, Rust, Fortran, Ada, or assembly. Likewise, an ELF shared object does not by itself identify its source language.

## Source reconstruction levels

- Exact source reference — original source is available or explicitly referenced.
- Debug-assisted reconstruction — substantial source-level metadata exists.
- Symbol-assisted reconstruction — symbols and ABI information provide strong names/types.
- Compiler-assisted reconstruction — producer/toolchain fingerprint provides a constrained candidate family.
- Semantic reconstruction — source-like code is recovered from machine behavior.
- Partial reconstruction — some regions are understood while others remain unresolved.
- Unknown — evidence does not justify a source-language claim.

The output should retain which level produced each conclusion.

## Reference artifact classes

The decompiler reference system should maintain mappings for executable, shared library, dynamic library, static library, object file, archive, JVM class, JVM archive, .NET assembly, BEAM module, Lua bytecode, WebAssembly module, LLVM IR/bitcode, firmware/raw image, and core/crash artifacts where supported.

## Safety

Reverse mapping is analysis-only by default. The system must not load or execute a library merely to identify it. Any sandboxed dynamic analysis must be a separately governed capability with explicit policy, isolation, provenance checks, and resource limits.

## Reverse resolution model

artifact -> container/format -> architecture -> OS/ABI -> library/object members -> debug/provenance -> producer -> language candidates -> version candidates -> source references -> reconstructed output

The result is an evidence graph, not a single guessed source language.

**SLeeLa — MEARVK LLC — 2026**
