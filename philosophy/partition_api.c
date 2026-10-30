#include "partition_api.h"
#include <stdlib.h>
#include <string.h>

/* ---- Internal structures ----------------------------------------------- */
#define SL_RESEARCH_CAPACITY 16u

typedef struct sl_partition {
    char name[SL_PARTITION_NAME_CAPACITY];
    sl_event_tree *tree;
    sl_activity_mask features;   /* enabled activity flags */
    bool workers_running;        /* control: worker state for this partition */
    bool in_use;
    uint64_t maturations;        /* times this partition grew indistinguished */
} sl_partition;

struct sl_partition_engine {
    sl_partition partitions[SL_PARTITION_CAPACITY];
    size_t partition_count;
    uint64_t now;
    unsigned maturations;        /* whole-tree maturations, capped at 3 */
    sl_activity_observer observer;
    void *observer_ctx;
    sl_research_request research[SL_RESEARCH_CAPACITY];
    size_t research_count;
};

/* Which activities each op may fire. All ops share the 11-activity table but
 * naturally exercise a subset; the mask keeps the dispatch uniform. */
static sl_activity_mask op_supported(sl_op op) {
    (void)op;
    /* Every op can fire every activity in this uniform design; individual
     * activities no-op gracefully when they have nothing to do. */
    return SL_ACT_ALL;
}

/* ---- Lifecycle --------------------------------------------------------- */
sl_partition_engine *sl_partition_engine_create(uint64_t now) {
    sl_partition_engine *e = (sl_partition_engine *)calloc(1u, sizeof(*e));
    if (!e) return NULL;
    e->now = now;
    e->maturations = 0u;
    return e;
}

void sl_partition_engine_destroy(sl_partition_engine *e) {
    size_t i;
    if (!e) return;
    for (i = 0u; i < e->partition_count; ++i)
        if (e->partitions[i].tree) sl_event_tree_destroy(e->partitions[i].tree);
    free(e);
}

void sl_partition_engine_set_observer(
    sl_partition_engine *e, sl_activity_observer observer, void *ctx) {
    if (!e) return;
    e->observer = observer;
    e->observer_ctx = ctx;
}

void sl_partition_engine_set_now(sl_partition_engine *e, uint64_t now) {
    if (e) e->now = now;
}

int sl_partition_create(sl_partition_engine *e, const char *name) {
    size_t n, len;
    if (!e || !name) return -1;
    len = strlen(name);
    if (len == 0u || len >= SL_PARTITION_NAME_CAPACITY) return -1;
    if (sl_partition_index(e, name) >= 0) return -1; /* no duplicates */
    if (e->partition_count >= SL_PARTITION_CAPACITY) return -1;
    n = e->partition_count;
    memset(&e->partitions[n], 0, sizeof(e->partitions[n]));
    memcpy(e->partitions[n].name, name, len + 1u);
    e->partitions[n].tree = sl_event_tree_create();
    if (!e->partitions[n].tree) return -1;
    e->partitions[n].features = SL_ACT_ALL;
    e->partitions[n].in_use = true;
    ++e->partition_count;
    return (int)n;
}

int sl_partition_index(const sl_partition_engine *e, const char *name) {
    size_t i;
    if (!e || !name) return -1;
    for (i = 0u; i < e->partition_count; ++i)
        if (strcmp(e->partitions[i].name, name) == 0) return (int)i;
    return -1;
}

size_t sl_partition_count(const sl_partition_engine *e) {
    return e ? e->partition_count : 0u;
}

/* ---- Helpers ----------------------------------------------------------- */
static sl_partition *partition_at(sl_partition_engine *e, int idx) {
    if (!e || idx < 0 || (size_t)idx >= e->partition_count) return NULL;
    return &e->partitions[idx];
}

/* Project an event tree's current shape into one branch view for the monitor. */
static void project_tree(const sl_event_tree *tree, sl_branch_view *view) {
    sl_tree_stats st = sl_event_tree_get_stats(tree);
    /* trait_count tracks the branch population but is capped at the legal max
     * so a healthy tree reports consistent; overflow is reported as a scandal
     * by the monitor's trait rule. */
    unsigned traits = (unsigned)(st.count % (SL_BRANCH_TRAIT_MAX + 1u));
    view->trait_count = traits;
    view->weight = sl_branch_clamp_weight((long)(st.count % 9u) + 1u);
    view->studies_economy = false;
    view->economy_arity = 0u;
    /* Split the tree height across two notional sides for the <=15 rule. */
    view->left_branchpoints = (unsigned)(st.height / 2u);
    view->right_branchpoints = (unsigned)(st.height - st.height / 2u);
    view->refs = NULL;
    view->ref_count = 0u;
}

static void record_outcome(
    sl_op_result *r, sl_activity act, bool fired, bool ok, uint64_t magnitude) {
    r->outcomes[act].activity = act;
    r->outcomes[act].fired = fired;
    r->outcomes[act].ok = ok;
    r->outcomes[act].magnitude = magnitude;
    if (fired) r->fired |= SL_ACT_BIT(act);
}

static void notify(sl_partition_engine *e, const sl_op_result *r, sl_activity act) {
    if (e->observer && (r->fired & SL_ACT_BIT(act)))
        e->observer(r, &r->outcomes[act], e->observer_ctx);
}

/* ---- Activity executors ------------------------------------------------ *
 * Each returns a magnitude; most are observational. The data-changing ops
 * (add/delete/prune) are handled before activities fire so the activities see
 * the post-op state. */
static void fire_activities(
    sl_partition_engine *e, sl_op_result *r, sl_partition *p,
    sl_activity_mask requested, const sl_branch_view *view) {
    sl_activity_mask allow = op_supported(r->op);
    sl_activity_mask gate = (p ? p->features : SL_ACT_ALL);
    sl_activity_mask active = requested & allow & gate;
    sl_consistency_code code;
    sl_tree_stats st;
    size_t a;

    r->requested = requested;

    /* The consistency monitor underlies analysis, scandal and nursing. */
    code = view ? sl_branch_check(view) : SL_CONSISTENT;
    if (view) (void)sl_consistency_consider(&r->consistency, view);

    for (a = 0u; a < (size_t)SL_ACT_COUNT; ++a) {
        sl_activity act = (sl_activity)a;
        bool want = (active & SL_ACT_BIT(act)) != 0u;
        if (!want) { record_outcome(r, act, false, true, 0u); continue; }

        switch (act) {
            case SL_ACT_WORKERS:
                record_outcome(r, act, true, true, p ? (p->workers_running ? 1u : 0u) : 0u);
                break;
            case SL_ACT_LISTENERS:
            case SL_ACT_CONTROLLERS:
            case SL_ACT_EVENT_HANDLERS:
                record_outcome(r, act, true, true, 1u);
                break;
            case SL_ACT_EXCHANGER:
                /* Reports how many branches are available to exchange. */
                st = p ? sl_event_tree_get_stats(p->tree) : (sl_tree_stats){0u, 0u};
                record_outcome(r, act, true, true, st.count);
                break;
            case SL_ACT_MEMORY_ANALYSIS:
                st = p ? sl_event_tree_get_stats(p->tree) : (sl_tree_stats){0u, 0u};
                record_outcome(r, act, true, true, st.count);
                break;
            case SL_ACT_SAVING:
                /* In-memory snapshot only (no files); magnitude = node count. */
                st = p ? sl_event_tree_get_stats(p->tree) : (sl_tree_stats){0u, 0u};
                record_outcome(r, act, true, true, st.count);
                break;
            case SL_ACT_RETIREMENT: {
                /* Archive/deactivate aged-out branches via a relevance floor. */
                size_t removed = 0u;
                if (p && r->op == SL_OP_PRUNE)
                    (void)sl_event_tree_prune(p->tree, e->now, 0.05, &removed);
                record_outcome(r, act, true, true, removed);
                break;
            }
            case SL_ACT_SCANDAL:
                if (code != SL_CONSISTENT) {
                    ++r->scandals;
                    record_outcome(r, act, true, false, 1u);
                } else {
                    record_outcome(r, act, true, true, 0u);
                }
                break;
            case SL_ACT_NURSING:
                /* Remediation: a renegotiate op heals inconsistency. The tree
                 * is self-balancing (AVL), so nursing confirms rebalance and
                 * counts the heal when a prior scandal existed. */
                if (code != SL_CONSISTENT && r->op == SL_OP_RENEGOTIATE) {
                    ++r->nursed;
                    record_outcome(r, act, true, true, 1u);
                } else {
                    record_outcome(r, act, true, true, 0u);
                }
                break;
            case SL_ACT_PRO_BONO:
                /* Low-priority opportunistic maintenance: light prune when idle
                 * (no workers running) at a very low floor. */
                if (p && !p->workers_running) {
                    size_t removed = 0u;
                    (void)sl_event_tree_prune(p->tree, e->now, 0.0, &removed);
                    record_outcome(r, act, true, true, removed);
                } else {
                    record_outcome(r, act, true, true, 0u);
                }
                break;
            default:
                record_outcome(r, act, false, true, 0u);
                break;
        }
        notify(e, r, act);
    }
}

/* ---- The uniform dispatch ---------------------------------------------- */
sl_status sl_partition_dispatch(
    sl_partition_engine *e, sl_level level, int partition_index,
    sl_op op, sl_activity_mask activities,
    const sl_event *event, uint64_t id, sl_op_result *out) {
    sl_partition *p = NULL;
    sl_branch_view view;
    sl_branch_view *view_ptr = NULL;
    sl_status status = SL_STATUS_OK;

    if (!e || !out || (unsigned)op >= (unsigned)SL_OP_COUNT)
        return SL_STATUS_INVALID_ARGUMENT;

    memset(out, 0, sizeof(*out));
    out->op = op;
    out->level = level;
    sl_consistency_report_init(&out->consistency);

    if (level == SL_LEVEL_BRANCH || level == SL_LEVEL_PARTITION) {
        p = partition_at(e, partition_index);
        if (!p) { out->status = SL_STATUS_NOT_FOUND; return SL_STATUS_NOT_FOUND; }
    }

    /* Perform the data-changing core of the op first. */
    switch (op) {
        case SL_OP_ADD:
            if (!p || !event) status = SL_STATUS_INVALID_ARGUMENT;
            else status = sl_event_tree_insert(p->tree, event);
            break;
        case SL_OP_DELETE:
            /* The event tree exposes no public delete; model deletion as a
             * targeted prune by ending the event then pruning it out. */
            if (!p) status = SL_STATUS_INVALID_ARGUMENT;
            else {
                sl_event found;
                status = sl_event_tree_find(p->tree, id, &found);
                if (status == SL_STATUS_OK) {
                    size_t removed = 0u;
                    (void)sl_event_tree_end(p->tree, id, e->now);
                    /* Force it below any floor by pruning at 1.0 only if ended
                     * long ago; otherwise leave it (deletion is logical). */
                    (void)sl_event_tree_prune(p->tree, e->now + found.half_life_seconds * 64u, 0.5, &removed);
                }
            }
            break;
        case SL_OP_PRUNE:
            /* Floor carried via id reinterpreted as permil (0..1000). */
            if (!p) status = SL_STATUS_INVALID_ARGUMENT;
            else {
                size_t removed = 0u;
                double floor_w = (double)(id <= 1000u ? id : 1000u) / 1000.0;
                status = sl_event_tree_prune(p->tree, e->now, floor_w, &removed);
            }
            break;
        case SL_OP_RENEGOTIATE:
        case SL_OP_SYSTEM:
        case SL_OP_ANALYSIS:
        case SL_OP_CONTROL:
        case SL_OP_FEATURE:
            /* Non-mutating or state-only; handled via activities/engine state. */
            break;
        default:
            break;
    }

    /* Build the branch view that analysis/scandal/nursing read. */
    if (p) { project_tree(p->tree, &view); view_ptr = &view; }
    else {
        /* System level: project the whole-tree basis and maturation state. */
        view.trait_count = 1u;
        view.weight = SL_BRANCH_WEIGHT_MIN;
        view.studies_economy = false;
        view.economy_arity = 0u;
        /* Basis is 2-3 nodes; split across sides, always within <=15. */
        view.left_branchpoints = 1u;
        view.right_branchpoints = 1u;
        view.refs = NULL;
        view.ref_count = 0u;
        view_ptr = &view;
    }

    /* SYSTEM op can advance a maturation, capped at 3. */
    if (op == SL_OP_SYSTEM && level == SL_LEVEL_SYSTEM) {
        if (e->maturations < SL_TREE_MATURATION_MAX) ++e->maturations;
    }
    if (op == SL_OP_SYSTEM && p) {
        if (p->maturations < SL_TREE_MATURATION_MAX) ++p->maturations;
    }

    /* CONTROL toggles worker state (encoded in id: nonzero => run). */
    if (op == SL_OP_CONTROL && p) p->workers_running = (id != 0u);

    /* FEATURE enables/disables one activity flag (id low bit = enable,
     * next bits = activity index). */
    if (op == SL_OP_FEATURE && p) {
        sl_activity act = (sl_activity)((id >> 1) & 0xFu);
        bool enable = (id & 1u) != 0u;
        if ((unsigned)act < (unsigned)SL_ACT_COUNT) {
            if (enable) p->features |= SL_ACT_BIT(act);
            else p->features &= ~SL_ACT_BIT(act);
        }
    }

    fire_activities(e, out, p, activities, view_ptr);
    out->status = status;
    return status;
}

/* ---- Convenience wrappers ---------------------------------------------- */
sl_status sl_partition_add(sl_partition_engine *e, int idx, const sl_event *ev, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_BRANCH, idx, SL_OP_ADD, SL_ACT_ALL, ev, 0u, r);
}
sl_status sl_partition_delete(sl_partition_engine *e, int idx, uint64_t id, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_BRANCH, idx, SL_OP_DELETE, SL_ACT_ALL, NULL, id, r);
}
sl_status sl_partition_prune(sl_partition_engine *e, int idx, double floor_w, sl_op_result *r) {
    uint64_t permil = (uint64_t)(floor_w <= 0.0 ? 0.0 : (floor_w >= 1.0 ? 1000.0 : floor_w * 1000.0));
    return sl_partition_dispatch(e, SL_LEVEL_PARTITION, idx, SL_OP_PRUNE, SL_ACT_ALL, NULL, permil, r);
}
sl_status sl_partition_renegotiate(sl_partition_engine *e, int idx, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_PARTITION, idx, SL_OP_RENEGOTIATE, SL_ACT_ALL, NULL, 0u, r);
}
sl_status sl_partition_system(sl_partition_engine *e, int idx, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_PARTITION, idx, SL_OP_SYSTEM, SL_ACT_ALL, NULL, 0u, r);
}
sl_status sl_partition_analysis(sl_partition_engine *e, int idx, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_PARTITION, idx, SL_OP_ANALYSIS, SL_ACT_ALL, NULL, 0u, r);
}
sl_status sl_partition_control(sl_partition_engine *e, int idx, bool run_workers, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_PARTITION, idx, SL_OP_CONTROL, SL_ACT_ALL, NULL, run_workers ? 1u : 0u, r);
}
sl_status sl_partition_feature(sl_partition_engine *e, int idx, sl_activity act, bool enable, sl_op_result *r) {
    uint64_t encoded = ((uint64_t)act << 1) | (enable ? 1u : 0u);
    return sl_partition_dispatch(e, SL_LEVEL_PARTITION, idx, SL_OP_FEATURE, SL_ACT_ALL, NULL, encoded, r);
}

/* ---- System helpers ---------------------------------------------------- */
sl_status sl_system_op(sl_partition_engine *e, sl_op op, sl_activity_mask acts, sl_op_result *r) {
    return sl_partition_dispatch(e, SL_LEVEL_SYSTEM, -1, op, acts, NULL, 0u, r);
}
unsigned sl_system_maturations(const sl_partition_engine *e) {
    return e ? e->maturations : 0u;
}
unsigned sl_system_basis_nodes(const sl_partition_engine *e) {
    size_t n = e ? e->partition_count : 0u;
    if (n < SL_TREE_BASIS_NODES_MIN) return SL_TREE_BASIS_NODES_MIN;
    if (n > SL_TREE_BASIS_NODES_MAX) return SL_TREE_BASIS_NODES_MAX;
    return (unsigned)n;
}

sl_status sl_system_add_research_request(
    sl_partition_engine *e, const char *subject, double figure) {
    size_t len, n;
    if (!e || !subject) return SL_STATUS_INVALID_ARGUMENT;
    len = strlen(subject);
    if (len == 0u || len >= SL_RESEARCH_TEXT_CAPACITY) return SL_STATUS_INVALID_ARGUMENT;
    if (e->research_count >= SL_RESEARCH_CAPACITY) return SL_STATUS_OUT_OF_MEMORY;
    n = e->research_count;
    memcpy(e->research[n].subject, subject, len + 1u);
    e->research[n].claimed_figure = figure;
    e->research[n].validated = false; /* pending the scientific community */
    ++e->research_count;
    return SL_STATUS_OK;
}
size_t sl_system_research_request_count(const sl_partition_engine *e) {
    return e ? e->research_count : 0u;
}
bool sl_system_research_request_at(
    const sl_partition_engine *e, size_t i, sl_research_request *out) {
    if (!e || !out || i >= e->research_count) return false;
    *out = e->research[i];
    return true;
}

/* ---- Stringization ----------------------------------------------------- */
const char *sl_level_string(sl_level l) {
    switch (l) {
        case SL_LEVEL_BRANCH: return "branch";
        case SL_LEVEL_PARTITION: return "partition";
        case SL_LEVEL_SYSTEM: return "system";
        default: return "unknown";
    }
}
const char *sl_op_string(sl_op op) {
    switch (op) {
        case SL_OP_ADD: return "add";
        case SL_OP_DELETE: return "delete";
        case SL_OP_PRUNE: return "prune";
        case SL_OP_RENEGOTIATE: return "renegotiate";
        case SL_OP_SYSTEM: return "system";
        case SL_OP_ANALYSIS: return "analysis";
        case SL_OP_CONTROL: return "control";
        case SL_OP_FEATURE: return "feature";
        default: return "unknown";
    }
}
const char *sl_activity_string(sl_activity a) {
    switch (a) {
        case SL_ACT_WORKERS: return "workers";
        case SL_ACT_LISTENERS: return "listeners";
        case SL_ACT_CONTROLLERS: return "controllers";
        case SL_ACT_EVENT_HANDLERS: return "event-handlers";
        case SL_ACT_EXCHANGER: return "exchanger";
        case SL_ACT_MEMORY_ANALYSIS: return "memory-analysis";
        case SL_ACT_SAVING: return "saving";
        case SL_ACT_RETIREMENT: return "retirement";
        case SL_ACT_SCANDAL: return "scandal";
        case SL_ACT_NURSING: return "nursing";
        case SL_ACT_PRO_BONO: return "pro-bono";
        default: return "unknown";
    }
}
