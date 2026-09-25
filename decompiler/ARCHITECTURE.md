# SLeeLa Decompiler Architecture

**Max Rupplin - MEARVK LLC - 2026**

```
Artifact
   |
   v
Container Reader -> PE / ELF / Mach-O / Raw
   |
   v
Architecture Decoder -> x86/x64/ARM/ARM64
   |
   v
Instruction Stream
   |
   +--> Symbols / Imports / Exports / Relocations
   |
   v
Basic Blocks -> CFG -> Function Candidates
   |
   v
SLIR
   |
   +--> Data-flow
   +--> Type hints
   +--> Call graph
   +--> Source reconstruction
   |
   v
Reports / JSON / API / Refactoring
```

## Design rules

1. Parsing is deterministic and side-effect free.
2. Original bytes are preserved and referenced by offset.
3. Every inferred object carries provenance.
4. Architecture-specific decoding is isolated from SLIR.
5. The public API uses opaque handles where ABI stability matters.
6. The VM is a validation target for SLIR and never an execution path for input binaries.
7. Output is suitable for archival storage and later re-analysis.

## Native VM

The underlying SLeeLa VM provides a compact instruction set for SLIR execution tests. It contains a register file, bounded linear memory, call stack, deterministic scheduler state, and host-independent traps. It is deliberately separate from operating-system execution.

## Driver analysis

PE SYS files are handled through the same PE parser as DLL files, with additional driver-oriented metadata records for subsystem, imports, exports, dispatch-table evidence and section permissions. No device is opened and no kernel module is loaded.
