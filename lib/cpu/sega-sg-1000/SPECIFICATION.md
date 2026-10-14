# Sega SG-1000 (Z80) Specification

## 1. Identity

**Model:** Sega SG-1000 (Z80)  
**Architecture class:** 8-bit console main CPU (Z80)  
**Data path:** 8-bit  
**Addressing:** 16-bit external address space (64 KiB)  
**Register model:** accumulator / indexed register machine

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

Z80 register model (AF, BC, DE, HL + alternates, IX, IY, SP, PC).

The SLeeLa model stores these in SLRegisterFile and exposes 10 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 8 bits.
- Address path: 16 bits.
- Directly addressable space: 64 KiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 4 MHz (overclockable in the model via setSpecClock).

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

- L1 instruction cache: none
- L1 data cache: none
- L2: none
- L3: none

cache coherence protocol: none

## 7. DMA

The base CPU has no integrated DMA controller; DMA is a system-level bus-master concern and SLDMAController is left uninstantiated for the base profile.

## 8. Interrupts

The model records interrupt request, recognition, vector/entry and timing through SLInterruptController without inventing undocumented internal behavior.

## 9. MMU/TLB

The base CPU has no MMU/TLB; address translation is CPU address -> external system address, and any mapping belongs to a surrounding system or later derivative.

## 10. Evidence labels

- documented: directly stated by a hardware/reference source.
- derived: mechanically calculated from documented properties.
- estimated: engineering approximation, never presented as silicon fact.
- unspecified: deliberately left unknown.

## Sources

- https://en.wikipedia.org/wiki/SG-1000
- https://en.wikipedia.org/wiki/Zilog_Z80
