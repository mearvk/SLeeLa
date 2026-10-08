# DEC Alpha Circuit Model

Functional architecture/microarchitecture model, not transistor reconstruction.

Blocks: fetch, decoder, integer/FP register files, integer/FP units, shift/byte unit, multiply/divide, branch unit, load/store, issue/scheduling, MMU/TLB, caches, PAL interface, trap/interrupt control, retirement, interconnect, clock.

Couplers connect decode to registers, registers to execution, branches to PC, load/store to MMU, MMU to TLB/cache, issue to execution, execution to retirement, PAL to privileged state, cache to interconnect, and DMA to arbitration.