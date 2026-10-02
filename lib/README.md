<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">





# SLeeLa Standard Library Front End

The `lib/` tree is the unified SLeeLa source layer for the language-facing SDK and VM/OS object model.

SLeeLa is treated as a Turing-complete language whose front end should be expressed in SLeeLa wherever the language can carry the contract. Native C/C++ remains below explicit VM/OS bridge boundaries for facilities requiring kernel, device, memory, networking, cryptography, or process primitives.

The library uses one SLeeLa source file per front-end object. This makes the object inventory measurable and gives the project a path toward a roughly 2,000-object standard library without hiding declarations inside aggregate files.

Families: `core/`, `collections/`, `text/`, `io/`, `vm/`, `os/`, `net/`, `security/`.

The standard-library target is **2,048 object types**. This is an architectural target, not a claim that all 2,048 objects are implemented today.

**SLeeLa — MEARVK LLC — 2026**
## Library discovery

The /lib tree is recursively indexed by the compiler and Nordshrift loader. Current canonical collection: **74 package families / 967 .sleela source units / 88 module-facade symbols / 1,055 total symbol records**. See `LIBRARY.SYMBOLS.md` for the complete collection.

The compiler and Nordshrift share recursive `/lib` discovery; new package directories and source units require no compiler allow-list update.

## Decompiler

The /lib/decompiler package provides the SLeeLa-sourced and SLeeLa-driven decompiler model. It supports explicit source language/version selection, expected input, desired output, fractional-input handling, loadable language modules, weighted OS/ABI discernment, evidence-preserving reconstruction, and VM-ready validation.

## Compiler and Decompiler Reference Catalogs

Compiler: lib/compiler/LANGUAGE.FORMAT.REFERENCE.md

Decompiler: lib/decompiler/LANGUAGE.FORMAT.REFERENCE.md

These catalogs provide language names, producer-program mappings, source/IR/object relationships, known binary and executable formats, OS/ABI associations, and safety-aware native reference data.