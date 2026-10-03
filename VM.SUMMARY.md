# SLeeLa Virtual Machine Family Summary

**SLeeLa / SLVM Architecture Document**  
**Repository:** `mearvk/SLeeLa`  
**Document:** `VM.SUMMARY.md`  
**Status:** Architectural reference  
**Scope:** The eleven numbered SLeeLa virtual-machine generations currently represented by the repository, plus the authoritative `/impl` execution core

---

## I. Purpose

This document establishes a single architectural view of the SLeeLa Virtual Machine (SLVM) family.

The repository currently contains **12 VM entries in the architecture: `/impl` Core plus the eleven numbered generations `/1` through `/11`**.

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
10. **SLVM/9 — filesystem and operating-system adaptation**
11. **SLVM/10 — verified storage execution**
12. **SLVM/11 — filesystem-module hosting and schema-driven extension**

The numbered generations preserve the architectural development of SLVM. The `/impl` tree provides the present authoritative, buildable execution substrate.

---

## II. The Eleven Numbered VM Generations

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
| **SLVM/9** | `/sleela-virtual-machine/9` | Filesystem and OS adaptation generation | Filesystem Abstraction Layer, native OS adapters, capability negotiation, TAC3 profile |
| **SLVM/10** | `/sleela-virtual-machine/10` | Verified storage generation | Storage identity, filesystem generation verification, adapter qualification, quiesce/recovery |
| **SLVM/11** | `/sleela-virtual-machine/11` | Filesystem-module host generation | Schema-driven filesystem modules, module discovery, validation, capability qualification |

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

SLVM/11 represents the newest numbered VM generation presently documented in the repository.

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

The eleven numbered generations should be treated as a **single VM family**.

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

## XI. What the Eleven Numbered VMs Do Not Mean

The existence of eleven numbered generations does **not** mean that SLeeLa requires eleven unrelated language interpreters.

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

This makes the eleven-generation architecture sustainable.

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

The eleven-generation VM architecture plus the Core provides a path toward a single coherent SLeeLa VM family without requiring eleven permanently divergent runtimes.

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

This principle permits the repository to retain the work represented by all eleven numbered VM generations plus the Core while avoiding unnecessary duplication of language semantics and runtime machinery.

The repository's eleven numbered VM generations are consequently the complete current numbered SLVM family, `/1` through `/11`, with `/impl` serving as the separate authoritative Core implementation.

---

## XVI. Repository Reference

Primary repository:

`https://github.com/mearvk/SLeeLa`

Primary operational execution tree:

`/impl`

VM generation tree:

`/sleela-virtual-machine/`

Generation directories:

- `/sleela-virtual-machine/1` — Foundation
- `/sleela-virtual-machine/2` — Operator
- `/sleela-virtual-machine/3` — Specialist
- `/sleela-virtual-machine/4` — Supervisor
- `/sleela-virtual-machine/5` — Manager
- `/sleela-virtual-machine/6` — Director
- `/sleela-virtual-machine/7` — Administrator
- `/sleela-virtual-machine/8` — Executive
- `/sleela-virtual-machine/9` — Authority
- `/sleela-virtual-machine/10` — Principal
- `/sleela-virtual-machine/11` — Sovereign

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

### Eleven-Generation Context

This present source-to-VM model sits beneath the eleven-generation architecture described in this document. /impl provides the current operational execution substrate, while SLVM/1 through SLVM/11 describe the complete numbered VM family and its increasingly specialized execution contracts around that common executable representation.

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

This is the present foundation on which the eleven numbered VM generations can be understood.


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


---

## XX. SLVM/9 — Filesystem and Operating-System Adaptation

SLVM/9 extends SLVM/8 by making filesystem semantics an explicit negotiated capability of the execution environment.

The design introduces a Filesystem Abstraction Layer (FAL) between the supervised VM and native operating-system/filesystem APIs. Linux, Windows, and macOS receive explicit adapter foundations. Custom filesystems can advertise capabilities without changing SLeeLa language semantics.

The TAC3 design in Ubuntu.Determinant.Beta.Restricted/tools/tac3 is an important reference case. TAC3 has a versioned on-disk format, explicit superblock and extents, a read-only persistent-mount phase, a contextual FILE layer, HEALTH/ADMIN/RECOVERY regions, device-class metadata, system-pointer relationships, and a deliberate distinction between reconstructed read-only state and actual durable writes. SLVM/9 models these as capabilities rather than assuming that ordinary inode/path semantics apply.

The resulting path is:

    SLeeLa Source -> Compiler -> Artifact -> SLVM/8 Admission/Supervision -> SLVM/9 Filesystem Abstraction Layer -> OS Adapter -> Native Filesystem

Central invariant: a filesystem may only provide the guarantees it can actually demonstrate. Durability, atomicity, recovery, contextual identity, native handles, and transactions are separately discoverable capabilities. Unknown or unsupported features are denied rather than guessed.


---

## XXI. SLVM/10 — Verified Storage Execution

SLVM/10 composes the SLVM/9 filesystem boundary into a verified storage execution layer. Storage identity, filesystem generation, adapter qualification, policy state, and requested operation must agree before an operation is accepted.

Its filesystem-module contract is shared with SLVM/9 and SLVM/11. TAC3 is the first standardized module and uses the same machine-readable definition in all three generations.

## XXII. SLVM/11 — Filesystem-Module Host

SLVM/11 establishes a dedicated extension point for filesystem definitions and structures at `/sleela-virtual-machine/11/file-system/modules/<module-id>/`. Modules are discovered by versioned module ID and schema rather than by pathname or guessed filesystem family. Module presence does not grant SLeeLa capability.

## XXIII. Standardized TAC3 Module

SLVM/9, SLVM/10, and SLVM/11 now contain the same formulaic TAC3 module structure:

    file-system/
      modules/
        tac3/
          MODULE.md
          tac3.definition.json

The machine-readable `tac3.definition.json` is byte-identical across all three generations. The definition carries the TAC3 v1.0 format, 4096-byte block size, CRC32C integrity, persistent regions, 33-stat contextual identity model, authority notation, memory/swap policy, optional FAT model, boot/recovery sequence, persistence status, and VM integration rules. Generation-specific C headers provide the validation adapter while the filesystem data remains one contract.


## Native source compiler implementation

The source-to-VM architecture is backed by the repository's existing native C/C++ frontend under `/impl/frontend`. The lexer, parser, semantic analyzer, compiler/lowering layer, and persistent artifact emitter feed the C SLeeLa Core. The `/lib/compiler` package now builds that authoritative executable through `/impl/Makefile`.

The build gate is recursive: every `/lib/**/*.sleela` file is inventoried, the source-side `lib/vm/InstructionSet.sleela` is compared in exact order with the native `SLOp` enumeration, and only then does `tools/sleela-build.py compile SOURCE OUTPUT` delegate to the native artifact compiler. This makes the `/lib` source collection an actual compiler resource boundary rather than a documentation-only inventory.


---

## XXIV. Formal VM Naming and Source-Creator Contract

The complete formal naming system is:

| **/impl** | **Core** | `/impl` | Authoritative operational execution substrate |
| **SLVM/1** | **Foundation** | `/sleela-virtual-machine/1` | Complete VM foundation, broker, capabilities, security, OS boundary |
| **SLVM/2** | **Operator** | `/sleela-virtual-machine/2` | Cryptographic identity, secure linking, observation |
| **SLVM/3** | **Specialist** | `/sleela-virtual-machine/3` | Isolation, verification, observation, attestation |
| **SLVM/4** | **Supervisor** | `/sleela-virtual-machine/4` | Authenticated and distributed execution |
| **SLVM/5** | **Manager** | `/sleela-virtual-machine/5` | Reproducibility, policy, recovery, replay, quotas |
| **SLVM/6** | **Director** | `/sleela-virtual-machine/6` | Continuous verification, lineage, leases, migration |
| **SLVM/7** | **Administrator** | `/sleela-virtual-machine/7` | Hardened management, recovery, watchdogs, quarantine |
| **SLVM/8** | **Executive** | `/sleela-virtual-machine/8` | Admission, supervised execution, capability leases, transactions, audit |
| **SLVM/9** | **Authority** | `/sleela-virtual-machine/9` | Filesystem and operating-system adaptation |
| **SLVM/10** | **Principal** | `/sleela-virtual-machine/10` | Verified storage execution and storage identity |
| **SLVM/11** | **Sovereign** | `/sleela-virtual-machine/11` | Filesystem-module hosting and schema-driven extension |

The names are architectural identifiers, not permissions. A higher-numbered generation does not automatically grant OS, filesystem, network, storage, security, or administrative authority. Capabilities remain explicit and validated.

### Source-side VM Creator definitions

The SLeeLa source definitions under `/lib/vm/creator/` mirror the formal VM names:

| VM | Formal Name | SLeeLa Source |
|---|---|---|
| `/impl` | **Core** | `SleelaVMCore.sleela` |
| `/1` | **Foundation** | `SLVMFoundation.sleela` |
| `/2` | **Operator** | `SLVMOperator.sleela` |
| `/3` | **Specialist** | `SLVMSpecialist.sleela` |
| `/4` | **Supervisor** | `SLVMSupervisor.sleela` |
| `/5` | **Manager** | `SLVMManager.sleela` |
| `/6` | **Director** | `SLVMDirector.sleela` |
| `/7` | **Administrator** | `SLVMAdministrator.sleela` |
| `/8` | **Executive** | `SLVMExecutive.sleela` |
| `/9` | **Authority** | `SLVMAuthority.sleela` |
| `/10` | **Principal** | `SLVMPrincipal.sleela` |
| `/11` | **Sovereign** | `SLVMSovereign.sleela` |

`SleelaVMCreator.sleela` coordinates construction and `SleelaVMGenerationCatalog.sleela` provides the canonical number/name/path mapping.

The VM Creator construction flow is:

**configuration → generation catalog → formal VM source → architecture/options → build plan → compiler → native C/C++ boundary → VM artifact → verification → package**

### Core relationship

`/impl` is **Core** and remains the standard authoritative operational implementation. It is not a twelfth numbered VM generation. The numbered generations define execution contracts around the common SLeeLa language/compiler model and Core representation.

### SLVM/9 Authority

Authority defines the filesystem/OS adaptation boundary and negotiated filesystem capabilities.

### SLVM/10 Principal

Principal adds verified storage execution, requiring storage identity, filesystem generation, adapter qualification, policy state, and requested operation to agree.

### SLVM/11 Sovereign

Sovereign hosts schema-driven filesystem modules under `/sleela-virtual-machine/11/file-system/modules/<module-id>/`. Module discovery does not itself grant capability.

### Documentation consistency

All references in this document to seven, eight, or nine VM generations are historical wording and should be interpreted as superseded by the current **SLVM/1–SLVM/11 + /impl Core** architecture. The current authoritative architecture is **12 VM entries total: `/impl` Core plus `/1` through `/11`**. The numbered VM family is exactly eleven generations, and `/11` Sovereign is the newest numbered VM.

## XXV. Complete 12-Entry VM Index

The complete current VM architecture is explicitly enumerated here so the summary never ends at an older generation:

| Entry | Formal Name | Role |
|---|---|---|
| /impl | **Core** | Authoritative operational execution implementation |
| /1 | **Foundation** | VM foundation, broker, capabilities, security, OS boundary |
| /2 | **Operator** | Cryptographic identity, secure linking, observation |
| /3 | **Specialist** | Isolation, verification, observation, attestation |
| /4 | **Supervisor** | Authenticated and distributed execution |
| /5 | **Manager** | Reproducibility, policy, recovery, replay, quotas |
| /6 | **Director** | Continuous verification, lineage, leases, migration |
| /7 | **Administrator** | Hardened management, recovery, watchdogs, quarantine |
| /8 | **Executive** | Admission, supervised execution, leases, transactions, audit |
| /9 | **Authority** | Filesystem and operating-system adaptation |
| /10 | **Principal** | Verified storage execution and storage identity |
| /11 | **Sovereign** | Filesystem-module hosting and schema-driven extension |

**There are no numbered generations after /11 in the current architecture. /11 is the newest numbered VM. /impl is Core, not /12.**
