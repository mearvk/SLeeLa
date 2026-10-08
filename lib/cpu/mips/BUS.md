# MIPS Bus

## Architectural boundary

The MIPS ISA does not require one universal external bus implementation. SLeeLa therefore separates the CPU core from its system interconnect.

The bus model carries:

- address;
- read/write;
- transfer size;
- data;
- byte enables;
- response/wait;
- exception/error status;
- arbitration ownership.

## Load/store path

Load and store instructions calculate an effective address in EX and perform the memory transaction in MEM.

## System interconnect

A concrete MIPS SoC may use a proprietary or standardized interconnect. The SLeeLa core connects through SLIOBus and SLBusArbiter rather than hard-coding one external bus.

## DMA

External DMA is a separate bus master coupled through SLBusArbiter.
