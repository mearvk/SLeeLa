# x86 / x86-64 Architecture

## Front end

SLX86CPU
-> instruction fetch
-> prefix scanner
-> variable-length decoder
-> micro-operation/operation representation
-> execution scheduling

Unlike the fixed-width PowerPC/MIPS profiles, the decoder must determine instruction length before execution.

## Execution domains

The functional model exposes:

- integer ALU;
- address-generation;
- branch/control;
- load/store;
- x87;
- SIMD/vector;
- system/privileged execution.

Not every implementation has every domain.

## Memory pipeline

The memory path is:

effective address
-> segmentation/linearization where applicable
-> paging/MMU
-> TLB/cache hierarchy
-> system interconnect
-> physical memory/device.

Intel documents TLBs and paging-structure caches as mechanisms that accelerate address translation. citeturn0search26

## Out-of-order boundary

Modern x86 CPUs may translate architectural instructions into internal operations and execute them out of order. SLeeLa represents this as an implementation-level scheduling/completion layer, not as an architectural requirement.

## Compatibility

The same SLeeLa family object can describe legacy and 64-bit modes through explicit mode state, while concrete CPU variants select which instructions/extensions are enabled.
