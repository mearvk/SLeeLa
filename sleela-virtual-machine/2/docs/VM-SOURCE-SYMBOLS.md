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
