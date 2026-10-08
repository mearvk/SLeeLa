# x86 / x86-64 DMA

DMA is a platform/system facility.

The CPU model exposes a bus-arbitration/interconnect interface through which an external DMA engine can access memory and devices.

SLDMAController
-> SLBusArbiter
-> memory/controller/device fabric

DMA coherency and ordering are implementation/platform dependent.

The x86 CPU object therefore does not claim an integrated DMA controller unless a particular SoC/processor profile documents one.
