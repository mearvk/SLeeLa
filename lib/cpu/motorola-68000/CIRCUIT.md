# Motorola 68000 Circuit Model

## Scope

This is a functional architectural circuit model, not a transistor-for-transistor reconstruction.

## Major blocks

1. Eight data-register paths.
2. Eight address-register paths.
3. Program counter.
4. Status/control register.
5. ALU and condition-code generation.
6. Effective-address generation.
7. Instruction decode/control.
8. Internal operand/address movement.
9. External bus interface.
10. Interrupt/exception control.
11. Bus arbitration interface.
12. Clock/timing control.

## Couplers

SLeeLa uses SLCoupler to expose controlled movement between register files, effective-address logic, ALU, instruction state, and bus interfaces.

The external 16-bit transfer path is deliberately kept separate from the 32-bit programmer-visible datapath.

## Accuracy boundary

Only documented or independently validated die-level details should be promoted into a deeper circuit model.

## Executable circuit model

`SL68000Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SL68000CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **B** (8/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
