# SLeeLa Decompiler API

**Max Rupplin - MEARVK LLC - 2026**

The public API is organized around six layers.

### 1. Artifact API
- `Artifact::open()`
- `Artifact::sha256()`
- `Artifact::size()`
- `Artifact::bytes()`
- `Artifact::format()`
- `Artifact::architecture()`

### 2. Container API
- `PeImage`
- `ElfImage`
- `RawImage`
- `Section`
- `Symbol`
- `Import`
- `Export`
- `Relocation`

### 3. Decode API
- `Decoder`
- `Instruction`
- `Operand`
- `BasicBlock`
- `FunctionCandidate`
- `ControlFlowGraph`

### 4. SLIR API
- `Module`
- `Function`
- `Block`
- `Operation`
- `Value`
- `MemoryRegion`
- `TypeHint`
- `Provenance`

### 5. Analysis API
- `Analyzer`
- `CallGraph`
- `DataFlowGraph`
- `StringIndex`
- `Fingerprint`
- `Report`

### 6. VM API
- `Vm`
- `VmConfig`
- `VmMemory`
- `VmRegisters`
- `VmTrap`

All APIs are read-only against the input artifact. Refactoring operates on SLIR or generated source rather than modifying the original binary.
