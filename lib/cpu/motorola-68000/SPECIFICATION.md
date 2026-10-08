# Motorola 68000 Specification

## 1. Programmer model

| Register | Width | Purpose |
|---|---:|---|
| D0-D7 | 32 each | data registers |
| A0-A7 | 32 each | address registers |
| PC | 32 | program counter |
| SR | 16 | status/control register |

A7 is the active stack pointer. The status register includes condition-code bits and supervisor/interrupt control.

## 2. Datapath

The 68000 is a 32-bit processor internally/programmatically while presenting a 16-bit external data bus. The original family provides a 24-bit address space, giving 16 MiB of directly addressable locations.

SLeeLa keeps internal operand width separate from external transfer width.

## 3. Memory organization

The M68000 stores multi-byte quantities in big-endian order. Word transfers are aligned to even addresses in the base architecture.

## 4. Addressing

The architecture provides a rich effective-address system including data and address register direct, address indirect, postincrement, predecrement, displacement, indexed addressing, absolute addressing, immediate, and PC-relative forms.

SLeeLa represents effective-address formation as a distinct execution/coupler stage rather than hiding it inside a generic ALU call.

## 5. Timing

The M68000 manual documents distinct byte and word read/write cycles, read-modify-write cycles, interrupt acknowledge cycles, and bus arbitration.

Timing records therefore contain clock phase, bus cycle, function code, address, data, read/write, byte/word size, bus acknowledge/wait, interrupt state, and arbitration state.

## 6. Cache

The original MC68000 has no conventional L1/L2/L3 cache hierarchy.

## 7. DMA

DMA is not modeled as an integrated MC68000 CPU block. External DMA or a later integrated 68K processor can become a separate bus master through SLBusArbiter.

NXP documents later integrated 68K processors such as the MC68340 as processors with DMA, demonstrating why DMA must remain a variant/system property rather than being assigned to the base 68000.

## 8. Interrupts and exceptions

SLeeLa models reset, interrupt acknowledge, traps, faults, and exception-vector entry as explicit control-flow events. The vector mechanism is part of the 68K architectural model rather than an invented external convention.

## 9. MMU/TLB

The original 68000 has no MMU/TLB. Memory translation/protection belongs to later processors or external hardware.

## 10. Evidence discipline

Documented, derived, estimated, and unspecified remain separate. The circuit model does not claim undocumented transistor-level details.

## Sources

- NXP M68000 8-16-32-Bit Microprocessors User's Manual: https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf
- NXP 68000 Family Programmer's Reference Manual: https://www.nxp.com/products/MC68000
