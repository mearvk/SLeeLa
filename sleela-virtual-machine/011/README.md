<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 11

SLVM/11 establishes a filesystem-module host above the verified storage contracts of SLVM/9 and SLVM/10. It provides a stable slot for custom filesystem definitions and structures while keeping filesystem semantics versioned and capability-driven.

`file-system/modules/<module-id>/` is the standard extension point. TAC3 is the first formulaically standardized module and is byte-identical with SLVM/9 and SLVM/10.

Custom modules do not receive authority merely by being present; they must be discovered, version-checked, identity-verified, and capability-qualified.

## Runnable VM

SLVM/11 is a **real, runnable virtual machine**, not only a specification. It
links the shared native Core engine (`impl/build/libslcore.a`) and runs compiled
`.sleela` artifacts; its generation contribution — the filesystem-module host —
is applied as a pre-boot admission step (the baseline TAC3 module must validate
before a program boots).

```sh
make -C ../../impl libslcore        # once: build the shared Core engine archive
make                                # build vm-build/slvm-11
./vm-build/slvm-11 --selfcheck      # verify the Core link + TAC3 admit/reject check
./vm-build/slvm-11 program.art      # run a compiled .sleela artifact on SLVM/11
```

From the repository CLI: `sleela vm run premium 11 program.sleela` builds this
VM on demand, compiles the program, and runs it here. `src/slvm11_vm.c` is the
VM entry; `src/slvm11_filesystem_module.c` is the generation feature.

Copyright (c) Max Rupplin - MEARVK LLC - 2026