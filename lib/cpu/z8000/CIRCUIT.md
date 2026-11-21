# Zilog Z8000 Circuit Model

## Functional scope

Architectural functional model, not transistor-level reconstruction.

## Blocks

1. Instruction fetch and instruction register.
2. Variable-length decoder.
3. R0-R15 register file.
4. Effective-address generator.
5. Integer/logic execution unit.
6. Condition-code/status logic.
7. Z8001 segment unit where applicable.
8. Memory/I/O interface.
9. Interrupt/control logic.
10. Clock and sequencing.

## Couplers

SLCoupler models fetch-to-decode, decode-to-registers, registers-to-execution, address-to-memory, execution-to-status, interrupt-to-control, and DMA-to-bus-arbitration paths.

## Executable circuit model

`SLZ8000Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLZ8000CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **B** (7/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
