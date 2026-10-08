# SPARC CPU

SLeeLa models SPARC as a family ISA with explicit V8 and V9 profiles. SPARC V9 supports 32- and 64-bit integer data, 32/64/128-bit floating-point data, and 32-bit instruction encoding. citeturn0search20

## Core architecture

- integer unit;
- floating-point unit;
- register windows;
- PC/nPC control-flow state;
- trap/privilege state;
- load/store unit;
- optional MMU/TLB;
- implementation-specific caches and interconnect.

SPARC V9 is deliberately separated from UltraSPARC-specific microarchitecture because the V9 architecture leaves implementation decisions to processors. citeturn0search22

## Profiles

- SPARC V7: historical baseline;
- SPARC V8: 32-bit architecture;
- SPARC V9: 64-bit extension;
- implementation extensions such as VIS remain explicit extensions.
