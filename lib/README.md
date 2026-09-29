<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Standard Library Front End

The `lib/` tree is the unified SLeeLa source layer for the language-facing SDK and VM/OS object model.

SLeeLa is treated as a Turing-complete language whose front end should be expressed in SLeeLa wherever the language can carry the contract. Native C/C++ remains below explicit VM/OS bridge boundaries for facilities requiring kernel, device, memory, networking, cryptography, or process primitives.

The library uses one SLeeLa source file per front-end object. This makes the object inventory measurable and gives the project a path toward a roughly 2,000-object standard library without hiding declarations inside aggregate files.

Families: `core/`, `collections/`, `text/`, `io/`, `vm/`, `os/`, `net/`, `security/`.

The standard-library target is **2,048 object types**. This is an architectural target, not a claim that all 2,048 objects are implemented today.

**SLeeLa — MEARVK LLC — 2026**
## Library discovery

The /lib tree is recursively indexed by the compiler and Nordshrift loader. Current inventory: **23 packages / 901 .sleela source units**, with **53 module-facade symbols** added for subsystem packages. See `LIBRARY.SYMBOLS.md` for the complete collection.

API is now represented in /lib as a package facade so compiler and loader package discovery includes the API module.