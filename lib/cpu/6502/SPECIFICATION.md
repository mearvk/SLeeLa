# 6502 Specification

## 1. Identity

**Model:** MOS Technology 6502  
**Family:** MOS 6500 series  
**Architecture class:** 8-bit microprocessor  
**Addressing:** 16-bit external address bus  
**Base profile:** original NMOS 6502

The 6502 exposes a compact programmer model while using internal latches, buses and control logic to sequence each instruction.

## 2. Programmer-visible state

| Register | Width | Purpose |
|---|---:|---|
| A | 8 | accumulator |
| X | 8 | index register |
| Y | 8 | index register |
| S | 8 | stack pointer |
| PC | 16 | program counter |
| P | 8 | processor status representation |

The hardware status representation has seven meaningful flags: N, V, B, D, I, Z and C. Bit 5 is treated specially rather than as an ordinary writable flag.

## 3. Address and data path

- Data path: 8 bits.
- Address path: 16 bits.
- Little-endian multi-byte addresses.
- 64 KiB directly addressable space.
- Stack occupies page $0100-$01FF in the programmer model.
- Zero page occupies $0000-$00FF.

## 4. Clock

The original 6502 uses a two-phase clocking scheme. External systems commonly describe the phases as phi1 and phi2.

SLeeLa records:

- nominal clock frequency;
- clock phase;
- bus cycle number;
- read/write direction;
- address;
- data;
- interrupt/wait condition;
- completion phase.

Clock frequency must not be confused with instruction latency. A single instruction may consume multiple bus cycles.

## 5. Execution model

The 6502 does not use a modern out-of-order or superscalar pipeline. Instruction work is sequenced through control logic and repeated bus operations.

The SLeeLa model therefore represents:

1. instruction fetch;
2. opcode decode;
3. operand/address acquisition;
4. ALU/register operation;
5. memory read/write;
6. prefetch/next-cycle transition.

## 6. Cache

Base 6502:

- L1 instruction cache: none
- L1 data cache: none
- L2: none
- L3: none
- cache coherence protocol: none

Every memory access is modeled against the external memory/bus interface.

## 7. DMA

The base CPU has no integrated DMA controller. DMA in a 6502 system is an external-system function.

SLeeLa therefore leaves SLDMAController uninstantiated for the base profile while permitting a system-level DMA component to couple to the CPU bus.

## 8. Interrupts

The architectural model includes:

- IRQ: maskable interrupt request;
- NMI: non-maskable interrupt;
- RESET: reset sequence.

The interrupt controller model records request, recognition, vector fetch and entry timing without inventing undocumented internal transistor behavior.

## 9. MMU/TLB

The original 6502 has no MMU or TLB. Address translation is therefore:

CPU address -> external system address

Any banking, mapping or protection belongs to a surrounding system or a later 6500-family derivative.

## 10. Evidence labels

- documented: directly stated by a hardware/reference source.
- derived: mechanically calculated from documented properties.
- estimated: engineering approximation, never presented as silicon fact.
- unspecified: deliberately left unknown.

This profile uses those labels to keep architectural modeling separate from speculation.

## Sources

- MAME 6502 family technical specification: https://github.com/mamedev/mame/blob/master/docs/source/techspecs/m6502.rst
- Visual6502 datapath archive: https://tinymachines.ai/6502/archive/wiki/6502_datapath.html
- 6502 specification reference: https://tutorial-6502.sourceforge.io/specification/
