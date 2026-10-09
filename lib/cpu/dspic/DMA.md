# dsPIC DMA

DMA availability, channel count, request sources, transfer widths, and buffering vary by device. When present, model DMA as a bus master with profile-defined requests, arbitration, transfer completion, and errors. Keep peripheral DMA separate from CPU load/store and DSP execution; do not enable DMA for a profile that does not document it.
