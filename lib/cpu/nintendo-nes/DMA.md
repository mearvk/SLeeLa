# Nintendo NES (Ricoh 2A03 / 6502) DMA

## Base CPU / SoC

This part integrates DMA capability.

SLeeLa may instantiate SLDMAController and arbitrate its transfers against CPU bus access:

SLDMAController -> SLBusArbiter -> SLIOBus

## Modeling rule

Model only the DMA facts that are documented for this part (presence, and where documented, channels and arbitration). Do not invent channel counts, descriptor registers, or burst sizes that are not documented.

## Sources

- https://en.wikipedia.org/wiki/Ricoh_2A03
- https://www.nesdev.org/wiki/CPU
