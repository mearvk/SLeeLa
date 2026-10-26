# Improvements from SLVM/9 to SLVM/10

SLVM/10 (**Principal**) turns SLVM/9 (**Authority**)'s native filesystem
*observation* into a verified storage *lifecycle*. It does not replace /8 or /9;
it composes them into a stronger verification boundary. The guiding principle:
**observation is not authority, and authority is not permanence.**

| Area | /9 | /10 |
|---|---|---|
| Identity | Mount/namespace identity | First-class verified storage identity |
| Generation | Generation tracking | Revalidated generations; fail-closed on stale |
| Operations | Capability-negotiated access | Operations carry required capabilities + verification state |
| Handles | Recovery-safe re-acquisition | Explicit revalidation for long-lived handles |
| Lifecycle | Observation + adapters | Quiesce + recovery as an explicit execution phase |
| Adapters | OS adapter contracts | Adapter qualification as an explicit contract |
| Durability | Durability reporting | A successful operation is not automatically durable |
| Agreement | Native observation | Storage/adapter/policy must agree with the admitted artifact |

The VM no longer merely asks "can the OS open this file?" — it asks whether the
storage identity, generation, adapter, policy, and requested operation still
agree with the admitted SLeeLa artifact. A previously valid identity must be
revalidated after a generation change, recovery, migration, or mount
replacement.

SLVM/10 remains an execution evolution, not a second language implementation.
