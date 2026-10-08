# PowerPC Architecture

## Logical processing

The PowerPC logical processor contains branch processing, fixed-point processing, and floating-point processing, with sequencing/control for instruction fetch, execution, and interrupt action. IBM describes this as the principal logical processing organization. citeturn0search1

SLeeLa decomposes it into:

SLPowerPCCPU
-> branch processor
-> fixed-point processor
-> floating-point processor
-> load/store
-> storage/interconnect
-> exception/interrupt control

## Branch processor

The branch subsystem owns:

- Condition Register;
- Link Register;
- Count Register;
- branch target generation;
- condition testing;
- link/return state.

## Fixed-point processor

The integer unit operates on the 32-bit GPR set and provides arithmetic, logical, compare, rotate/shift, and address-generation operations.

## Floating-point processor

The FPU uses the 32-entry FPR file and FPSCR. It remains a replaceable implementation component because embedded PowerPC processors may omit floating-point support. The MPC860 documentation, for example, explicitly identifies unsupported floating-point instructions. citeturn0search7

## Pipeline

SLPipeline represents instruction fetch, decode/dispatch, execute, memory, and completion/writeback phases as a functional model. Actual stage count and out-of-order behavior belong to each concrete PowerPC implementation.

## Exceptions

The exception controller couples MSR and supervisor state to vector selection, save/restore state, interrupt recognition, and privileged control.
