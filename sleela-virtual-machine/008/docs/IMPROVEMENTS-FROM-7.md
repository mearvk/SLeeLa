# Improvements from SLVM/7 to SLVM/8

| SLVM/7 | SLVM/8 |
|---|---|
| Managers provide independent contracts | Supervisor coordinates their lifecycle |
| Required manager checks | Admission gate before execution |
| Resource pressure | Resource admission before execution |
| Policy is evidence | Policy is an explicit immutable input |
| Boundary capabilities | Expiring capability leases |
| Checkpoint/recovery controls | Supervisor-coordinated recovery |
| Logs and evidence | Chained security audit records |
| Fault handling | Deterministic lifecycle transitions |
| Runtime actions | Explicit transactions |

SLVM/8 does not replace the SLeeLa Core interpreter. It surrounds the common Core execution model with a stronger admission and supervision contract.
