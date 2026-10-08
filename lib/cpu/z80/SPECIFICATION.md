# Z80 Specification

## Programmer-visible state

| Register | Width | Purpose |
|---|---:|---|
| AF | 16 | accumulator/status pair |
| BC | 16 | general register pair |
| DE | 16 | general register pair |
| HL | 16 | general register pair |
| IX | 16 | index register |
| IY | 16 | index register |
| SP | 16 | stack pointer |
| PC | 16 | program counter |
| I | 8 | interrupt-vector register |
| R | 8 | refresh register |
| AF', BC', DE', HL' | 16 each | alternate register set |

## Instruction model

The Z80 extends the 8080-family programming model with indexed addressing, alternate registers, block operations, bit manipulation, and a richer interrupt system.

SLeeLa models instruction execution as a sequence of machine cycles rather than treating the CPU as a single atomic instruction executor.

## Address/data model

- 8-bit data path.
- 16-bit address path.
- 64 KiB directly addressable memory.
- Separate I/O operation space.
- Little-endian storage for multi-byte values.

## Clock and timing

A basic operation takes multiple T cycles. Zilog documents memory/I/O operations as M cycles composed of T cycles; the first M cycle is normally instruction fetch. WAIT can insert additional wait states. See the Zilog Z80 CPU User Manual.

SLeeLa records clock period, M-cycle number, T-cycle number, operation, address, data, read/write, wait state, and interrupt state.

## Cache

No conventional L1/L2/L3 CPU cache is modeled for the original Z80.

## DMA

The Z80 CPU itself does not contain the separate Z80 DMA peripheral. Zilog documents the Z80 DMA as a separate device with programmable control/status registers and bus-control behavior.

SLeeLa models DMA at the system/peripheral level: SLDMAController -> SLBusArbiter -> SLIOBus.

## Interrupts

The Z80 supports maskable and non-maskable interrupt behavior, with interrupt modes that determine vector handling. SLeeLa records request, recognition, acknowledge, vector handling, and entry timing.

## MMU/TLB

The original Z80 has no MMU or TLB. Memory mapping beyond its native address space belongs to external hardware or a derivative.

## Evidence discipline

Documented, derived, estimated, and unspecified remain separate states. The profile does not infer undocumented transistor-level implementation from behavior alone.

## Sources

- Zilog Z80 CPU User Manual: https://www.zilog.com/docs/z80/z80cpu_um.pdf
- Zilog Z80 CPU Peripherals User Manual: https://www.zilog.com/docs/z80/um0081.pdf
