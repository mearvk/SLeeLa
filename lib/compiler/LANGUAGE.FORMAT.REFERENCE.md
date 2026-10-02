<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-compiler-logo-001.jpeg" alt="SLeeLa Compiler" width="100%"></p>

# SLeeLa Compiler Language, Program, and Binary Format Reference

Max Rupplin - MEARVK LLC - 2026

This document is the native reference catalog for the SLeeLa Compiler. It records known language names, common producer programs, source extensions, intermediate/object representations, executable formats, operating-system associations, and safety considerations.

## Reference rule

These mappings are recognition and planning references, not execution permissions. A name, extension, magic value, compiler signature, or format match never authorizes execution, loading, linking, installation, or native calls.

## Language and producer mappings

| Language / family | Common producer programs | Source forms | Common intermediate / object forms | Common executable / package forms |
|---|---|---|---|---|
| SLeeLa | SLeeLa Compiler / SLeeLa VM toolchain | .sleela | SLIR, SLVM | SLVM / project-defined artifacts |
| C | GCC, Clang, MSVC, ICC/ICX | .c | LLVM IR, assembly, object | ELF, PE/COFF, Mach-O |
| C++ | GCC/G++, Clang++, MSVC, ICC/ICX | .cc, .cpp, .cxx | LLVM IR, assembly, object | ELF, PE/COFF, Mach-O |
| Objective-C | Clang | .m | LLVM IR, object | Mach-O, ELF, PE/COFF where supported |
| Objective-C++ | Clang | .mm | LLVM IR, object | Mach-O, ELF, PE/COFF where supported |
| Rust | rustc | .rs | LLVM IR, object | ELF, PE/COFF, Mach-O |
| Go | go build / cmd/compile | .go | Go object/internal IR | ELF, PE/COFF, Mach-O |
| Swift | swiftc | .swift | SIL, LLVM IR, object | Mach-O, ELF, PE/COFF where supported |
| Fortran | GCC/gfortran, LLVM Flang, Intel Fortran | .f, .f90, .f95, .f03, .f08 | LLVM IR, object | ELF, PE/COFF, Mach-O |
| Ada | GNAT | .adb, .ads | compiler IR, object | ELF, PE/COFF, Mach-O |
| D | DMD, LDC, GDC | .d | LLVM IR/object or compiler object | ELF, PE/COFF, Mach-O |
| Zig | zig | .zig | LLVM IR/object or native object | ELF, PE/COFF, Mach-O |
| Assembly | NASM, YASM, GAS, MASM | .asm, .s | object | ELF, PE/COFF, Mach-O |
| Pascal / Object Pascal | Free Pascal, Delphi | .pas, .pp | object | ELF, PE/COFF, Mach-O |
| COBOL | GnuCOBOL and other toolchains | .cob, .cbl | object / compiler-specific | ELF, PE/COFF and vendor-specific forms |
| Java | javac, Eclipse compiler | .java | JVM class files, JAR | JVM class/JAR |
| Kotlin | kotlinc | .kt, .kts | JVM class/JAR, Kotlin metadata | JVM class/JAR; native targets vary |
| Scala | scalac | .scala | JVM class/JAR | JVM class/JAR |
| C# | csc, dotnet | .cs | IL/.NET metadata | PE/.NET assembly |
| F# | fsc, dotnet | .fs, .fsi, .fsx | IL/.NET metadata | PE/.NET assembly |
| Visual Basic .NET | vbc, dotnet | .vb | IL/.NET metadata | PE/.NET assembly |
| Python | CPython, PyPy | .py | bytecode / implementation-specific IR | .pyc and packaged forms |
| JavaScript | Node.js, browsers, JS engines | .js, .mjs, .cjs | engine bytecode/JIT artifacts | source/package/bundle |
| TypeScript | tsc, bundlers | .ts, .tsx | JavaScript / source maps | JS packages/bundles |
| Ruby | ruby, JRuby, TruffleRuby | .rb | VM-specific bytecode/JIT | source/package/implementation-specific |
| PHP | php, Zend tooling | .php | VM bytecode/opcache | source/package/implementation-specific |
| Lua | lua, luac | .lua | Lua bytecode | .luac |
| Haskell | GHC | .hs, .lhs | GHC Core/STG, LLVM/native object | ELF, PE/COFF, Mach-O |
| OCaml | ocamlc, ocamlopt | .ml, .mli | bytecode/native object | ELF, PE/COFF, Mach-O |
| Erlang | erlc | .erl, .hrl | BEAM bytecode | BEAM modules/releases |
| Elixir | mix/elixirc | .ex, .exs | BEAM bytecode | BEAM modules/releases |
| WebAssembly | wasm toolchains | language-dependent | WASM modules | .wasm |
| LLVM IR | LLVM language frontends | .ll | LLVM IR | target object/executable after lowering |

## Program-to-format reference

GCC/G++, Clang/Clang++, MSVC, rustc, Go, swiftc, GNAT, DMD/LDC/GDC, zig, NASM/YASM/GAS/MASM, Free Pascal/Delphi, javac, kotlinc, scalac, csc/dotnet, CPython/PyPy, GHC, OCaml tooling, BEAM tooling, and luac are represented as producer references.

Mappings are many-to-many: a language does not uniquely determine an executable format, and an executable format does not uniquely determine a source language.

## Known native binary / executable / object formats

| Format | Family | Typical role | Typical OS / platform evidence |
|---|---|---|---|
| ELF | Executable and Linkable Format | executable, shared object, object, core | Unix-like systems, including Linux |
| PE | Portable Executable | executable, DLL, driver-related image | Windows |
| COFF | Common Object File Format | object and PE-related structures | Windows/toolchain-specific and historical Unix variants |
| Mach-O | Mach Object | executable, dylib, object | Apple platforms |
| a.out | historical executable/object | executable/object | historical Unix systems |
| AR | archive | static library/object archive | Unix/toolchain ecosystems |
| WebAssembly | WASM module | portable sandboxed executable module | browser/runtime/embedded environments |
| JVM Class | JVM bytecode | managed class artifact | Java/JVM |
| JAR | ZIP-based Java archive | JVM application/library package | Java/JVM |
| .NET assembly / PE | managed assembly | IL + metadata | .NET |
| BEAM | Erlang VM artifact | managed bytecode module | Erlang/BEAM |
| Lua bytecode | Lua VM artifact | managed bytecode | Lua runtimes |
| LLVM bitcode | compiler IR | intermediate representation | LLVM ecosystem |
| raw binary | no universal container | firmware, flat image, extracted payload, or unknown | platform inferred from evidence |

## Safety considerations

1. Inspect before executing. Reference databases are for classification, not execution.
2. Treat binaries as untrusted input. Parsing must be bounded and memory-safe.
3. Do not execute constructors, entry points, loader code, or embedded scripts during compilation analysis.
4. Do not equate file extension with identity. Consider headers, sections, relocations, imports/exports, ABI markers, architecture, and compiler metadata together.
5. Preserve conflicting evidence rather than silently choosing one identity.
6. Require architecture and OS agreement before native-target assumptions.
7. Separate managed bytecode from native machine code.
8. Verify provenance, signatures, and checksums where policy requires them.
9. Never bypass SLeeLa capability, resolver, memory, certificate, or VM boundaries.
10. Compilation is not execution. Later native execution must cross an explicit SLeeLa security and VM policy boundary.

## Compiler selection model

language name -> language family -> version -> producer program -> source form -> IR/object model -> target architecture -> target OS/ABI -> executable/package format

Unknown or conflicting mappings remain unresolved and are surfaced in diagnostics rather than silently guessed.

**SLeeLa — MEARVK LLC — 2026**
