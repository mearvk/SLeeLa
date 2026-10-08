# PowerPC DMA

DMA is modeled as a system-level bus-master function.

SLDMAController
-> SLBusArbiter
-> SLIOBus

The CPU may continue, stall, or observe coherency effects according to the concrete system/interconnect implementation.

Embedded PowerPC implementations can integrate substantial system peripherals. For example, the MPC860 documentation describes an implementation whose architecture and memory-management facilities differ from the complete PowerPC architecture. citeturn0search7

Therefore DMA belongs in the concrete SoC/processor profile rather than being assumed for every PowerPC CPU.
