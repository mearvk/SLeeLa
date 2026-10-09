# OpenRISC 1000 DMA

DMA is typically part of the SoC or peripheral subsystem, not a universal property of the OR1K ISA. Enable it only when the selected platform provides a DMA controller. Model channels, request sources, transfer widths, bus arbitration, completion, and faults as platform configuration, distinct from CPU load/store execution.
