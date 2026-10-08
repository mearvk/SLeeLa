# PA-RISC Timing

## Architectural timing

The timing model records:

- fetch;
- decode;
- issue;
- integer execution;
- shift/bit operations;
- branch;
- load/store;
- FPU;
- cache;
- TLB;
- trap/interrupt;
- retirement.

PA-RISC 1.0 was fundamentally scalar, while later PA-RISC 1.1 processors became superscalar. PA-RISC 2.0 implementations such as PA-8000 reached four-way superscalar execution with out-of-order scheduling. citeturn0search1turn0search3

The generic implementation therefore does not claim one universal cycle count or pipeline depth.
