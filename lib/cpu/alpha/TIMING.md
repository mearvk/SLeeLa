# DEC Alpha Timing

## Timing model

SLeeLa tracks:

- fetch;
- decode;
- register read;
- issue;
- integer execution;
- FP execution;
- branch prediction/result;
- load/store;
- cache hit/miss;
- TLB;
- dependency stalls;
- exception;
- retirement.

## Microarchitectural generations

EV4, EV5, and EV6 have materially different pipeline and issue behavior. EV6's out-of-order, superscalar design requires explicit scheduling and retirement state.

The generic Alpha implementation therefore does not claim one universal cycle count or pipeline depth.
