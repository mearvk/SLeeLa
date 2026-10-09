# Cortex-M Timing and Exceptions

## Timing policy

Timing depends on core variant, instruction mix, memory placement, wait states, bus contention, debug configuration, and optional extensions. The simulator should separate architectural correctness from cycle-accurate timing.

## Evidence classes

- `documented`: from the selected core and system documentation.
- `derived`: calculated from documented rules with assumptions stated.
- `estimated`: approximate model for planning or simulation.
- `unspecified`: insufficient target evidence.

## Sources of variable latency

- Flash/SRAM/peripheral wait states and bus arbitration.
- Branches, instruction fetch boundaries, and pipeline refill.
- Exception entry/return, stacking, lazy FPU context preservation, and tail-chaining when supported.
- Cache misses and maintenance where caches exist.
- DSP/FPU/MVE instructions according to the selected implementation.

## Interrupt behavior

Priorities and masking rules follow the configured NVIC/profile. Model architectural exception state precisely; model latency only to the evidence level available. Late arrival and tail-chaining are not universal timing constants.

## Output

Timing reports must identify profile, variant, clock, memory assumptions, enabled extensions, and evidence level. Do not publish a single family-wide cycles-per-instruction figure.
