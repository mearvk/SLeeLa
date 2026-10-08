# Motorola 68030 DMA

DMA remains external to the CPU execution core.

SLDMAController
-> SLBusArbiter
-> 68030 bus
-> memory/I/O

The processor's bus arbitration and bus-master interface allow platform DMA engines and other masters to share the system bus. The generic CPU model does not claim an integrated DMA controller.
