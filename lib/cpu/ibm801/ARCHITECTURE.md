# IBM 801 Architecture

## Execution path

SLIBM801CPU
-> instruction fetch
-> decode
-> register read
-> integer execution
-> branch/control
-> load/store
-> memory
-> completion

## RISC boundary

The 801 is represented as an early RISC architecture, not as a reduced version of later POWER hardware.

Later IBM cache, MMU, superscalar, or execution resources are not projected backward onto the 801.
