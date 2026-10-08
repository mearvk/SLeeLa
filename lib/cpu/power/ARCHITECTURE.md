# IBM POWER Architecture

## Processing organization

SLPOWERCPU
-> instruction fetch
-> branch processing
-> fixed-point processing
-> floating-point processing
-> storage
-> completion

IBM's logical POWER processing model separates branch, fixed-point, and floating-point processing around storage. citeturn0search0

## POWER2

POWER2 adds additional fixed-point capability and an enhanced floating-point subsystem. IBM documentation records two 64-bit floating-point execution units and additional instructions in POWER2 implementations. citeturn0search19

## Instruction encoding

POWER instructions are 32-bit and word aligned. The architecture contains multiple instruction formats, including D, I, B, X, XL, and related forms. citeturn0search3turn0search7

## Timing model

The generic model exposes fetch, branch, fixed-point, floating-point, storage, exception, and completion events without claiming one universal implementation pipeline.
