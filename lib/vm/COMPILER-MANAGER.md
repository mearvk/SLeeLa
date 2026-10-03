<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-vm-creator-logo-001.jpg" alt="SLeeLa VM Creator" width="100%"></p>

# SLeeLa Compiler Manager

The Compiler Manager (CM) is a compile-time completeness gate for an SLVM/SLJVM declaration. The SLeeLa VM source may declare a CM profile and an object-count/category declaration. The CM reviews the declaration before generation and reports exactly which modules are fine, missing, excessive, invalid, or dependent on another module.

## Profiles

### Basic Complete

`SleelaVMCompilerManagerBasic` requires a complete, usable VM construction set. It reviews total object count and the required categories without requiring every advanced subsystem.

### Advanced Total

`SleelaVMCompilerManagerAdvanced` reviews the total VM construction set, exact object counts, category counts, dependency chains, capabilities, certificates, attestation, and provenance where declared by the VM.

## Compile-time declaration

A VM source can include `SleelaVMCompilerManager` plus `SleelaVMObjectCountDeclaration`. The declaration establishes the expected object inventory. Categories include architecture, execution, memory, security, I/O, runtime, management, linkage, observability, and build.

The important rule is **declared intent controls the boundary**: the CM does not silently add or remove objects to make a declaration pass. It reports missing and excess objects so the compiler can require the SLeeLa source to be corrected or explicitly mark an object optional.

## Compile result

The CM produces structured findings with these statuses:

- `FINE` — declared object/category is present at the expected count.
- `MISSING` — a required object/category is absent.
- `EXCESS` — more objects are declared than the selected profile permits.
- `REQUIRES` — another module/part must be present before this one can be valid.
- `INVALID` — the declaration itself is inconsistent.

A textual compile report can therefore read as a module-by-module checklist instead of merely returning a single pass/fail value.


## Build integration

The package build is available with `make -C lib/vm` and from the repository root with `make vm`. The Compiler Manager contract is reviewed before VM package objects are considered ready for assembly; the build does not silently change the declared VM inventory.

## Source-to-VM completeness gate

The Compiler Manager also owns the source inventory boundary. The authoritative input set is every .sleela file recursively under /lib; it is not limited to /lib/compiler or /lib/vm. The current repository contains **10,077 /lib/**/*.sleela source files** on the verified tree snapshot. The inventory is discovered dynamically so newly added library source cannot be omitted by a stale manifest.

Before an artifact is admitted, the manager requires:

1. recursive /lib source discovery;
2. source identity/digest preservation;
3. dependency and symbol coverage;
4. lowering coverage into the canonical SLeeLa IR;
5. exact ISA coverage against /lib/vm/InstructionSet.sleela;
6. native dispatch coverage in impl/core/sleela_core.h and its implementation;
7. artifact ABI validation;
8. execution-test coverage for every emitted instruction family.

The source-side ISA registry currently contains **98 ordered instructions**, and the native SLOp enumeration has been verified to contain the same 98 instructions in exactly the same order. Missing compiler lowering or runtime dispatch remains a completeness failure even when an opcode is present in both registries.


## Native compiler implementation

The native compiler implementation is present under `/impl/frontend` (lexer, parser, semantic analysis, compiler/lowering, artifact emission) and is the authoritative implementation behind `impl/build/sleela`. `/lib/compiler` is the SLeeLa-sourced compiler model; it does not duplicate the native frontend. The package build now wires these layers together and runs the recursive `/lib` source + 98-op ISA gate before source compilation.
