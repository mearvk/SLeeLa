# Intel Itanium Timing

## EPIC timing

The timing model tracks:

- bundle fetch;
- template decode;
- stop boundaries;
- predicate evaluation;
- issue groups;
- execution-unit latency;
- register dependencies;
- load/store latency;
- speculation/checking;
- cache/TLB behavior;
- RSE activity;
- interruption;
- retirement.

Unlike a conventional dynamically scheduled superscalar ISA, the IA-64 instruction stream communicates substantial scheduling information to the processor.

Concrete Itanium generations vary in pipeline depth, issue resources, cache latency, and speculation behavior.
