# Intel 8086 Specification

## 1. Identity

**Model:** Intel 8086  
**Architecture class:** 16-bit microprocessor  
**Data path:** 16-bit  
**Addressing:** 20-bit external address space (1 MiB)  
**Register model:** CISC named/split register set

The profile exposes a programmer model while using the shared SLeeLa CPU objects (clock, register file, ALU, decoder, bus) to sequence each instruction.

## 2. Programmer-visible state

AX, BX, CX, DX, SI, DI, BP, SP, IP, segment registers (CS/DS/SS/ES), FLAGS.

The SLeeLa model stores these in SLRegisterFile and exposes 8 addressable register slots to the runnable model.

## 3. Address and data path

- Data path: 16 bits.
- Address path: 20 bits.
- Directly addressable space: 1 MiB.
- The model's SLRAM arena is bounded for simulation; the architectural address width is recorded above.

## 4. Clock

Rated model clock: about 5 MHz (overclockable in the model via setSpecClock).

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

- https://en.wikipedia.org/wiki/X86
- https://en.wikipedia.org/wiki/Intel_8086
