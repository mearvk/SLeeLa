# IBM POWER9 Specification

## 1. Identity

**Model:** IBM POWER9  
**Architecture class:** 64-bit server CPU (Power ISA)  
**Data path:** 64-bit  
**Addressing:** 52-bit external address space (4 PiB)  
**Register model:** RISC general-purpose register file

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

Power ISA v3.0 register model (32 GP, 32 VSR), SMT4/SMT8, large on-chip L2/L3.

The SLeeLa model stores these in SLRegisterFile and exposes 32 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 64 bits.
- Address path: 52 bits.
- Directly addressable space: 4 PiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 3800 MHz (overclockable in the model via setSpecClock).

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
- L3: present

cache coherence protocol: modeled where multi-core

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

- https://en.wikipedia.org/wiki/POWER9
- https://en.wikipedia.org/wiki/Power_ISA
