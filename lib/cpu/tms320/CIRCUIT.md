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

## Executable circuit model

`SLTMS320Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLTMS320CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
