# SLeeLa VM Source Symbol Contract

The VM source classes under `/lib/vm` are compiler-visible symbols. The authoritative compiler must accept these classes as input symbols and must be able to emit their resolved construction information as output metadata.

## Input symbols

`SleelaVMSource`, `SleelaVMModule`, `SleelaVMArchitecture`, `SleelaVMPhysicalLimits`, `SleelaVMOptions`, `SleelaVMOptionCodes`, `SleelaVMFeatureBits`, `SleelaVMExecutionOptions`, `SleelaVMMemoryOptions`, `SleelaVMCpuOptions`, `SleelaVMConcurrencyOptions`, `SleelaVMIOOptions`, `SleelaVMSecurityOptions`, `SleelaVMRuntimeOptions`, `SleelaVMJVMOptions`, and `SleelaVMBuildOptions`.

## Compiler planning symbols

The compiler resolves architecture, operating system, ABI, execution model, memory model, CPU features, concurrency, I/O, resolver, security, capabilities, runtime, JVM/broker, build, packaging, and physical limits.

## Output symbols

The resolved plan must expose target kind, architecture, OS, ABI, selected option codes, feature masks, module set, generated C/C++ units, headers, libraries, capabilities, resource reservations, verification metadata, and final artifact information.

## VM generations 1–6

SLVM/1 through SLVM/6 consume the same authoritative SLeeLa Core/Output Symbol representation. Generation-specific VM policy adds security, distributed execution, attestation, migration, lineage, leases, and recovery requirements without creating a second language or symbol vocabulary.

New VM source symbols therefore become part of the common VM construction contract for all six generations; unsupported generation-specific features are rejected or omitted according to the source's required/optional status.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
