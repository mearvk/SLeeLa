<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-decompiler-logo-001.jpeg" alt="SLeeLa Decompiler" width="100%"></p>\n\n# SLeeLa Decompiler Language, Program, and Binary Format Reference

Max Rupplin - MEARVK LLC - 2026

This is the decompiler-side native reference for known source languages, producer programs, binary/object/executable formats, and platform evidence. It is intentionally compatible with lib/compiler/LANGUAGE.FORMAT.REFERENCE.md.

## Purpose

The decompiler uses these names and mappings as evidence for language identification, source-version hypotheses, compiler/producer recognition, binary and object-format recognition, architecture and ABI analysis, operating-system discernment, reconstruction target selection, and safety policy.

A mapping is never proof by itself.

## Reference families

The catalog covers C, C++, SLeeLa, Objective-C, Objective-C++, Rust, Go, Swift, Fortran, Ada, D, Zig, Assembly, Pascal/Object Pascal, COBOL, Java, Kotlin, Scala, C#, F#, Visual Basic .NET, Python, JavaScript, TypeScript, Ruby, PHP, Lua, Haskell, OCaml, Erlang, Elixir, WebAssembly, and LLVM IR.

Known producers include GCC/G++, Clang/Clang++, MSVC, rustc, Go, swiftc, GNAT, DMD/LDC/GDC, zig, NASM/YASM/GAS/MASM, Free Pascal/Delphi, javac, kotlinc, scalac, csc/dotnet, CPython/PyPy, GHC, OCaml compilers, BEAM tooling, and luac.

## Format evidence

The catalog recognizes ELF, PE, COFF, Mach-O, a.out, AR archives, WebAssembly, JVM class/JAR artifacts, .NET assemblies/PE, BEAM, Lua bytecode, LLVM bitcode, and raw/unknown binary input.

Format recognition should inspect, as applicable:

- magic/header fields;
- class and endianness;
- machine/architecture identifiers;
- sections and segments;
- imports/exports;
- relocations;
- symbol tables;
- debug information;
- runtime/compiler metadata;
- calling conventions;
- loader/linker records;
- code signatures and provenance;
- OS-specific library references.

## Language-to-format mapping principle

binary format -> platform/ABI evidence -> producer evidence -> language evidence -> version evidence -> reconstruction hypothesis

The reverse mapping is never one-to-one. ELF may contain C, C++, Rust, Go, Fortran, Ada, Zig, Haskell, OCaml, or assembly output. PE may contain C/C++, Rust, Go, .NET assemblies, and other toolchain products. Mach-O may contain C-family, Swift, Rust, Go, and other supported native code.

## Safety reference

The decompiler remains analysis-first:

- no automatic execution of an input artifact;
- no loading of untrusted native libraries merely to identify them;
- no invocation of binary entry points;
- no assumption that an extension, filename, or compiler string is trustworthy;
- no conversion of uncertain evidence into a claimed source language without recording confidence;
- conflicting OS/ABI evidence remains visible;
- fractional input remains preserved under the selected fractional-input policy;
- native analysis is static unless an explicit, separately governed sandbox capability exists.

Native C/C++ implementations provide parsing and analysis services behind the SLeeLa-defined boundary. The reference catalog is not a native loader.

## Decompiler resolution model

artifact -> format -> architecture -> OS/ABI -> producer -> language family -> language version -> reconstruction target

The result should include evidence and unresolved regions whenever the mapping is incomplete.

**SLeeLa — MEARVK LLC — 2026**
