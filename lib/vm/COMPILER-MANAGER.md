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
