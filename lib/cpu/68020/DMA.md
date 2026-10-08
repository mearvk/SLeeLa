# Motorola 68020 DMA

DMA is external to the CPU core.

SLDMAController
-> SLBusArbiter
-> 68020 system bus
-> memory/I/O

The 68020 bus architecture supports external bus masters through system arbitration. A platform may supply DMA controllers or other bus masters without making them part of the CPU itself.
