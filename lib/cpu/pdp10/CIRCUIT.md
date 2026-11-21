# DEC PDP-10 Circuit Model

Functional architectural simulation only, not transistor-level reconstruction.

Major blocks: instruction fetch/register, decoder, 36-bit accumulator file, effective-address unit, byte-pointer unit, arithmetic/logic execution, memory interface, I/O interface, interrupt/control logic, clock/sequencing.

SLCoupler models fetch-to-decode, decode-to-accumulator, accumulator-to-execution, byte-pointer-to-address, execution-to-memory, I/O-to-control, and interrupt-to-processor paths. Implementation-specific details are not inferred beyond documented profiles.

## Executable circuit model

`SLPDP10Circuit.sleela` carries this circuit in source. It states the logic family and
voltage thresholds, the supply rail, the clock and its phases, the functional
blocks with width, depth, gate and transistor counts and derived area, two
timing paths with stage delays, four monitored nets with signal-quality
checks, and three semaphores for the shared latch and buses. `SLPDP10CPU.circuitModel()`
returns it built and clocked.

The class header marks every figure as recalled or derived. Recalled figures
come from general knowledge and still need checking against a primary source.
Derived figures are planning numbers, not measurements. The model is at
functional-block level, not a transistor netlist.

Circuit grade: **C** (6/10), with 0 verification violations, from
`circuitScore()` in `SLCircuitModel`. `lib/cpu/examples/circuit-grades.sleela`
grades every CPU.
