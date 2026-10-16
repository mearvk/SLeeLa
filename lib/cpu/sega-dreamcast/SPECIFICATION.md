# Sega Dreamcast (SH-4) Specification

## 1. Identity

**Model:** Sega Dreamcast (SH-4)  
**Architecture class:** 32-bit console main CPU (SH-4)  
**Data path:** 32-bit  
**Addressing:** 32-bit external address space (4 GiB)  
**Register model:** RISC general-purpose register file

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

Hitachi SH-4 register model (R0-R15 banked, PC, SR, FP bank) with on-chip cache and MMU.

The SLeeLa model stores these in SLRegisterFile and exposes 16 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 32 bits.
- Address path: 32 bits.
- Directly addressable space: 4 GiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 200 MHz (overclockable in the model via setSpecClock).

The model records nominal frequency, cycle number, bus direction, and completion phase. Clock frequency is not instruction latency: one instruction may consume multiple cycles.

## 5. Execution model

The SLeeLa model sequences each instruction as:

1. instruction fetch;
2. opcode decode;
3. operand/address acquisition;
4. ALU/register operation;
5. memory read/write;
6. next-instruction transition.

## 6. Cache

- L1 instruction cache: present
- L1 data cache: present
- L2: none
- L3: none

cache coherence protocol: none

## 7. DMA

The processor/SoC integrates a DMA engine; SLeeLa may instantiate SLDMAController for it.

## 8. Interrupts

The model records interrupt request, recognition, vector/entry and timing through SLInterruptController without inventing undocumented internal behavior.

## 9. MMU/TLB

The processor integrates an MMU/TLB; address translation is modeled as virtual -> physical through that unit.

## 10. Evidence labels

- documented: directly stated by a hardware/reference source.
- derived: mechanically calculated from documented properties.
- estimated: engineering approximation, never presented as silicon fact.
- unspecified: deliberately left unknown.

## Sources

- https://en.wikipedia.org/wiki/Dreamcast
- https://en.wikipedia.org/wiki/SuperH
