# Motorola 68040 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction fetch/decode.
2. Instruction cache.
3. Integer register file.
4. Address-generation unit.
5. Integer execution units.
6. Floating-point unit.
7. Data cache.
8. MMU/translation.
9. Branch/control.
10. Exception/interrupt control.
11. Bus/burst interface.
12. Issue/pipeline control.
13. Write-back/completion.

## Couplers

SLCoupler models:

- fetch -> I-cache/decode;
- decode -> issue;
- registers -> integer/FPU;
- address generation -> MMU;
- MMU -> I/D cache;
- D-cache -> bus;
- FPU -> completion;
- exception -> processor state;
- DMA -> bus arbitration.

## Accuracy boundary

No 68060-specific superscalar depth, branch prediction, or cache topology is projected backward onto the 68040.

## Executable circuit model

`SL68040Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SL68040CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
