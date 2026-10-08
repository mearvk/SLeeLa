# Intel Itanium Architecture

## Datapath

SLItaniumCPU
-> instruction fetch
-> bundle/template decoder
-> predicate/register files
-> execution clusters
-> load/store
-> register-stack/speculation machinery
-> MMU/TLB
-> cache/interconnect
-> retirement/architectural state

## Bundle execution

A bundle supplies three instruction slots. The template describes instruction-slot categories and stop information.

SLeeLa represents each bundle as a scheduling object rather than pretending the three slots are ordinary x86-style variable-length instructions.

## Predicate execution

Each instruction has predicate context. A false predicate suppresses the architectural operation while preserving the instruction's place in the scheduled stream.

## Register rotation

Procedure calls and software-pipelined loops can rotate portions of the register file. Rotation is represented explicitly in the register-window/rename layer.

## EPIC scheduling

The generic model exposes issue groups, stop boundaries, execution resources, dependencies, and latency. Actual Itanium processor issue width and cluster topology remain implementation-specific.

## Compatibility

IA-32 compatibility is kept as a separate execution subsystem/profile and is not merged into the IA-64 decoder.
