# DEC VAX Architecture

## Execution path

SLVAXCPU
-> instruction fetch
-> variable-length decode
-> operand/address generation
-> integer/FP/string execution
-> memory-management
-> bus
-> completion

## Addressing

VAX's orthogonal instruction design exposes many operand forms. SLeeLa keeps the instruction decoder, operand specification, effective-address generation, and execution units separate so the architecture can be represented without inventing a modern pipeline.

## Profiles

VAX-11 and later VAX implementations may differ substantially internally. Cache, pipeline, memory-controller, and bus characteristics therefore remain profile-specific.
