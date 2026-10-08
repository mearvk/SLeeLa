# x86 / x86-64 Circuit Model

## Scope

Functional architectural/microarchitectural boundary model; not a transistor-level reconstruction.

## Major blocks

1. General-register file.
2. Instruction pointer.
3. Flags/condition logic.
4. Segment and descriptor state.
5. Variable-length instruction fetch/decode.
6. Prefix and opcode decoder.
7. Micro-operation/operation representation.
8. Integer execution units.
9. Address-generation units.
10. Branch/control unit.
11. Load/store unit.
12. x87/vector execution where implemented.
13. Paging/MMU.
14. TLB and paging-structure caches.
15. Cache hierarchy.
16. Control/debug/MSR state.
17. Interrupt/exception unit.
18. Retirement/completion state.
19. System interconnect.
20. Clock/control.

## Couplers

SLCoupler exposes:

- decoder -> execution operation;
- register -> ALU;
- register -> address generation;
- address -> segmentation/paging;
- paging -> TLB/cache;
- cache -> load/store;
- branch -> fetch/IP;
- flags -> conditional execution;
- control registers -> MMU;
- exception -> descriptor/interrupt state;
- retirement -> architectural register state.

## Accuracy boundary

No exact Intel or AMD internal pipeline width, cache topology, port count, transistor structure, or speculative mechanism is claimed by the generic x86 object.
