# Tutorial 03 — Language and Binary Format References

## Goal

Use the compiler reference catalog to plan source and artifact mappings.

Language references include SLeeLa, C/C++, Rust, Go, Swift, Fortran, Ada, Java, Kotlin, C#, Python, JavaScript/TypeScript, Ruby, PHP, Lua, Haskell, OCaml, Erlang/Elixir, WebAssembly, and LLVM IR.

Binary references include ELF, PE/COFF, Mach-O, a.out, AR, WebAssembly, JVM Class/JAR, .NET assemblies, BEAM, Lua bytecode, LLVM bitcode, and raw binary.

Use the mapping as planning and identification data:

source language -> producer/toolchain -> source/IR/object form -> binary format -> architecture/OS/ABI -> VM target.

The mapping does not authorize execution or loading.

Read /lib/compiler/LANGUAGE.FORMAT.REFERENCE.md plus LanguageReference.sleela, ProgramMapping.sleela, BinaryFormatReference.sleela, and SafetyReference.sleela.
