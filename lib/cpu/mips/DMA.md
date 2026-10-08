# MIPS DMA

DMA is not an intrinsic requirement of the MIPS instruction-set architecture.

SLeeLa models system DMA separately:

SLDMAController -> SLBusArbiter -> SLIOBus

The DMA controller can request bus ownership, perform memory/peripheral transfers, and release ownership.

A particular MIPS SoC may integrate DMA controllers or use external DMA. Those capabilities belong to that SoC/core profile and are not assigned to all MIPS processors.
