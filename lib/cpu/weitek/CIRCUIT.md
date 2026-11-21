# Weitek POWER Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch.
2. Decoder.
3. Integer register file.
4. Floating-point register file.
5. Integer execution.
6. Floating-point execution.
7. Load/store unit.
8. Branch/control unit.
9. Cache interface.
10. Optional MMU/TLB.
11. Exception/interrupt controller.
12. System-bus interface.
13. Clock/sequencing.

## Couplers

SLCoupler models:

- fetch -> decoder;
- decoder -> register files;
- register files -> execution;
- execution -> load/store;
- cache/MMU -> bus;
- branch -> fetch;
- exception -> processor state;
- DMA -> arbitration.

## Accuracy boundary

Weitek POWER remains a separate implementation profile and is not assigned IBM POWER or PowerPC internals.

## Executable circuit model

`SLWeitekPowerCircuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLWeitekPowerCPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
