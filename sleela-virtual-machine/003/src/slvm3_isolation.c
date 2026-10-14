/* ==========================================================================
 * slvm3_isolation.c  --  SLVM/3 "Specialist" isolation-policy enforcement.
 *
 * The Specialist generation's distinguishing contract is verified execution:
 * deterministic isolation limits a program runs under. Before the VM admits a
 * program it validates the isolation policy, and each resource request is
 * charged against the policy's caps. Implements slvm3_isolation.h.
 *
 * slvm3_isolation_validate: a policy is well-formed iff it is non-NULL and
 *   declares at least one positive bound (a policy that bounds nothing is not a
 *   real isolation boundary). Returns 0 on success, -1 otherwise.
 * slvm3_isolation_charge(p, mem, net_bytes, file_bytes): a single request is
 *   admissible iff each amount is within its cap AND the matching allow_* flag
 *   permits it (network/file). Returns 0 if admissible, -1 if it would breach
 *   the isolation boundary.
 * ========================================================================== */
#include "slvm3_isolation.h"

int slvm3_isolation_validate(const slvm3_isolation_policy_t *p) {
    if (!p) return -1;
    /* An isolation policy must actually bound something. */
    if (p->max_memory == 0 && p->max_open_resources == 0 &&
        p->max_network_bytes == 0 && p->max_file_bytes == 0 &&
        p->max_execution_steps == 0 && p->max_wall_time_ms == 0) {
        return -1;
    }
    return 0;
}

int slvm3_isolation_charge(slvm3_isolation_policy_t *p,
                           uint64_t mem, uint64_t net_bytes, uint64_t file_bytes) {
    if (!p) return -1;
    if (mem > p->max_memory) return -1;
    if (net_bytes > 0) {
        if (!p->allow_network) return -1;
        if (net_bytes > p->max_network_bytes) return -1;
    }
    if (file_bytes > 0) {
        if (!p->allow_files) return -1;
        if (file_bytes > p->max_file_bytes) return -1;
    }
    return 0;
}
