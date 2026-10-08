# NS32000 Architecture

## Execution path

SLNS32000CPU
-> fetch
-> variable-length decode
-> operand/effective-address generation
-> integer execution
-> optional FPU
-> memory management
-> bus
-> completion

## Family profiles

The 32000 family evolved across several implementations. Cache, MMU, pipeline, floating-point, and bus characteristics are therefore profile-specific.

SLeeLa does not project later implementation features onto the original NS32016.
