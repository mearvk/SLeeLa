<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 4

SLVM/4 — codename **Supervisor** — advances the SLVM/3 **Specialist** (verified
execution: isolation, deterministic policy, supply-chain integrity) toward
**authenticated and distributed execution**. It preserves /3's verification and
isolation contract and does not introduce a parallel language implementation.

## Improvements over /3

- authenticated execution **nodes**: a node participates only once its identity
  is proven and its attestation is verified;
- a distributed **execution quorum**: a program is admitted only when at least
  the required number of authenticated, attested, admitted nodes agree;
- **membership epochs** so stale nodes from an earlier epoch cannot participate;
- cluster identity and per-node attestation-hash binding;
- a real admission **validator** (`slvm4_node_validate` / the execution-set
  check) applied before a program runs.

These build directly on /3's verified-execution base and feed forward into /5's
reproducible, policy-verifiable, fault-tolerant execution.

## Grade contract

SLVM/4 is a **real, runnable virtual machine**: it links the shared native Core
engine (`impl/build/libslcore.a`) and runs compiled `.sleela` artifacts. Its
distinguishing contribution — authenticated and distributed execution — is
expressed in source (`src/slvm04_vm.c`, `src/slvm4_distributed.c`; descriptor in
`include/slvm4_distributed.h`): the admission step builds a well-formed
distributed-execution instance and calls this grade's real validator, and under
`--selfcheck` proves a malformed instance (unauthenticated node, failed quorum,
or stale epoch) is rejected. A program boots only once that grade-specific
contract is admitted.

```sh
make -C ../../impl libslcore        # once: build the shared Core engine archive
make                                # build vm-build/slvm-4
./vm-build/slvm-4 --selfcheck       # verify the Core link + grade admit/reject check
./vm-build/slvm-4 program.art       # run a compiled .sleela artifact on SLVM/4
```

From the repository CLI: `sleela vm run premium 4 program.sleela` builds this VM
on demand, compiles the program, and runs it here.

The C interface remains the stable ABI; C++ may provide higher-level
authentication, attestation, and orchestration without bypassing the C security
boundary.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
