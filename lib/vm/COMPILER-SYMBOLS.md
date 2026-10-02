# SLeeLa VM Source Symbol Contract

The VM source classes under `/lib/vm` are compiler-visible symbols.

## Input symbols

Core: `SleelaVMSource`, `SleelaVMModule`, `SleelaVMArchitecture`, `SleelaVMPhysicalLimits`, `SleelaVMOptions`, `SleelaVMOptionCodes`, `SleelaVMFeatureBits`, `SleelaVMExecutionOptions`, `SleelaVMMemoryOptions`, `SleelaVMCpuOptions`, `SleelaVMConcurrencyOptions`, `SleelaVMIOOptions`, `SleelaVMSecurityOptions`, `SleelaVMRuntimeOptions`, `SleelaVMJVMOptions`, `SleelaVMBuildOptions`.

MM: `SleelaVMMemoryManagementSimple`, `SleelaVMMemoryManagementManaged`, `SleelaVMMemoryManagementAdvanced`.

SM: `SleelaVMSecurityManagementSimple`, `SleelaVMSecurityManagementManaged`, `SleelaVMSecurityManagementAdvanced`.

## Planning

MM checkpointing requires integrity; migration requires checkpointing. SM replay and certificates require cryptography; attestation requires certificates; delegation and provenance require audit evidence.

## Output

The compiler emits target, architecture, OS, ABI, selected options, feature masks, MM/SM policy, modules, generated C/C++ units, capabilities, resource reservations, verification metadata, and final artifact information.

## VM generations 1–6

All six VM generations consume the same authoritative SLeeLa Core/Output representation. New MM/SM symbols are common compiler symbols; generation-specific policy remains a fitment constraint.

Copyright (c) Max Rupplin - MEARVK LLC - 2026
