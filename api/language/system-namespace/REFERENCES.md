# Verified `ran::` References

This is an evidence-backed inventory, not a speculative list of desired system metrics.

| Reference | Evidence | Documentation status |
|---|---|---|
| `ran::mem` | `tests/sleela-syntax-1.9/ran-namespace-pass.sleela` | Accepted by the positive fixture. Confirm exact value type, units, and semantics in implementation before relying on them. |
| `ran::cpu` | `tests/sleela-syntax-1.9/ran-namespace-unknown.sleela` | Negative/unknown-name case; not a supported reference on the evidence inspected. |

## Adding a reference

For each new name, record its exact spelling, result type, units, sampling behavior, supported platforms, failure behavior, and implementation location. Add one positive fixture and at least one negative/type-boundary test where applicable. Update this file only after those checks exist.
