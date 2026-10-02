<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-vm-creator-logo-001.jpg" alt="SLeeLa VM Creator" width="100%"></p>

# SLeeLa VM Source Symbol Contract

The VM source classes under `/lib/vm` are compiler-visible symbols. The authoritative compiler must accept these classes as input symbols and emit resolved construction information as output metadata.

## Input symbols

Core construction symbols:

`SleelaVMSource`, `SleelaVMModule`, `SleelaVMArchitecture`, `SleelaVMPhysicalLimits`, `SleelaVMOptions`, `SleelaVMOptionCodes`, `SleelaVMFeatureBits`, `SleelaVMExecutionOptions`, `SleelaVMMemoryOptions`, `SleelaVMCpuOptions`, `SleelaVMConcurrencyOptions`, `SleelaVMIOOptions`, `SleelaVMSecurityOptions`, `SleelaVMRuntimeOptions`, `SleelaVMJVMOptions`, and `SleelaVMBuildOptions`.

Memory Management symbols:

- `SleelaVMMemoryManagementSimple`
- `SleelaVMMemoryManagementManaged`
- `SleelaVMMemoryManagementAdvanced`

Security Management symbols:

- `SleelaVMSecurityManagementSimple`
- `SleelaVMSecurityManagementManaged`
- `SleelaVMSecurityManagementAdvanced`

## Compiler planning

The compiler resolves architecture, operating system, ABI, execution model, memory model, MM policy, CPU features, concurrency, I/O, resolver, SM policy, capabilities, runtime, JVM/broker, build, packaging, and physical limits.

MM dependencies are ordered so that checkpointing requires integrity and migration requires checkpointing. SM dependencies are ordered so replay protection and certificates require cryptography; attestation requires certificate support; delegation/provenance require audit evidence.

## Output symbols

The resolved plan must expose target kind, architecture, OS, ABI, selected option codes, feature masks, MM/SM policy, module set, generated C/C++ units, headers, libraries, capabilities, resource reservations, verification metadata, and final artifact information.

## VM generations 1–6

SLVM/1 through SLVM/6 consume the same authoritative SLeeLa Core/Output Symbol representation. Generation-specific VM policy adds security, distributed execution, attestation, migration, lineage, leases, and recovery requirements without creating a second language or symbol vocabulary.

New VM source symbols therefore become part of the common VM construction contract for all six generations; unsupported generation-specific features are rejected or omitted according to the source's required/optional status.

Copyright (c) Max Rupplin - MEARVK LLC - 2026


## Linking Manager symbols

The compiler resolves five Linking Manager profiles: `SleelaVMLinkingManagerBasic`, `SleelaVMLinkingManagerModerate`, `SleelaVMLinkingManagerAdvanced`, `SleelaVMLinkingManagerGovernment`, and `SleelaVMLinkingManagerMilitary`. Each links to a known SLVM major/minor version before observations are enabled. Memory, certificate, transaction, resolver, audit, attestation, capability, provenance, checkpoint, and higher-assurance observations are emitted only when permitted by the plan. Linking is observational and does not bypass VM capability or security controls.


## Challenge and Reports Manager symbols

Challenge symbols: `SleelaVMChallengeManager`, `SleelaVMChallengeManagerBasic`, `SleelaVMChallengeManagerModerate`, `SleelaVMChallengeManagerAdvanced`. Reports symbol: `SleelaVMReportsManager`. Challenge conditions lower to bounded `ConditionObserved` events and never imply arbitrary executable injection. Reports lower authorized input/output/message observations into named binary objects and IQ/output routing. Remote challenge delivery requires capability, resolver, authentication, and audit policy.


## Compiler Manager symbols

The compiler-visible completeness symbols are `SleelaVMCompilerManager`, `SleelaVMCompilerManagerBasic`, `SleelaVMCompilerManagerAdvanced`, `SleelaVMCompilerManagerReport`, and `SleelaVMObjectCountDeclaration`. CM performs compile-time object-count and category review and emits `FINE`, `MISSING`, `EXCESS`, `REQUIRES`, or `INVALID` findings. The CM never silently changes the declared object inventory to obtain a successful compile.


## Build integration

The package build is available with `make -C lib/vm` and from the repository root with `make vm`. The Compiler Manager contract is reviewed before VM package objects are considered ready for assembly; the build does not silently change the declared VM inventory.
