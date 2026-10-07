<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-compiler-logo-001.jpeg" alt="SLeeLa Compiler" width="100%"></p>


# SLeeLa Compiler

Max Rupplin - MEARVK LLC - 2026

The `/lib/compiler` package is the SLeeLa-sourced compiler design and implementation layer. It is VM-ready: compiler declarations are written in `.sleela`, and the native C/C++ layer provides a stable implementation boundary for compiler services.

## Multi-language framework

The package also provides a **modular framework for building compilers for any
publicly known programming language** (C, C++, Java, Python, JavaScript, Rust,
Go, …). A developer adds a language by writing one small SLeeLa front end that
extends `SLLanguageCompiler`; many independent front ends register into one
shared `SLCompilerRegistry` with no per-language allow-list. Each front end
lowers its own language toward the common SLeeLa IR and a VM-ready artifact.

See [`MULTI-LANGUAGE.FRAMEWORK.md`](MULTI-LANGUAGE.FRAMEWORK.md) for the full
reference, the per-language front ends under [`frontends/`](frontends/), and
lesson [`tutorials/04-building-a-language-front-end.md`](tutorials/04-building-a-language-front-end.md).

The framework identifies, plans, and reports; it never executes an input
program. Compilation is not execution — a produced artifact runs only later
across an explicit SLeeLa VM/OS and security boundary.

## Authority

The authoritative path is:

`.sleela source -> compiler frontend -> semantic analysis -> SLeeLa IR -> VM lowering -> SLVM/SLJVM artifact`

The compiler does not create a parallel language. Native C/C++ code implements the declared compiler services; SLeeLa definitions describe the compiler model and are the source-level contract.

## Pipeline

1. Source loading
2. Lexing
3. Parsing
4. Declaration/object discovery
5. Name and symbol resolution
6. Type and semantic analysis
7. SLeeLa IR construction
8. Capability and dependency review
9. VM lowering
10. SLVM/SLJVM emission
11. Diagnostics and compile report

## Profiles

- BASIC_COMPLETE: a complete usable compiler path for a declared target.
- ADVANCED_TOTAL: complete pipeline inventory with explicit semantic, capability, dependency, security, provenance, and VM-target checks.

The Compiler Manager in `/lib/vm` remains the completeness gate for VM object declarations. This package supplies the compiler implementation that feeds that gate.

## Native boundary

C provides the stable ABI. C++ may orchestrate compiler objects and pipelines, but cannot bypass the SLeeLa capability, security, resolver, memory, certificate, or VM boundaries.

## Build

`make -C lib/compiler`

or from the repository root:

`make compiler`

No object, module, capability, or VM feature is silently inserted or removed by the compiler.

## Language and format reference

The package uses LANGUAGE.FORMAT.REFERENCE.md as its native reference for language names, producer programs, source forms, IR/object models, binary/executable formats, OS/ABI associations, and safety considerations. These references are identification and planning data only; they never grant execution permission.

## SST and Nordshrift source/compile integration

`/lib/sst` and `/lib/nordshrift` are canonical SLeeLa source-facing packages. They participate in the same recursive `/lib` discovery used by the compiler and loader.

The path is:

`.sst source -> Nordshrift -> /lib discovery -> .sleela source -> SLeeLa Compiler -> SLeeLa IR -> SLVM/SLJVM`

SST remains declarative input. Nordshrift resolves and emits SLeeLa source. The compiler consumes that output through the normal library index; there is no second package allow-list.

## SST symbol intake

SST declarations are not opaque metadata. Nordshrift resolves them as package-qualified compiler symbols against the canonical `/lib` index before emitting SLeeLa. The compiler then consumes the emitted SLeeLa symbols through its normal name-resolution pipeline.

## SST symbol intake

SST declarations are not opaque metadata. Nordshrift resolves them as package-qualified compiler symbols against the canonical `/lib` index before emitting SLeeLa. The compiler then consumes the emitted SLeeLa symbols through its normal name-resolution pipeline.

## Native compiler build

The native implementation is now wired directly to the authoritative `/impl/frontend` compiler build. `make -C lib/compiler all` builds the compiler contract objects, builds `impl/build/sleela`, and runs the recursive `/lib` source + ISA gate. The repository-wide `tools/sleela-build.py compile SOURCE OUTPUT` command performs the same source inventory/ISA gate before emitting a persistent runnable `.sleela` Core artifact.