# Syntax 1.9 GC-Hint Test Matrix

Canonical fixture directory: [`tests/sleela-syntax-1.9/`](../../../tests/sleela-syntax-1.9/).

| Fixture | Expected role | Contract witnessed |
|---|---|---|
| `gc-hint-pass.sleela` | Pass | Standard numeric and bare hints preserve program output. |
| `gc-hint-contextual.sleela` | Pass | `gc` can remain an ordinary identifier outside the hint position. |
| `gc-hint-liveness.sleela` | Pass | A still-reachable value survives a full hint; expected field value is 777 per fixture README. |
| `gc-hint-stress-structs.sleela` | Pass | Repeated hints across 20,000 short-lived structs preserve the expected sum 199990000. |
| `gc-hint-stress-arrays.sleela` | Pass | 100,000 arrays with full hints preserve expected sum 4999950000 and exercise bounded VM-local array handles. |
| `gc-hint-range-invalid.sleela` | Reject | Out-of-range aggressiveness is rejected explicitly. |
| `gc-hint-version-invalid.sleela` | Reject | The hint is rejected under syntax 1.8. |
| `ran-namespace-pass.sleela` | Pass | A known system reference (`ran::mem`) is accepted. |
| `ran-namespace-unknown.sleela` | Reject | An unknown reference (`ran::cpu`) is rejected. |

## Execution note

This matrix records the intended fixture roles from the repository's fixture README and names. It does not assert that CI has passed in the current commit. Run the repository's supported syntax test command and retain the actual exit status/output when validating changes.
