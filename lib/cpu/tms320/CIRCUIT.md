# Texas Instruments TMS320 Circuit Model

## Functional scope

Functional DSP model, not transistor-level reconstruction.

## Configurable blocks

1. Program fetch/decode.
2. Profile-specific register files.
3. Address-generation units.
4. Multiplier and multiply-accumulate unit.
5. Accumulator/arithmetic logic.
6. Program-memory interface.
7. Data-memory interface.
8. Circular/repeat-loop control.
9. Interrupt/peripheral interface.
10. Clock and pipeline sequencing.

## Couplers

SLCoupler models fetch-to-decode, address-to-data-memory, multiplier-to-accumulator, program/data-memory traffic, loop-control-to-fetch, and peripheral/DMA-to-system-bus paths.
