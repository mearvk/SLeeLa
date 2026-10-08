# Motorola 68040 DMA

DMA remains external to the processor core.

SLDMAController
-> SLBusArbiter
-> 68040 bus
-> memory/I/O

Platform implementations may contain DMA engines and other bus masters. They are not treated as integrated 68040 CPU execution units.
