# Motorola 68020 Circuit Model

## Functional scope

Architectural/functional model, not transistor reconstruction.

## Major blocks

1. Instruction prefetch.
2. Instruction cache.
3. Decoder.
4. Effective-address generator.
5. Data register file.
6. Address register file.
7. Integer/shift unit.
8. Condition-code logic.
9. Branch/exception controller.
10. Bus interface.
11. Coprocessor interface.
12. Clock/pipeline control.

## Couplers

SLCoupler models:

- prefetch -> cache/decode;
- decode -> register files;
- address registers -> effective-address unit;
- effective address -> bus;
- ALU -> condition codes;
- exception -> supervisor/stack state;
- coprocessor -> execution/control;
- DMA -> bus arbitration.

## Accuracy boundary

No 68030 MMU, 68040 superscalar pipeline, or later 68060 implementation detail is projected backward onto the 68020.

## Executable circuit model

`SL68020Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SL68020CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
