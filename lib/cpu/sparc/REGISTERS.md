# SPARC Registers

## Integer window

The architectural window view contains:

- %g0-%g7;
- %o0-%o7;
- %l0-%l7;
- %i0-%i7.

%g0 is architecturally zero. Windowed registers overlap between adjacent windows. citeturn0search9

## Control state

### V8-oriented
- PC
- nPC
- PSR
- WIM
- TBR
- Y

### V9-oriented
- PC
- nPC
- CCR
- CWP
- PIL
- TBA
- TT
- VER
- ASI
- CANSAVE
- CANRESTORE
- CLEANWIN
- FPRS
- OTHERWIN
- PSTATE
- TICK
- TL
- TPC/TNPC
- TSTATE
- WSTATE

V9 removed/reworked several V8 state registers while adding the expanded window/trap/privilege state. citeturn0search0

## Floating point

The FPU register file supports single/double/quad views according to the selected SPARC profile. V9 adds extended floating-point register state. citeturn0search1
