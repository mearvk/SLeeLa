# PA-RISC Specification

## Architectural generations

| Profile | Data width | Character |
|---|---:|---|
| PA-RISC 1.0 | 32-bit | original Precision Architecture |
| PA-RISC 1.1 | 32-bit | MMU/SMP and later extensions |
| PA-RISC 2.0 | 64-bit | widened registers and functional units |

PA-RISC uses fixed 32-bit instructions and a load/store organization. citeturn0search1

## Registers

The architecture provides 32 general-purpose registers. PA-RISC 1.0 also defines shadow registers for fast interrupt handling; floating-point registers are separate. citeturn0search1

## Addressing

The core instruction model includes indexed and displacement-based addressing. Memory-mapped I/O is part of the broader Precision Architecture system model. citeturn0search1

## PA-RISC 2.0

PA-RISC 2.0 widens registers and functional units to 64 bits and expands the virtual-address architecture. It also supports substantially greater instruction-level parallelism in implementations such as PA-8000. citeturn0search1turn0search3

## Optional facilities

- floating point;
- MAX multimedia operations;
- precise traps/interrupts;
- virtual memory;
- SMP/system support;
- implementation-specific caches and prefetch.
