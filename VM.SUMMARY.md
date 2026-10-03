# SLeeLa Virtual Machine Family Summary

**SLeeLa / SLVM Architecture Document**  
**Repository:** `mearvk/SLeeLa`  
**Document:** `VM.SUMMARY.md`  
**Status:** Architectural reference  
**Scope:** The eight numbered SLeeLa virtual-machine generations currently represented by the repository, plus the authoritative `/impl` execution core

---

## I. Purpose

This document establishes a single architectural view of the SLeeLa Virtual Machine (SLVM) family.

The repository currently contains **nine discrete VM generations or execution architectures** when the authoritative operational VM in `/impl` is considered together with the eight numbered generations under `/sleela-virtual-machine`.

These generations are related, but they are not equivalent independent interpreters. The current architecture is better understood as:

1. **`/impl` — the authoritative operational execution core**
2. **SLVM/1 — dedicated VM, broker, capability, security, and OS boundary**
3. **SLVM/2 — cryptographic and VM-link evolution**
4. **SLVM/3 — isolation and verification evolution**
5. **SLVM/4 — authenticated/distributed execution architecture**
6. **SLVM/5 — reproducibility, policy, recovery, and distributed assurance**
7. **SLVM/6 — continuous verification, lineage, attestation, leases, and migration**
8. **SLVM/7 — hardened management, failure handling, recovery, and resource safety**
9. **SLVM/8 — supervised execution, admission, capability leases, transactions, and audit orchestration**

The numbered generations preserve the architectural development of SLVM. The `/impl` tree provides the present authoritative, buildable execution substrate.

---

## II. The Eight Numbered VM Generations

| Generation | Repository location | Present characterization | Primary architectural emphasis |
|---|---|---|---|
| **Current Core** | `/impl` | Operational/authoritative execution implementation | Core bytecode execution, runtime services, C ABI, OS/runtime integration |
| **SLVM/1** | `/sleela-virtual-machine/1` | Substantial dedicated VM implementation | Broker, capabilities, security, memory security, observation, OS adapters |
| **SLVM/2** | `/sleela-virtual-machine/2` | Architectural/security generation | Cryptography, linking, observation, security evolution |
| **SLVM/3** | `/sleela-virtual-machine/3` | Architectural verification generation | Isolation, verification, observation, attestation/certification |
| **SLVM/4** | `/sleela-virtual-machine/4` | Architectural generation/specification | Authenticated and distributed execution direction |
| **SLVM/5** | `/sleela-virtual-machine/5` | Architectural generation with extended policy/reliability model | Reproducibility, policy verification, recovery, replay, quotas, compatibility |
| **SLVM/6** | `/sleela-virtual-machine/6` | Advanced architectural generation | Continuous verification, execution lineage, multi-party attestation, leases, migration |
| **SLVM/7** | `/sleela-virtual-machine/7` | Hardened management/recovery generation | Log and memory managers, watchdog health, manager dependency validation, bounded recovery, quarantine, resource pressure handling |
| **SLVM/8** | `/sleela-virtual-machine/8` | Supervised execution generation | Admission gate, immutable policy, capability leases, transaction control, lifecycle supervisor, chained audit evidence |

The distinction between **implementation** and **architecture** is intentional. A generation can define a VM contract, security model, execution boundary, or compatibility model before it has an execution implementation equivalent in size to `/impl`.

---

## III. The Authoritative Execution Model

### `/impl`

The current `/impl` tree is the authoritative operational implementation.

Its Core VM is a **Turing-complete, stack-based bytecode virtual machine** with a stable C-facing execution boundary. SLeeLa source is compiled into the repository's Core representation and executed by this implementation.

The execution model includes the fundamental VM responsibilities:

- values and value representation
- operand stack
- constants
- globals
- call frames
- opcode dispatch
- arithmetic and comparison
- branching
- calls and returns
- artifact loading
- runtime libraries
- memory management
- threading
- synchronization
- I/O
- networking
- filesystem/path facilities
- terminal support
- time
- media/audio support
- regular expressions
- system monitoring
- platform integration

The principal Core implementation is centered around:

`/impl/core/sleela_core.c`

with its corresponding public interface and supporting runtime modules.

The `/impl` architecture therefore represents the point at which the SLeeLa VM family has a concrete, integrated execution substrate rather than only a VM generation specification.

---

## IV. SLVM/1 — The Dedicated VM Boundary

SLVM/1 is the first major dedicated VM generation under `/sleela-virtual-machine`.

It contains a substantial implementation including:

- `slvm.c`
- `slvm_broker.c`
- `slvm_broker_security.c`
- `slvm_capability.cpp`
- `slvm_io_heuristic.c`
- `slvm_memory_security.c`
- `slvm_observer.c`
- `slvm_os.cpp`
- POSIX OS adapter
- Windows OS adapter
- `slvm_security.c`
- `slvm_sleela.cpp`

Its public interfaces include VM, broker, capability, security, memory-security, observer, OS, and SLeeLa integration headers.

SLVM/1 therefore establishes a clear VM boundary around:

**execution → broker → capabilities/security → operating-system facilities**

This generation is particularly important historically because it demonstrates that SLeeLa execution was designed as a managed VM boundary rather than merely as a language interpreter.

---

## V. SLVM/2 — Cryptographic and Linking Evolution

SLVM/2 develops the dedicated VM model toward stronger identity, security, and inter-VM relationships.

Its principal public interfaces include:

- `slvm2.h`
- `slvm2_crypto.h`
- `slvm2_link.h`
- `slvm2_observer.h`

Its documentation defines the architectural improvements over SLVM/1 and gives cryptographic security and linking first-class treatment.

SLVM/2 should therefore be understood as the generation in which the VM boundary begins to encompass:

- cryptographic identity
- secure linking
- authenticated relationships
- VM observation
- stronger security contracts

It is not necessary to treat SLVM/2 as an unrelated execution engine. It is an evolution of the VM boundary established by SLVM/1.

---

## VI. SLVM/3 — Isolation and Verification

SLVM/3 extends the model from security mechanisms toward explicit execution verification and isolation.

Its public interfaces include:

- `slvm3.h`
- `slvm3_isolation.h`
- `slvm3_observer.h`
- `slvm3_verify.h`

Its architectural documentation gives explicit attention to:

- isolation
- verification
- observation
- security
- certificates/attestation
- generation-to-generation improvements

The conceptual transition is:

**secure execution → execution whose conditions can be verified and constrained**

SLVM/3 consequently provides the architectural bridge between an ordinary protected runtime and a higher-assurance VM model.

---

## VII. SLVM/4 — Authenticated and Distributed Execution

SLVM/4 is presently much more specification-oriented than `/impl` or SLVM/1.

The current repository representation should therefore not be described as a complete independent executable runtime.

Instead, SLVM/4 establishes the next architectural direction: execution in environments where authentication, distributed relationships, and trusted execution boundaries become part of the VM contract.

SLVM/4 is consequently an important generation even where the implementation footprint is intentionally small.

---

## VIII. SLVM/5 — Reproducibility, Policy, Recovery, and Compatibility

SLVM/5 advances the VM family into a higher-assurance distributed execution model.

The architecture addresses concepts including:

- reproducible execution
- reproducible build identity
- stronger attestation
- versioned and immutable policies
- durable audit checkpoints
- distributed transaction boundaries
- fault/recovery state
- resource reservations
- quotas
- capability delegation
- certificate lifecycle and trust rotation
- deterministic replay
- cross-generation compatibility negotiation

SLVM/5 retains the authoritative SLeeLa compiler and Core/Output Symbol representation as the language execution source.

It therefore does not establish a second SLeeLa language runtime. Instead, it strengthens the conditions under which the authoritative language artifacts may execute.

---

## IX. SLVM/6 — Continuous Verification and Migration

SLVM/6 represents the most advanced VM generation presently documented in the repository.

Its conceptual pipeline is:

**SLeeLa source → authoritative compiler → Output Symbols/Core → verified artifact → SLVM/6 → capability/security boundary → broker/resolver → OS adapter**

The generation adds architectural concerns including:

- continuous verification
- execution lineage
- multi-party attestation
- lease revocation
- failure-domain isolation
- execution migration

The inclusion of migration and lineage is significant. The VM is no longer modeled solely as a process that begins, executes, and exits. Its execution identity, trust state, and relationship to its execution environment can remain meaningful throughout its lifetime.

---

## X. The Family Relationship

The eight numbered generations should be treated as a **single VM family**.

They can be represented conceptually as:

```
                         SLeeLa Source
                              |
                              v
                    Authoritative Compiler
                              |
                              v
                    Output Symbols / Core
                              |
              +---------------+----------------+
              |                                |
              v                                v
       CURRENT EXECUTION                 VM GENERATIONS
          /impl/core                    /sleela-virtual-machine
              |                                |
              |                +---------------+---------------+
              |                |                               |
              |              SLVM/1                          SLVM/2
              |                |                               |
              |                v                               v
              |              SLVM/3                          SLVM/4
              |                |                               |
              |                +---------------+---------------+
              |                                |
              |                              SLVM/5
              |                                |
              |                              SLVM/6
              |
              v
             SLVM/7
              |
              v
       Runtime / OS Boundary
```

The generations therefore describe an expanding execution contract:

```
/impl
  Operational execution

SLVM/1
  VM boundary + broker + capabilities + OS/security

SLVM/2
  Cryptographic identity + secure linking

SLVM/3
  Isolation + verification + attestation

SLVM/4
  Authenticated/distributed execution

SLVM/5
  Reproducibility + policy + recovery + replay

SLVM/6
  Continuous verification + lineage + migration

SLVM/7
  Hardened managers + watchdog health + bounded recovery + quarantine

SLVM/8
  Admission + supervised execution + capability leases + transactions + audit
```

---

## XI. What the Seven VMs Do Not Mean

The existence of seven generations does **not** mean that SLeeLa requires seven unrelated language interpreters.

The architectural objective is the opposite.

There should be one authoritative language/compiler model and one authoritative Core representation, with VM generations defining progressively stronger execution environments and contracts.

In particular:

- The SLeeLa language should not fork into seven incompatible languages.
- Core/Output Symbols should remain the common execution representation.
- The compiler should remain authoritative.
- VM generations should preserve compatibility where their contracts permit.
- Security and assurance layers should not unnecessarily duplicate language semantics.
- Runtime services should be shared where the execution contract permits.
- OS integration should remain behind explicit VM/runtime boundaries.

This makes the seven-generation architecture sustainable.

---

## XII. The Emerging Unified Architecture

The repository now contains several components that naturally fit around this VM family:

- `/impl` — operational execution core
- `/lib/compiler` — SLeeLa-driven compiler support
- compiler manager — compiler/document qualification and construction logic
- `/decompiler` — reverse/inspection tooling
- `/resolver` — dynamic resolution facilities
- packet/logging infrastructure
- connector/broker architecture
- artifact handling
- Java-equivalence support under `/lib/java`
- HTTP 1.0–9.0 integrations
- platform adapters and build systems
- verification and security infrastructure

These should be regarded as **supporting layers around the VM family**, rather than separate language runtimes.

The resulting conceptual stack is:

```
SLeeLa Source
     |
     v
Language / Compiler Manager
     |
     v
Compiler
     |
     v
Output Symbols / Core Artifact
     |
     +------------------------------+
     |                              |
     v                              v
/impl Core                     VM Generation Contract
     |                              |
     |                 +------------+------------+
     |                 |            |            |
     |              SLVM/1       SLVM/2       SLVM/3 ...
     |                                             |
     |                                           SLVM/6
     |                                             |
                                           SLVM/7
     |                                             |
     +----------------------+----------------------+
                            |
                            v
                   Broker / Resolver
                            |
                            v
                   Capability Boundary
                            |
                            v
                     OS / Platform
```

This is the most useful way to think about the current repository.

---

## XIII. Implementation Versus Generation

For future development and documentation, the following terminology is recommended.

### Operational VM

A VM with an executable implementation integrated into the supported build/test system.

At present, the principal operational implementation is:

`/impl/core`

SLVM/1 also contains a substantial historical/dedicated implementation.

### VM Generation

A defined architectural generation of the SLVM execution contract.

SLVM/7 currently represents the hardened management direction: it adds explicit Log, Memory, Health/Watchdog, Recovery, Checkpoint, Resource, Attestation, and Lineage management around the continuously verified execution model of SLVM/6.

SLVM/8 represents the next execution-control direction: it adds a pre-execution Admission gate and an explicit Supervisor coordinating immutable policy, expiring capability leases, transactional action groups, chained audit evidence, recovery, quarantine, and shutdown.

SLVM/2 through SLVM/6 increasingly fit this category in their current repository state.

### VM Profile

A future term that may be useful when a single operational substrate exposes different assurance/security/execution contracts.

For example:

- Core/basic profile
- secure profile
- verified profile
- distributed profile
- reproducible profile
- continuously verified/migratable profile

A profile should not imply a new programming language.

---

## XIV. Long-Term Direction

The nine-layer repository architecture provides a path toward a single coherent SLeeLa VM family without requiring eight permanently divergent runtimes.

The intended progression is:

**SLeeLa language**

→ **authoritative compiler**

→ **Core/Output Symbols**

→ **operational SLVM substrate**

→ **security/capability boundary**

→ **verification and attestation**

→ **distributed/reproducible execution**

→ **continuous verification, lineage, and migration**

The numbered generations therefore remain valuable as architectural milestones even as the implementation converges around a common execution substrate.

The goal is not to erase the generations.

The goal is to make their relationship explicit.

---

## XV. Architectural Principle

> **One SLeeLa language. One authoritative compiler model. One common Core representation. A family of VM generations defining progressively stronger execution contracts.**

This principle permits the repository to retain the work represented by all nine repository VM layers while avoiding unnecessary duplication of language semantics and runtime machinery.

The repository's eight numbered VM generations are consequently best understood as **eight discrete points in the evolution of the SLeeLa execution architecture**, with `/impl` serving as the current operational foundation and SLVM/1–6 defining the successive VM-generation architecture.

---

## XVI. Repository Reference

Primary repository:

`https://github.com/mearvk/SLeeLa`

Primary operational execution tree:

`/impl`

VM generation tree:

`/sleela-virtual-machine/`

Generation directories:

- `/sleela-virtual-machine/1`
- `/sleela-virtual-machine/2`
- `/sleela-virtual-machine/3`
- `/sleela-virtual-machine/4`
- `/sleela-virtual-machine/5`
- `/sleela-virtual-machine/6`
- `/sleela-virtual-machine/7`
- `/sleela-virtual-machine/8`

**Max Rupplin - MEARVK LLC - 2026**

---

## XVII. The Present SLeeLa Source-to-VM Execution Model

The VM architecture described above is also grounded in the repository's present source-to-execution workflow.

### Full /lib SLeeLa Source

The SLeeLa library tree under /lib is treated as SLeeLa source material rather than as documentation alone. The source files participate in the compiler/runtime model and are intended to be transformed into runnable SLeeLa artifacts.

The present conceptual flow is:

**full /lib SLeeLa source → SLeeLa compilation → .sleela runnable artifacts → Terminal SLeeLa or VM loading → VM execution**

### .sleela as the Runnable Artifact

A compiled .sleela file is not merely an intermediate text representation. It is the runnable SLeeLa artifact produced by the compilation process and consumed by the SLeeLa execution environment.

The bytecode produced by compilation is consequently part of the VM contract. The VM is responsible for executing that compiled representation according to the SLeeLa language, API, runtime, and platform integration definitions.

### Bytecode and the Modern Operating System

The proposed SLeeLa bytecode model is explicitly intended to maintain a **1:1 execution correspondence** between what the SLeeLa program describes and what the supported operating system execution layer is instructed to perform, subject to the defined SLeeLa API, VM boundary, security/capability rules, and OS facilities.

In this sense, “1:1” means that the VM does not intentionally introduce a second semantic interpretation of the program between the SLeeLa-defined operation and its supported OS-level realization. The VM translates and dispatches the compiled SLeeLa operation through its defined runtime, capability, broker, and platform layers while preserving the operation's SLeeLa meaning.

This is an architectural contract for the proposed SLeeLa bytecode and execution model. Platform-specific details necessarily remain behind the supported OS adapters and runtime interfaces.

### C/C++ VM Execution

The C and C++ implementation of the SLeeLa VM is the execution machinery responsible for running the compiled bytecode.

The intended relationship is:

**SLeeLa source → compiler → .sleela / bytecode → C/C++ SLeeLa VM → SLeeLa API semantics → supported OS facilities**

The C/C++ VM therefore does not define an independent meaning for the program. It executes the compiled bytecode in accordance with the SLeeLa API and the explicit description of the source language and its author-defined semantics.

Where an operation is exposed by the SLeeLa API as an OS-facing capability, the VM's C/C++ runtime and platform layer provide the concrete implementation required to realize that operation on the modern operating system.

### Source, Bytecode, and OS-Level Fidelity

The intended fidelity chain is:

    SLeeLa Source
     |
     | full /lib source and application source
     v
    SLeeLa Compiler
     |
     | compilation
     v
    .sleela Artifact / SLeeLa Bytecode
     |
     | VM loading or Terminal SLeeLa execution
     v
    C/C++ SLeeLa VM
     |
     | SLeeLa API + runtime semantics
     v
    Broker / Capabilities / Platform Adapters
     |
     | defined OS-facing operation
     v
    Modern Operating System

The purpose of this chain is to preserve the source program's defined behavior through compilation and VM execution rather than treating bytecode as an unrelated instruction language.

### Terminal and Direct VM Execution

The same compiled .sleela artifact is intended to support two principal entry paths:

1. **Terminal Command SLeeLa** — invoke the SLeeLa command-line execution environment with the compiled artifact.
2. **Direct VM loading** — load the compiled artifact into the SLeeLa VM execution environment.

Both paths converge on the same underlying SLeeLa execution semantics. The Terminal is therefore an execution front end, while the VM provides the underlying bytecode execution machinery.

### Seven-Generation Context

This present source-to-VM model sits beneath the seven-generation architecture described in this document. /impl provides the current operational execution substrate, while SLVM/1 through SLVM/8 describe increasingly strong VM contracts around that common executable representation.

The important architectural distinction is therefore:

**the SLeeLa program is compiled once into its executable SLeeLa representation; the VM generation determines the execution contract under which that representation runs.**

This allows the repository to maintain a common SLeeLa language and bytecode model while progressively strengthening security, verification, distribution, reproducibility, lineage, and migration characteristics across VM generations.

---

## XVIII. Current Execution Contract

For the purposes of this architectural document, the present SLeeLa VM contract can be summarized as follows:

- /lib contains SLeeLa source that participates in the executable language/runtime ecosystem.
- SLeeLa source is compiled into .sleela runnable artifacts.
- The compiled artifact contains the bytecode/executable representation consumed by SLeeLa execution.
- The Terminal Command version of SLeeLa can execute the compiled artifact.
- The VM can load the same compiled artifact for execution.
- The C/C++ VM executes the bytecode according to the SLeeLa API and language semantics.
- The VM's runtime and platform layers connect those semantics to supported modern operating systems.
- The proposed bytecode contract seeks 1:1 semantic fidelity from SLeeLa-defined operations through VM execution to their defined OS-level realization.
- OS-specific mechanisms remain encapsulated by the VM's runtime, broker, capability, and platform-adapter boundaries.

The resulting principle is:

> **SLeeLa source defines the operation; compilation produces the executable SLeeLa representation; the C/C++ VM executes that representation; and the supported OS integration realizes the defined operation without intentionally changing its SLeeLa meaning.**

This is the present foundation on which the eight numbered VM generations can be understood.


---

## XIX. SLVM/8 — Supervised Execution

SLVM/8 builds directly on SLVM/7 rather than replacing it.

Its key architectural change is the introduction of an explicit **execution admission and supervision layer**. The VM should not begin executing an artifact merely because individual managers report healthy. Admission must first establish that the artifact, immutable policy, manager set, resource budget, capabilities, attestation, and lineage are all acceptable.

The SLVM/8 control path is:

    SLeeLa Artifact
          |
          v
       Admission
          |
          +--> Artifact
          +--> Policy
          +--> Managers
          +--> Resources
          +--> Capabilities
          +--> Attestation
          +--> Lineage
          |
          v
      Supervisor
          |
          v
     Transactional
       Execution
          |
          +--> Audit
          |
          +--> Checkpoint / Recovery
          |
          +--> Quarantine / Shutdown

### Supervisor

The Supervisor owns the lifecycle transitions between normal, admitted, running, degraded, checkpointing, recovering, quiescing, quarantined, and stopped states.

It does not define SLeeLa semantics and cannot grant itself capability.

### Policy and Capability Leases

SLVM/8 makes execution policy an explicit admission input. Capabilities are represented as bounded leases with issuance and expiry epochs. Revocation is immediate, and terminal quarantine invalidates the execution path.

### Transactions

Groups of runtime actions may be represented as transactions with explicit begin, record, commit, and abort states. This is a control mechanism rather than a claim that arbitrary operating-system side effects are inherently reversible. Irreversible operations remain subject to broker and SLeeLa policy.

### Audit

Security-relevant lifecycle decisions receive chained audit evidence. Audit evidence is deliberately separate from authorization; an audit record can prove that a decision was recorded but cannot grant authority.

### Relationship to SLVM/7

SLVM/7 answers:

**Are the managers, resources, checkpoints, recovery state, and evidence healthy enough to continue?**

SLVM/8 adds:

**Has this execution been formally admitted, and is the supervised execution lifecycle still authorized to proceed?**

This makes SLVM/8 the natural next hardening layer above the management and failure controls established by SLVM/7.
