# SPARC DMA

DMA is modeled as a system-level bus master.

SLDMAController
-> SLBusArbiter
-> SLIOBus / memory system

A concrete SPARC workstation/server may attach implementation-specific DMA engines and coherency behavior.

The generic ISA implementation does not claim an integrated DMA controller where the architecture does not require one.
