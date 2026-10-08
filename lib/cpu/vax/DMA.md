# DEC VAX DMA

DMA is modeled at the system/interconnect level.

SLDMAController
-> SLBusArbiter
-> VAX system bus
-> memory/peripheral

Unibus and later VAX system configurations may contain independent bus-mastering devices. This is not treated as an undocumented CPU-internal DMA engine.
