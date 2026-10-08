# NS32000 DMA

DMA is modeled outside the CPU execution core.

SLDMAController
-> SLBusArbiter
-> 32000 system bus
-> memory/peripherals

External DMA controllers and bus masters can therefore interact with the CPU without being incorrectly represented as CPU-internal execution units.
