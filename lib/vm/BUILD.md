<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-vm-creator-logo-001.jpg" alt="SLeeLa VM Creator" width="100%"></p>

# Building VM Pieces

A VM source object may produce independent C and C++ compilation units. C provides portable ABI-level construction; C++ provides optional orchestration. The final SLVM or SLJVM is assembled only after architecture/resource/security validation.

The same source model supports Linux, Windows, and macOS through platform adapters. Physical limits are measured or supplied as constraints and never grant additional capabilities.


## Compiler Manager package build

Run:

```sh
make -C lib/vm
```

or from the repository root:

```sh
make vm
```

The package build compiles the C stable ABI and C++ wrappers for the VM managers and performs a compile-time Compiler Manager manifest sanity check. The check covers object-count declarations and the ten standard VM categories:

1. architecture
2. execution
3. memory
4. security
5. I/O
6. runtime
7. management
8. linkage
9. observability
10. build

`SleelaVMCompilerManagerBasic` validates a complete usable VM inventory. `SleelaVMCompilerManagerAdvanced` validates the total/deep inventory. Neither profile silently adds or removes VM objects.


## Compiler Manager package build

Run `make -C lib/vm` or `make vm` from the repository root. The package compiles the C stable ABI and C++ wrappers for the VM managers and checks the Compiler Manager declaration contract. The standard categories are architecture, execution, memory, security, I/O, runtime, management, linkage, observability, and build. Findings are `FINE`, `MISSING`, `EXCESS`, `REQUIRES`, or `INVALID`. The build never silently inserts or removes declared VM objects.
