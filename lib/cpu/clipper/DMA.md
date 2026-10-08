# Fairchild Clipper DMA

DMA is modeled as a system-level resource.

SLDMAController
-> SLBusArbiter
-> Clipper system interconnect
-> memory/peripherals

No undocumented CPU-internal DMA engine is assumed.
