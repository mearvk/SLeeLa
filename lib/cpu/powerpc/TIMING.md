# PowerPC Timing

## Timing model

PowerPC implementations differ substantially in pipeline depth, dispatch width, execution latency, cache latency, and completion behavior.

SLeeLa therefore records timing as implementation data rather than asserting a universal cycle count.

The timing record can include:

- fetch cycle;
- decode/dispatch;
- execution unit;
- dependency/issue wait;
- cache hit/miss;
- memory transaction;
- branch result;
- completion;
- exception/interrupt;
- bus arbitration.

## Fixed instruction size

Instructions are 32-bit words, giving a stable fetch/decode granularity. citeturn0search9

## Branch timing

Branch processing is modeled independently because CR/CTR/LR state can affect control flow. Conditional branch forms may also update LR when LK is set. citeturn0search11

## Accuracy boundary

A PowerPC 601-style timing model must not be presented as a 604/750/970 timing model. Each concrete implementation gets its own timing profile.
