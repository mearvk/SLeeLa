<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-vm-creator-logo-001.jpg" alt="SLeeLa VM Creator" width="100%"></p>


# SLeeLa VM Source Classes

The `/lib/vm` model defines SLeeLa source-level classes for constructing SLVM and SLJVM pieces.

## Related SLeeLa VM and Architecture Source Files

The following `.sleela` files are the source-level VM and architecture definitions currently present in `/lib/vm`. They are the SLeeLa-side definitions that describe the VM components, configuration, managers, execution structures, and SLVM/SLJVM architecture. The native C/C++ implementation is support for these definitions; it is not a replacement for them.

### VM foundation and execution model

- `SleelaVMSource.sleela` — authoritative VM source request.
- `SleelaVMArchitecture.sleela` — VM architecture definition.
- `SleelaVMModule.sleela` — SLeeLa VM module contract.
- `SleelaVMCompiler.sleela` — VM source compilation model.
- `SleelaSLVM.sleela` — SLeeLa SLVM execution target.
- `SleelaSLJVM.sleela` — SLeeLa SLJVM execution target.
- `SLVM.sleela` — core SLVM definition.
- `SLVMClass.sleela` — VM class model.
- `SLVMField.sleela` — VM field model.
- `SLVMMethod.sleela` — VM method model.
- `SLVMObject.sleela` — VM object model.
- `SLVMValue.sleela` — VM value model.
- `SLVMFrame.sleela` — VM execution-frame model.
- `SLVMThread.sleela` — VM thread model.
- `SLVMModule.sleela` — VM module representation.
- `SLVMModuleLoader.sleela` — VM module loading model.
- `SLVMNativeBinding.sleela` — native binding model.
- `SLVMMemory.sleela` — VM memory model.
- `SLVMHeap.sleela` — VM heap model.

### VM options, capabilities, and resource architecture

- `SleelaVMOptions.sleela` — aggregate VM configuration options.
- `SleelaVMOptionCodes.sleela` — mutually exclusive VM option codes.
- `SleelaVMFeatureBits.sleela` — independent VM feature/capability bits.
- `SleelaVMExecutionOptions.sleela` — execution configuration.
- `SleelaVMRuntimeOptions.sleela` — runtime configuration.
- `SleelaVMCpuOptions.sleela` — CPU configuration.
- `SleelaVMConcurrencyOptions.sleela` — concurrency configuration.
- `SleelaVMIOOptions.sleela` — I/O configuration.
- `SleelaVMMemoryOptions.sleela` — memory configuration.
- `SleelaVMSecurityOptions.sleela` — security configuration.
- `SleelaVMPhysicalLimits.sleela` — physical/resource fitment limits.
- `SleelaVMResourcePlan.sleela` — resolved resource plan.
- `SleelaVMObjectCountDeclaration.sleela` — declared VM object inventory/count model.
- `SleelaVMOutput.sleela` — VM build/output description.
- `SleelaVMBuildOptions.sleela` — build configuration.
- `SleelaVMBuildPlan.sleela` — resolved build plan.

### Memory Management and Security Management

- `SleelaVMMemoryManagementSimple.sleela` — simple/complete MM profile.
- `SleelaVMMemoryManagementManaged.sleela` — managed/secure MM profile.
- `SleelaVMMemoryManagementAdvanced.sleela` — advanced/enterprise MM profile.
- `SleelaVMSecurityManagementSimple.sleela` — simple/complete SM profile.
- `SleelaVMSecurityManagementManaged.sleela` — managed/secure SM profile.
- `SleelaVMSecurityManagementAdvanced.sleela` — advanced/enterprise SM profile.

### Compiler Manager and VM construction management

- `SleelaVMCompilerManager.sleela` — VM Compiler Manager contract.
- `SleelaVMCompilerManagerBasic.sleela` — Basic Compiler Manager profile.
- `SleelaVMCompilerManagerAdvanced.sleela` — Advanced Compiler Manager profile.
- `SleelaVMCompilerManagerReport.sleela` — Compiler Manager reporting model.

### Linking Manager architecture

- `SleelaVMLinkingManagerBasic.sleela` — Basic linking profile.
- `SleelaVMLinkingManagerModerate.sleela` — Moderate linking profile.
- `SleelaVMLinkingManagerAdvanced.sleela` — Advanced linking profile.
- `SleelaVMLinkingManagerGovernment.sleela` — Government linking profile.
- `SleelaVMLinkingManagerMilitary.sleela` — Military linking profile.

### Challenge and Reports Managers

- `SleelaVMChallengeManager.sleela` — Challenge Manager contract.
- `SleelaVMChallengeManagerBasic.sleela` — Basic challenge profile.
- `SleelaVMChallengeManagerModerate.sleela` — Moderate challenge profile.
- `SleelaVMChallengeManagerAdvanced.sleela` — Advanced challenge profile.
- `SleelaVMReportsManager.sleela` — Reports Manager contract.

This list is intentionally source-oriented: a VM component belongs to the SLeeLa VM architecture when its behavior and configuration are represented by these `.sleela` definitions and connected to the corresponding SLeeLa VM module path. The compilation and native-build layers turn those definitions into VM-ready artifacts; the SLeeLa VM configuration and authorized module graph govern their runtime use.

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