# VM Source Symbols

This VM generation uses the common SLeeLa VM source symbol contract in `/lib/vm/COMPILER-SYMBOLS.md`.

The generation recognizes the complete VM source vocabulary:
- SleelaVMSource
- SleelaVMModule
- SleelaVMArchitecture
- SleelaVMPhysicalLimits
- SleelaVMOptions
- SleelaVMOptionCodes
- SleelaVMFeatureBits
- SleelaVMExecutionOptions
- SleelaVMMemoryOptions
- SleelaVMCpuOptions
- SleelaVMConcurrencyOptions
- SleelaVMIOOptions
- SleelaVMSecurityOptions
- SleelaVMRuntimeOptions
- SleelaVMJVMOptions
- SleelaVMBuildOptions
- SleelaVMResourcePlan
- SleelaVMBuildPlan
- SleelaVMOutput
- SleelaVMCompiler
- SleelaSLVM
- SleelaSLJVM

Generation-specific policy is applied after common symbol resolution. Required options that cannot be implemented by this VM generation are rejected; optional options may be omitted according to the source declaration.

Copyright (c) Max Rupplin - MEARVK LLC - 2026


## MM/SM management symbols

The VM consumes the common management symbols from `/lib/vm`:

- `SleelaVMMemoryManagementSimple`
- `SleelaVMMemoryManagementManaged`
- `SleelaVMMemoryManagementAdvanced`
- `SleelaVMSecurityManagementSimple`
- `SleelaVMSecurityManagementManaged`
- `SleelaVMSecurityManagementAdvanced`

They lower through the C stable ABI (`lib/vm/include/sleela_vm_management.h`) and C++ orchestration (`sleela_vm_management.hpp`). MM fitment enforces integrity -> checkpoint -> migration dependencies. SM fitment enforces cryptography -> certificates/replay -> attestation/delegation/provenance dependencies.

## Linking Manager

The VM consumes the five common Linking Manager profiles from `/lib/vm`: Basic, Moderate, Advanced, Government, and Military. A link targets an exact known SLVM major/minor version and exposes only capability-authorized observations such as memory, certificates, transaction records, resolver state, audit evidence, attestation, provenance, and checkpoints. The link is observational and cannot be used to bypass VM execution, memory, certificate, or capability controls.


## Challenge and Reports Managers

All VM generations consume the common Challenge Manager and Reports Manager source symbols. Challenge Manager profiles are Basic, Moderate, and Advanced; matched declared conditions emit capability-authorized `ConditionObserved` events. Reports Manager observes authorized input, output, messages, and system records and routes typed named binary objects through IQ/system-output paths. Remote challenge operation remains bounded by capability, resolver, certificate, security, and audit policy.


## Compiler Manager build integration

The VM generation uses the common Compiler Manager contract. Package-native manager objects are compiled with `make -C lib/vm`; the root dispatcher exposes the same check as `make vm`. The CM verifies declared object counts, categories, and dependencies before assembly.
