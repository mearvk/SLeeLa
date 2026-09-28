# SLeeLa Debugger — Conformance Suite

Version: 0.8.0

The conformance suite turns the 1-2-3-4 lifecycle into a machine-checkable release contract.

## Lifecycle
1. Model — contract and data model exist.
2. Implement — behavior exists in source.
3. Integrate — connected to the execution/backend path.
4. Verify — executable evidence demonstrates the behavior.

A capability must not be reported as production-ready merely because its API exists.

## Capability matrix

| Capability | Model | Implement | Integrate | Verify |
|---|---|---|---|---|
| Breakpoints | Required | Required | Backend-dependent | Required |
| Watchpoints | Required | Required | Backend-dependent | Required |
| Threads | Required | Required | Backend-dependent | Required |
| Symbols/source | Required | Required | DWARF/PDB/etc. | Required |
| Memory | Required | Required | Backend-dependent | Required |
| Crash evidence | Required | Required | Platform-dependent | Required |
| Replay | Required | Required | Backend-dependent | Deterministic test |
| Sanitizers | Required | Required | Adapter-dependent | Fixture test |
| Coverage | Required | Required | Test integration | Report test |
| DAP | Required | Interface | Adapter | Protocol suite |

## Native truth

Portable domain state is not equivalent to native operating-system support. The conformance result must identify platform, backend, executable identity, source revision, and test evidence.

## Release gate

A release candidate requires passing conformance records for every capability advertised at its declared status.