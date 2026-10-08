# IBM POWER DMA

DMA is modeled at the system/platform level.

SLDMAController
-> SLBusArbiter
-> system interconnect
-> memory/I/O

The generic POWER CPU model does not claim an integrated DMA controller. Platform DMA engines and I/O bridges are separate system resources.
