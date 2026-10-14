/* ==========================================================================
 * slvm6_lease.c  --  SLVM/6 "Director" execution-lease validation.
 *
 * The Director generation's distinguishing contract is continuously verified,
 * migratable execution: an object is held under a time-bounded, revocable lease
 * tied to a revocation epoch. Before the VM admits a program it validates the
 * lease against the current time and the current revocation epoch. Implements
 * slvm6_lease.h.
 *
 * Contract (slvm6_lease_validate): a lease is valid iff it is non-NULL, names a
 * real object and lease id, names a holder, is not revoked, `now_ns` lies
 * within [issued_at_ns, expires_at_ns), and its revocation_epoch is current
 * (>= current_revocation_epoch — a lease from an older epoch has been
 * superseded). Returns 0 on success, -1 otherwise.
 * ========================================================================== */
#include "slvm6_lease.h"

int slvm6_lease_validate(const slvm6_lease_t *l, uint64_t now_ns,
                         uint64_t current_revocation_epoch) {
    if (!l) return -1;
    if (l->object_id == 0u || l->lease_id == 0u) return -1;
    if (!l->holder || l->holder[0] == '\0') return -1;
    if (l->revoked) return -1;
    if (now_ns < l->issued_at_ns) return -1;
    if (l->expires_at_ns != 0u && now_ns >= l->expires_at_ns) return -1;
    if (l->revocation_epoch < current_revocation_epoch) return -1;
    return 0;
}
