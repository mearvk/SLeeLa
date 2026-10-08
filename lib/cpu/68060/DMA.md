# Motorola 68060 DMA

DMA is external to the processor.

SLDMAController
-> SLBusArbiter
-> 68060 bus
-> memory/I/O

System implementations can supply DMA engines, SCSI/network controllers, and other bus masters. These remain platform resources rather than 68060 execution units.
