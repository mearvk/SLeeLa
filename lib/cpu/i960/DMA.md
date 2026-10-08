# Intel i960 DMA

DMA is modeled at the system/interconnect level.

SLDMAController
-> SLBusArbiter
-> i960 system interconnect
-> memory/peripherals

The CPU model does not invent a universal internal DMA engine.
