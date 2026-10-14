/* ==========================================================================
 * slvm5_policy.c  --  SLVM/5 "Manager" policy-snapshot validation.
 *
 * The Manager generation's distinguishing contract is reproducible,
 * policy-verifiable, fault-tolerant execution: a program runs under a signed,
 * time-bounded, immutable policy snapshot. Before the VM admits a program it
 * validates that snapshot against the current time. Implements slvm5_policy.h.
 *
 * Contract (slvm5_policy_validate): a snapshot is valid iff it is non-NULL,
 * carries a non-zero version, names its content hash and issuer, is marked
 * immutable, and `now_ns` lies within [effective_at_ns, expires_at_ns).
 * Returns 0 on success, -1 otherwise.
 * ========================================================================== */
#include "slvm5_policy.h"

int slvm5_policy_validate(const slvm5_policy_snapshot_t *s, uint64_t now_ns) {
    if (!s) return -1;
    if (s->version == 0u) return -1;
    if (!s->hash || s->hash[0] == '\0') return -1;
    if (!s->issuer || s->issuer[0] == '\0') return -1;
    if (!s->immutable) return -1;
    if (s->expires_at_ns != 0u && now_ns >= s->expires_at_ns) return -1;
    if (now_ns < s->effective_at_ns) return -1;
    return 0;
}
