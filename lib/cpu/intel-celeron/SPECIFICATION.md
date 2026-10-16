# Intel Celeron Specification

## 1. Identity

**Model:** Intel Celeron  
**Architecture class:** 32/64-bit value microprocessor  
**Data path:** 32-bit  
**Addressing:** 36-bit external address space (64 GiB)  
**Register model:** CISC named/split register set

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

IA-32/x86-64 register model with reduced cache versus the contemporary Core/Pentium parts.

The SLeeLa model stores these in SLRegisterFile and exposes 8 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 32 bits.
- Address path: 36 bits.
- Directly addressable space: 64 GiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 1200 MHz (overclockable in the model via setSpecClock).

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
- L2: present
- L3: none

cache coherence protocol: none

## 7. DMA

The base CPU has no integrated DMA controller; DMA is a system-level bus-master concern and SLDMAController is left uninstantiated for the base profile.

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

- https://en.wikipedia.org/wiki/Celeron
