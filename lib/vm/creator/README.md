<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa VM Creator Source Package

The /lib/vm/creator package is the SLeeLa source-side VM Creator. It gives each SLVM generation a stable formal name and source-level construction contract while keeping /impl as the standard native Core implementation.

## Formal VM names

| Path | Formal Name | Source |
|---|---|---|
| /impl | Core | SleelaVMCore.sleela |
| /1 | Foundation | SLVMFoundation.sleela |
| /2 | Operator | SLVMOperator.sleela |
| /3 | Specialist | SLVMSpecialist.sleela |
| /4 | Supervisor | SLVMSupervisor.sleela |
| /5 | Manager | SLVMManager.sleela |
| /6 | Director | SLVMDirector.sleela |
| /7 | Administrator | SLVMAdministrator.sleela |
| /8 | Executive | SLVMExecutive.sleela |
| /9 | Authority | SLVMAuthority.sleela |
| /10 | Principal | SLVMPrincipal.sleela |
| /11 | Sovereign | SLVMSovereign.sleela |

These names are descriptive identifiers, not permissions by themselves. Selecting a higher-numbered VM never implicitly grants OS, filesystem, network, storage, security, or administrative capability.

## Creator entry points

- SleelaVMCreator.sleela — VM construction coordinator.
- SleelaVMGenerationCatalog.sleela — canonical version-to-name/path catalog.
- SleelaVMCore.sleela — /impl Core contract.

The creator reads the canonical VM configuration, selects one generation, resolves its source modules, builds a SleelaVMBuildPlan, invokes the compiler, verifies the artifact, and packages the result.

## Construction model

configuration -> generation catalog -> named VM source -> architecture/options -> build plan -> compiler -> C/C++ native boundary -> VM artifact -> verification -> package

All generations use the common source models under /lib/vm. A generation class adds the responsibilities appropriate to its documented VM architecture.

## Source authority

The .sleela definitions describe what the VM Creator is constructing. Native C/C++ implements required low-level services. The creator must not silently invent modules, capabilities, memory limits, or host authority.