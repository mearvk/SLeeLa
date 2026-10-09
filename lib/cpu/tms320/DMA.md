# Texas Instruments TMS320 DMA

DMA and peripheral data movement vary by generation and device. SLeeLa models DMA only when the selected DSP/system profile provides it, using SLDMAController and SLBusArbiter to represent requests, transfers, arbitration, completion, and errors.

External host or peripheral DMA is kept distinct from CPU MAC execution and internal address generation.
