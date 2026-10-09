# SHARC DSP DMA

DMA capabilities and channel topology vary by device. When present, model DMA as a bus master with profile-defined channels, descriptors, transfer widths, request sources, arbitration, completion, and error handling. Keep DMA transfers distinct from CPU load/store and MAC operations. A profile without documented DMA must not silently enable it.
