# Xtensa DMA

DMA is generally a system/peripheral integration feature rather than a universal property of the Xtensa ISA. Model DMA only when the selected SoC profile provides a controller, with explicit channels, request sources, transfer widths, arbitration, completion, and errors. Keep DMA bus-master activity distinct from CPU instruction execution.
