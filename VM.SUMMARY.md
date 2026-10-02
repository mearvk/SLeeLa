# SLeeLa Virtual Machine Family Summary

**SLeeLa / SLVM Architecture Document**  
**Repository:** `mearvk/SLeeLa`  
**Document:** `VM.SUMMARY.md`  
**Status:** Architectural reference  
**Scope:** The seven discrete SLeeLa virtual-machine generations currently represented by the repository

---

## I. Purpose

This document establishes a single architectural view of the SLeeLa Virtual Machine (SLVM) family.

The repository currently contains **seven discrete VM generations or execution architectures** when the authoritative operational VM in `/impl` is considered together with the six numbered generations under `/sleela-virtual-machine`.

These seven are related, but they are not seven equivalent independent interpreters. The current architecture is better understood as:

1. **`/impl` — the authoritative operational execution core**
2. **SLVM/1 — dedicated VM, broker, capability, security, and OS boundary**
3. **SLVM/2 — cryptographic and VM-link evolution**
4. **SLVM/3 — isolation and verification evolution**
5. **SLVM/4 — authenticated/distributed execution architecture**
6. **SLVM/5 — reproducibility, policy, recovery, and distributed assurance**
7. **SLVM/6 — continuous verification, lineage, attestation, leases, and migration**

The numbered generations preserve the architectural development of SLVM. The `/impl` tree provides the present authoritative, buildable execution substrate.

---

## II. The Seven VM Generations

| Generation | Repository location | Present characterization | Primary architectural emphasis |
|---|---|---|---|
| **Current Core** | `/impl` | Operational/authoritative execution implementation | Core bytecode execution, runtime services, C ABI, OS/runtime integration |
| **SLVM/1** | `/sleela-virtual-machine/1` | Substantial dedicated VM implementation | Broker, capabilities, security, memory security, observation, OS adapters |
| **SLVM/2** | `/sleela-virtual-machine/2` | Architectural/security generation | Cryptography, linking, observation, security evolution |
| **SLVM/3** | `/sleela-virtual-machine/3` | Architectural verification generation | Isolation, verification, observation, attestation/certification |
| **SLVM/4** | `/sleela-virtual-machine/4` | Architectural generation/specification | Authenticated and distributed execution direction |
| **SLVM/5** | `/sleela-virtual-machine/5` | Architectural generation with extended policy/reliability model | Reproducibility, policy verification, recovery, replay, quotas, compatibility |
| **SLVM/6** | `/sleela-virtual-machine/6` | Advanced architectural generation | Continuous verification, execution lineage, multi-party attestation, leases, migration |

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

The seven generations should be treated as a **single VM family**.

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

The seven-generation architecture provides a path toward a single coherent SLeeLa VM family without requiring seven permanently divergent runtimes.

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

This principle permits the repository to retain the work represented by all seven VM generations while avoiding unnecessary duplication of language semantics and runtime machinery.

The seven VMs are consequently best understood as **seven discrete points in the evolution of the SLeeLa execution architecture**, with `/impl` serving as the current operational foundation and SLVM/1–6 defining the successive VM-generation architecture.

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

**Max Rupplin - MEARVK LLC - 2026**
