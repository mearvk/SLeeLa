#ifndef SLEELA_SLVM4_DISTRIBUTED_H
#define SLEELA_SLVM4_DISTRIBUTED_H
/* ==========================================================================
 * slvm4_distributed.h  --  SLVM/4 "Supervisor": authenticated and distributed
 * execution descriptor.
 *
 * The Supervisor generation's distinguishing contract is authenticated,
 * distributed execution: a program may run across a set of nodes, but only
 * nodes that are authenticated (identity + verified attestation) and admitted
 * into the execution quorum may participate. This header declares the node
 * descriptor and the validator the VM applies before admitting a program.
 * ========================================================================== */
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* One participating execution node in the distributed VM. */
typedef struct {
    uint64_t    node_id;            /* non-zero node identity                 */
    uint64_t    cluster_id;         /* non-zero cluster the node belongs to   */
    uint64_t    epoch;              /* membership epoch the node joined at     */
    const char *identity;           /* authenticated node identity (e.g. SPKI) */
    const char *attestation_hash;   /* hash of the node's verified attestation */
    uint8_t     authenticated;      /* identity proven                         */
    uint8_t     attested;           /* attestation verified                    */
    uint8_t     admitted;           /* admitted into the execution quorum      */
} slvm4_node_t;

/* The distributed execution set the program runs over. */
typedef struct {
    uint64_t            execution_id;   /* non-zero distributed-execution id   */
    uint64_t            current_epoch;  /* current membership epoch            */
    const slvm4_node_t *nodes;          /* participating nodes                 */
    uint32_t            node_count;     /* number of nodes                     */
    uint32_t            quorum;         /* nodes required to admit execution   */
} slvm4_execution_t;

/* Validate one node: non-NULL, real ids, named identity + attestation hash,
 * authenticated && attested && admitted, and not from a stale epoch. Returns 0
 * if the node may participate, -1 otherwise. */
int slvm4_node_validate(const slvm4_node_t *node, uint64_t current_epoch);

/* Validate the distributed execution: a real execution id, a positive quorum,
 * and at least `quorum` nodes that each pass slvm4_node_validate against
 * `current_epoch`. Returns 0 if execution is admissible, -1 otherwise. */
int slvm4_execution_validate(const slvm4_execution_t *exec);

#ifdef __cplusplus
}
#endif
#endif
