# ESA/390 Logical Circuit Abstraction

## Scope

A functional logic abstraction for simulation and teaching, not a transistor-level System/390 netlist.

## Blocks

- Instruction fetch and decode.
- General-purpose register file and execution units.
- ESA/390 PSW, condition-code, and interruption control.
- Address generation and optional translation/protection path.
- Profile-enabled floating-point and control-state units.
- Storage interface and channel-I/O request interface.

## Profile control

Instantiate only blocks justified by the selected ESA/390 model and enabled facilities. Do not infer physical gate counts, clock rate, pipeline depth, power, or undocumented cache layout.

## Executable circuit model

`SLESAS390Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLESAS390CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
