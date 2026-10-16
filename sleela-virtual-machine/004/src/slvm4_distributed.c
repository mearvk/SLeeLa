/* ==========================================================================
 * slvm4_distributed.c  --  SLVM/4 "Supervisor" authenticated/distributed
 * execution validation. Implements slvm4_distributed.h.
 *
 * A node may participate only if authenticated (identity proven), attested
 * (verified attestation), admitted into the quorum, and current (its join epoch
 * is not older than the cluster's current epoch). A distributed execution is
 * admissible only if a real execution id and a positive quorum are declared and
 * at least `quorum` participating nodes pass node validation.
 * ========================================================================== */
#include "slvm4_distributed.h"

int slvm4_node_validate(const slvm4_node_t *n, uint64_t current_epoch) {
    if (!n) return -1;
    if (n->node_id == 0u || n->cluster_id == 0u) return -1;
    if (!n->identity || n->identity[0] == '\0') return -1;
    if (!n->attestation_hash || n->attestation_hash[0] == '\0') return -1;
    if (!n->authenticated || !n->attested || !n->admitted) return -1;
    if (n->epoch < current_epoch) return -1;   /* superseded membership */
    return 0;
}

int slvm4_execution_validate(const slvm4_execution_t *e) {
    if (!e) return -1;
    if (e->execution_id == 0u) return -1;
    if (e->quorum == 0u) return -1;
    if (!e->nodes || e->node_count == 0u) return -1;
    uint32_t healthy = 0;
    for (uint32_t i = 0; i < e->node_count; i++) {
        if (slvm4_node_validate(&e->nodes[i], e->current_epoch) == 0) {
            healthy++;
        }
    }
    return (healthy >= e->quorum) ? 0 : -1;
}
