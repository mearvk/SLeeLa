# SLeeLa Decompiler API

**Max Rupplin - MEARVK LLC - 2026**

The public API is organized around six layers. The implementation is C++20 and is intended to keep artifact parsing, native recovery, SLIR, and validation separately composable.

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

## Runnable API exemplars

- `decompiler/examples/api_inspect.cpp` — artifact identity and interface inspection.
- `decompiler/examples/api_decode_cfg.cpp` — bounded decoding and CFG construction.
- `decompiler/examples/api_library_metadata.cpp` — shared-library and kernel-module metadata.
- `decompiler/examples/api_library_graph.cpp` — family-level dependency/provider graphing.
- `decompiler/examples/api_slir_vm.cpp` — SLIR construction and deterministic VM validation.

These examples are enabled by default through `SLEE_LA_BUILD_API_EXAMPLES` and are intended to remain synchronized with the public API as it evolves.

## Stability rule

Observed bytes and parser evidence are authoritative inputs. Recovered function boundaries, inferred types, provider matches, and other analytical hypotheses must remain explicitly distinguishable from directly observed evidence.
