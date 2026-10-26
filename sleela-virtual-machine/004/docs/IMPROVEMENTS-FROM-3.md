# Improvements from SLVM/3 to SLVM/4

SLVM/4 (**Supervisor**) uses SLVM/3 (**Specialist**) as its verified-execution
and isolation base rather than replacing it, then extends execution across
authenticated, distributed nodes.

| Area | /3 | /4 |
|---|---|---|
| Execution | Single verified execution | Authenticated, distributed execution |
| Nodes | Local execution only | Per-node identity + verified attestation |
| Membership | — | Execution quorum over admitted nodes |
| Epochs | — | Membership epochs reject stale nodes |
| Identity | Supply-chain/artifact identity | Node + cluster identity binding |
| Attestation | Manifest-/policy-bound evidence | Per-node attestation-hash binding |
| Admission | Verified-artifact admission | Quorum admission validator before boot |
| Isolation | Sandbox/isolation profiles | Fault containment across the node set |

The validator (`slvm4_node_validate` and the execution-set check in
`include/slvm4_distributed.h`) admits a program only when at least the required
quorum of authenticated, attested, admitted, non-stale nodes agree; a malformed
set is rejected under `--selfcheck`.

SLVM/4 remains an execution evolution, not a second language implementation.
