# PA-RISC DMA

DMA is modeled at the system/platform level.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory system

HP PA-RISC systems used dedicated I/O and chipset hardware, so a concrete machine profile supplies its DMA implementation.

The generic CPU ISA does not claim an integrated DMA engine.
