<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Standard Library Front End

The `lib/` tree is the unified SLeeLa source layer for the language-facing SDK and VM/OS object model.

SLeeLa is treated as a Turing-complete language whose front end should be expressed in SLeeLa wherever the language can carry the contract. Native C/C++ remains below explicit VM/OS bridge boundaries for facilities requiring kernel, device, memory, networking, cryptography, or process primitives.

The library uses one SLeeLa source file per front-end object. This makes the object inventory measurable and gives the project a path toward a roughly 2,000-object standard library without hiding declarations inside aggregate files.

Families: `core/`, `collections/`, `text/`, `io/`, `vm/`, `os/`, `net/`, `security/`, `opcodes/`.

The standard-library target is **2,048 object types**. This is an architectural target, not a claim that all 2,048 objects are implemented today.

**SLeeLa — MEARVK LLC — 2026**
## Library discovery

The /lib tree is recursively indexed by the compiler and Nordshrift loader. Current canonical collection: **76 package families / 10,138 .sleela source units / 88 module-facade symbols / 10,226 total symbol records**. See `LIBRARY.SYMBOLS.md` for the complete collection.

The compiler and Nordshrift share recursive `/lib` discovery; new package directories and source units require no compiler allow-list update.

The `opcodes` family expresses the canonical SLeeLa VM instruction set as one SLeeLa class per opcode: 103 `SLOp*` classes (codes 0–102; the base 98 are `OP_NOP`..`OP_AUDIO_PLATFORM`), plus `SLOpcodeBase` and `SLOpcodeStream`. Each class carries a single opcode and honours the fetch-then-execute-one contract — carefully call the VM to the next instruction, then execute exactly that one opcode — modeling `impl/core`'s dispatch loop at the SLeeLa layer. The fetch/dispatch primitives sit below the explicit VM/OS bridge in `native/src/sleela_opcode.cpp`. See `opcodes/OPCODES.md`, including the rationale for why 98 opcodes is complete for a modern program and developer.

## Decompiler

The /lib/decompiler package provides the SLeeLa-sourced and SLeeLa-driven decompiler model. It supports explicit source language/version selection, expected input, desired output, fractional-input handling, loadable language modules, weighted OS/ABI discernment, evidence-preserving reconstruction, and VM-ready validation.

## Compiler and Decompiler Reference Catalogs

Compiler: lib/compiler/LANGUAGE.FORMAT.REFERENCE.md

Decompiler: lib/decompiler/LANGUAGE.FORMAT.REFERENCE.md

These catalogs provide language names, producer-program mappings, source/IR/object relationships, known binary and executable formats, OS/ABI associations, and safety-aware native reference data.

## SST and Nordshrift source/compile integration

`/lib/sst` and `/lib/nordshrift` are canonical SLeeLa source-facing packages. They participate in the same recursive `/lib` discovery used by the compiler and loader.

The path is:

`.sst source -> Nordshrift -> /lib discovery -> .sleela source -> SLeeLa Compiler -> SLeeLa IR -> SLVM/SLJVM`

SST remains declarative input. Nordshrift resolves and emits SLeeLa source. The compiler consumes that output through the normal library index; there is no second package allow-list.