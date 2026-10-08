# AMD 29000 DMA

DMA is modeled as an external/system facility.

SLDMAController
-> SLBusArbiter
-> 29000 system interconnect
-> memory/peripherals

No undocumented CPU-internal DMA engine is assumed.
