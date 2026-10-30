#ifndef SLEELA_PHILOSOPHY_PARTITION_API_H
#define SLEELA_PHILOSOPHY_PARTITION_API_H
/*
 * Uniform three-level API for the philosophy event tree.
 *
 * One interface is applied identically at three levels (consistent OOD):
 *   - BRANCH : an individual node/event, as it occurs.
 *   - PARTITION : a named group of branches.
 *   - SYSTEM : the whole tree's branching nature (height and maturations).
 *
 * A partition's behavior is the composition of its branches'; the system's is
 * the composition of its partitions'. The same eight operations and the same
 * eleven fired activities exist at every level.
 *
 * Eight operations:
 *   add, delete, prune, renegotiate, system, analysis, control, feature.
 *
 * Eleven activities fired via a dispatch table on each operation:
 *   workers, listeners, controllers, event-handlers, exchanger,
 *   memory-analysis, saving, retirement, scandal, nursing, pro-bono.
 *
 *   - scandal  : raise an audit/alarm when a target violates an invariant
 *                (too many traits, weight out of [1,9], over-height,
 *                 or more than the 3-maturation cap).
 *   - nursing  : remediation pass that repairs/rebalances a degraded or
 *                inconsistent target back to consistency.
 *   - pro-bono : low-priority, no-cost opportunistic maintenance pass
 *                (prune/renegotiate when the target is idle).
 *
 * The whole tree matures ("grows indistinguished") at most SL_TREE_MATURATION_MAX
 * (3) times across its whole nature; the usual case is 1. The system basis is a
 * small fan of SL_TREE_BASIS_NODES (2-3) nodes, not more.
 *
 * This layer is additive. It wraps event_tree / consistency_monitor /
 * signal_toss and leaves their APIs unchanged. It performs no I/O: "saving" is
 * an in-memory snapshot, consistent with the module's no-files rule.
 */
#include "consistency_monitor.h"
#include "event_tree.h"
#include "signal_toss.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Whole-tree basis / maturation caps -------------------------------- */
#define SL_TREE_BASIS_NODES_MIN   2u
#define SL_TREE_BASIS_NODES_MAX   3u
#define SL_TREE_MATURATION_USUAL  1u
#define SL_TREE_MATURATION_MAX    3u
#define SL_PARTITION_NAME_CAPACITY 48u
#define SL_PARTITION_CAPACITY      32u

/* ---- Levels ------------------------------------------------------------ */
typedef enum sl_level {
    SL_LEVEL_BRANCH = 0,
    SL_LEVEL_PARTITION,
    SL_LEVEL_SYSTEM
} sl_level;

/* ---- Operations -------------------------------------------------------- */
typedef enum sl_op {
    SL_OP_ADD = 0,
    SL_OP_DELETE,
    SL_OP_PRUNE,
    SL_OP_RENEGOTIATE,
    SL_OP_SYSTEM,
    SL_OP_ANALYSIS,
    SL_OP_CONTROL,
    SL_OP_FEATURE,
    SL_OP_COUNT
} sl_op;

/* ---- Activities (dispatch table) --------------------------------------- */
typedef enum sl_activity {
    SL_ACT_WORKERS = 0,
    SL_ACT_LISTENERS,
    SL_ACT_CONTROLLERS,
    SL_ACT_EVENT_HANDLERS,
    SL_ACT_EXCHANGER,
    SL_ACT_MEMORY_ANALYSIS,
    SL_ACT_SAVING,
    SL_ACT_RETIREMENT,
    SL_ACT_SCANDAL,
    SL_ACT_NURSING,
    SL_ACT_PRO_BONO,
    SL_ACT_COUNT
} sl_activity;

/* Bitmask of activities. SL_ACT_ALL fires every activity an op supports. */
typedef uint32_t sl_activity_mask;
#define SL_ACT_BIT(activity) ((sl_activity_mask)1u << (activity))
#define SL_ACT_ALL ((sl_activity_mask)((1u << SL_ACT_COUNT) - 1u))

/* ---- Scientist-addressed research requests ----------------------------- *
 * The module does not encode IQ or any intelligence scale (see README 3.2).
 * Figures supplied as "IQ" are recorded here only as open requests addressed
 * to the scientific community to establish the base merit of this series. */
#define SL_RESEARCH_TEXT_CAPACITY 192u
typedef struct sl_research_request {
    char subject[SL_RESEARCH_TEXT_CAPACITY];   /* what scientists are asked to assess */
    double claimed_figure;                     /* the supplied figure, e.g. 200.0 */
    bool validated;                            /* always false here: pending science */
} sl_research_request;

/* ---- Activity outcome -------------------------------------------------- */
typedef struct sl_activity_outcome {
    sl_activity activity;
    bool fired;
    bool ok;
    uint64_t magnitude;      /* op-specific count (nodes touched, pruned, etc.) */
} sl_activity_outcome;

typedef struct sl_op_result {
    sl_op op;
    sl_level level;
    sl_status status;
    sl_activity_mask requested;
    sl_activity_mask fired;
    sl_activity_outcome outcomes[SL_ACT_COUNT];
    sl_consistency_report consistency;
    uint64_t scandals;       /* invariant violations flagged this op */
    uint64_t nursed;         /* remediations performed this op */
} sl_op_result;

/* An observer invoked for every fired activity (the "event handlers"/"listeners"
 * seam). ctx is the user pointer registered on the engine. */
typedef void (*sl_activity_observer)(
    const sl_op_result *result, const sl_activity_outcome *outcome, void *ctx);

typedef struct sl_partition_engine sl_partition_engine;

/* ---- Lifecycle --------------------------------------------------------- */
sl_partition_engine *sl_partition_engine_create(uint64_t now);
void sl_partition_engine_destroy(sl_partition_engine *engine);
void sl_partition_engine_set_observer(
    sl_partition_engine *engine, sl_activity_observer observer, void *ctx);
void sl_partition_engine_set_now(sl_partition_engine *engine, uint64_t now);

/* Create/find named partitions (returns index, or -1 on failure/full). */
int sl_partition_create(sl_partition_engine *engine, const char *name);
int sl_partition_index(const sl_partition_engine *engine, const char *name);
size_t sl_partition_count(const sl_partition_engine *engine);

/* ---- The uniform operation entry point --------------------------------- *
 * Dispatches op at the given level, firing the activities in `activities`
 * that the op supports, and writes a full result. For SL_LEVEL_BRANCH and
 * SL_LEVEL_PARTITION, partition_index selects the partition; for
 * SL_LEVEL_SYSTEM it is ignored. `event` is used by SL_OP_ADD (branch level);
 * `id` is used by SL_OP_DELETE. */
sl_status sl_partition_dispatch(
    sl_partition_engine *engine, sl_level level, int partition_index,
    sl_op op, sl_activity_mask activities,
    const sl_event *event, uint64_t id, sl_op_result *out);

/* ---- Convenience wrappers for the eight ops (fire SL_ACT_ALL) ---------- */
sl_status sl_partition_add(sl_partition_engine *, int partition_index, const sl_event *, sl_op_result *);
sl_status sl_partition_delete(sl_partition_engine *, int partition_index, uint64_t id, sl_op_result *);
sl_status sl_partition_prune(sl_partition_engine *, int partition_index, double floor_w, sl_op_result *);
sl_status sl_partition_renegotiate(sl_partition_engine *, int partition_index, sl_op_result *);
sl_status sl_partition_system(sl_partition_engine *, int partition_index, sl_op_result *);
sl_status sl_partition_analysis(sl_partition_engine *, int partition_index, sl_op_result *);
sl_status sl_partition_control(sl_partition_engine *, int partition_index, bool run_workers, sl_op_result *);
sl_status sl_partition_feature(sl_partition_engine *, int partition_index, sl_activity activity, bool enable, sl_op_result *);

/* ---- System (whole-tree) helpers --------------------------------------- */
sl_status sl_system_op(sl_partition_engine *, sl_op, sl_activity_mask, sl_op_result *);
unsigned sl_system_maturations(const sl_partition_engine *engine);
unsigned sl_system_basis_nodes(const sl_partition_engine *engine);
/* Record an "IQ"/figure as an open request to scientists (never a score). */
sl_status sl_system_add_research_request(
    sl_partition_engine *engine, const char *subject, double figure);
size_t sl_system_research_request_count(const sl_partition_engine *engine);
bool sl_system_research_request_at(
    const sl_partition_engine *engine, size_t i, sl_research_request *out);

/* ---- Stringization ----------------------------------------------------- */
const char *sl_level_string(sl_level);
const char *sl_op_string(sl_op);
const char *sl_activity_string(sl_activity);

#ifdef __cplusplus
}
#endif
#endif
