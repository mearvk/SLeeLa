# PA-RISC Architecture

## Datapath

SLPARISCCPU
-> instruction fetch
-> decoder
-> register file
-> integer/shift units
-> branch/control
-> load/store
-> FPU/MAX
-> MMU/TLB
-> cache/interconnect
-> trap/retirement

## PA-RISC 1.x

Early implementations can be represented as scalar or limited superscalar pipelines. PA-7100 and later 1.1 processors introduced superscalar execution. citeturn0search1

## PA-RISC 2.0

PA-8000 introduced a substantially wider out-of-order design with ten functional units and an Instruction Reorder Buffer. citeturn0search3

The generic model therefore exposes:

- issue/dispatch;
- dependency tracking;
- execution resources;
- load/store scheduling;
- reorder/retirement state.

Concrete widths remain processor-specific.

## Traps and interrupts

Precise interrupt/trap state is represented explicitly. Fast interrupt/shadow state belongs to the selected architectural profile.

## Load/store

Only memory operations access ordinary memory; arithmetic and logical operations operate on registers. citeturn0search1
