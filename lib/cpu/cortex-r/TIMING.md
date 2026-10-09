# Cortex-R Timing and Real-Time Analysis

## No universal cycle count

Cycle timing depends on the exact core, instruction mix, cache/TCM configuration, memory placement, bus contention, interrupts, and peripheral behavior. A generic Cortex-R profile cannot guarantee worst-case execution time.

## Evidence levels

- `documented`: published for the selected core or SoC.
- `derived`: computed from documented facts and explicit assumptions.
- `estimated`: approximate simulation value.
- `unspecified`: insufficient target evidence.

## Sources of variability

- Cache hits, misses, line fills and write-buffer behavior.
- TCM versus external memory access.
- Flash and SRAM wait states.
- Interconnect arbitration and DMA traffic.
- Exception entry/return and interrupt nesting.
- FPU/DSP instructions and optional extensions.
- Debug, trace, and implementation-specific stalls.

## Analysis report

Every timing report identifies the exact core profile, clock, memory regions, cache and TCM configuration, interrupt assumptions, bus model, and evidence class. Distinguish average latency from a justified upper bound. Never label an estimate as a guaranteed WCET.
