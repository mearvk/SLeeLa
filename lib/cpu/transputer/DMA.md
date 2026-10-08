# Inmos Transputer DMA and Communication

The Transputer's communication links provide hardware-supported movement between processors and peripherals, but they are not modeled as a generic hidden CPU DMA engine.

SLeeLa separates:

- process/channel communication;
- serial link transfers;
- external DMA controllers where a system provides them;
- memory transfers.

SLDMAController and SLBusArbiter remain available at the system level.
