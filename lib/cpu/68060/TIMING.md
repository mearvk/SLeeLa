# Motorola 68060 Timing

Tracked events:

- instruction fetch;
- instruction-buffer access;
- decode;
- branch prediction;
- dispatch;
- integer issue/execution;
- address generation;
- FPU execution;
- I-cache/D-cache;
- MMU translation;
- bus/burst;
- exception/interrupt;
- completion.

The 68060 is modeled as a superscalar processor with explicit scheduling and branch resources, without inventing undocumented implementation timing.
