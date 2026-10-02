<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa VM Source Classes

The `/lib/vm` model defines SLeeLa source-level classes for constructing SLVM and SLJVM pieces.

## Option model

VM construction is controlled by two complementary mechanisms:

- **Integer option codes** select one value from a mutually exclusive condition set: target, architecture, OS, ABI, execution mode, garbage collector, threading model, I/O model, security model, link model, and package model.
- **Integer bit masks** enable independent capabilities/features such as files, network, DNS, IPC, threads, async I/O, GUI/media, crypto/TLS, JIT/AOT, SIMD/atomics, GC, checkpointing, migration, attestation, observability, deterministic execution, resolver, broker, certificates, and sandboxing.
- **Specific option classes** hold detailed resource and policy parameters for execution, memory, CPU, concurrency, I/O, security, runtime, JVM, and build/package construction.

## Memory and Security Management

MM and SM are explicit source-level architectures with three progressive forms each:

| Family | Simple / Complete | Managed / Secure | Advanced / Enterprise |
|---|---|---|---|
| Memory Management | bounded heap/stack/object budget, cleanup, zeroization | GC, guards, quarantine, scrubbing | reservation, integrity, checkpoint, migration, sealing/encryption policy |
| Security Management | policy, capabilities, isolation, audit | crypto identity, certificates, replay, resolver-aware decisions | attestation, delegation/revocation, provenance, recovery |

These classes lower through the same compiler into C/C++ VM modules and then into SLVM/SLJVM executable parts. They do not create a second language or bypass the capability boundary.

## Construction

`SleelaVMSource` describes the source-level VM request. `SleelaVMCompiler` resolves it into an architecture/resource/build plan. C provides the stable VM construction ABI; C++ provides higher-level orchestration.

## Fitment rules

1. A required option that cannot fit the target architecture or physical limits is rejected.
2. Optional features may be disabled only when the source marks them optional.
3. Capability selection never grants OS authority; capabilities remain explicit.
4. SLVM uses the native C/C++ path; SLJVM adds the JVM/object-broker boundary.
5. MM checkpointing requires integrity; MM migration requires checkpointing.
6. SM replay protection and certificates require cryptographic support; attestation requires certificates.
7. The compiler records selected option codes, feature masks, MM/SM plans, resource plan, ABI, and module set in output metadata.


## Linking Manager

The VM consumes the five common Linking Manager profiles from `/lib/vm`: Basic, Moderate, Advanced, Government, and Military. A link targets an exact known SLVM major/minor version and exposes only capability-authorized observations such as memory, certificates, transaction records, resolver state, audit evidence, attestation, provenance, and checkpoints. The link is observational and cannot be used to bypass VM execution, memory, certificate, or capability controls.


## Challenge and Reports Managers

The Challenge Manager supplies Basic, Moderate, and Advanced declared diagnostic challenge profiles. A matched condition emits `ConditionObserved` to an authorized listener or endpoint; remote operation remains capability- and security-scoped. The Reports Manager observes authorized input, output, messages, and system records and routes them through named binary objects and IQ/system-output paths to authorized messaging APIs.

## Build integration

The package build is available with `make -C lib/vm` and from the repository root with `make vm`. The Compiler Manager contract is reviewed before VM package objects are considered ready for assembly; the build does not silently change the declared VM inventory.
