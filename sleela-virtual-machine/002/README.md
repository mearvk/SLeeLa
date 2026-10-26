<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 2

SLVM/2 — codename **Operator** — advances the SLVM/1 **Foundation** toward
cryptographic and VM-link evolution. It uses /1 as its execution and security
specification rather than replacing it, and does not introduce a parallel
language implementation.

## Improvements over /1

- an explicit cryptographic **provider abstraction** (no custom crypto);
- trust-store policy and certificate attestation hooks;
- execution-manifest **linking**: hashes, signatures, ABI and capability declarations;
- user/deployment-selectable runtime target and adapter;
- composed, fail-closed policy for sensitive operations;
- an extended observer surface — security, crypto, link, certificate and runtime events;
- declarative technical compliance profiles;
- versioned runtime-selection configuration (`config/slvm-runtime.conf`).

See `docs/IMPROVEMENTS-FROM-1.md` for the area-by-area /1 → /2 mapping.

## Grade contract

SLVM/2 is a **real, runnable virtual machine**: it links the shared native Core
engine (`impl/build/libslcore.a`) and runs compiled `.sleela` artifacts. Its
distinguishing contribution — cryptographic and VM-link evolution over the
Foundation — is expressed in source (`src/slvm02_vm.c`, `src/slvm2_crypto.c`):
the admission step builds a well-formed instance of this grade's own feature and
calls this grade's real validator, and under `--selfcheck` proves a malformed
instance is rejected. A program boots only once that grade-specific contract is
admitted.

```sh
make -C ../../impl libslcore        # once: build the shared Core engine archive
make                                # build vm-build/slvm-2
./vm-build/slvm-2 --selfcheck       # verify the Core link + grade admit/reject check
./vm-build/slvm-2 program.art       # run a compiled .sleela artifact on SLVM/2
```

From the repository CLI: `sleela vm run premium 2 program.sleela` builds this VM
on demand, compiles the program, and runs it here.

The central invariant remains /1's: if the authoritative compiler can legally
produce an Output Symbol, the VM generation must define its execution semantics
or explicitly reject an incompatible artifact rather than silently reinterpret
it. The C interface remains the stable ABI; C++ may provide higher-level
verification and policy without bypassing the C security boundary.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
