# Motorola 68040 Timing

Tracked events:

- instruction fetch;
- decode;
- address generation;
- integer issue;
- integer execution;
- floating-point execution;
- I-cache;
- D-cache;
- MMU translation;
- bus/burst;
- exception/interrupt;
- write-back/retirement.

The 68040 is modeled as superscalar but not as a generic modern out-of-order architecture.
