#include "consistency_monitor.h"
#include "event_tree.h"
#include "signal_toss.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Demonstrates the threaded signal toss sampling an event tree for branch
 * consistency at ~44 Hz, with consistency updates throttled into the
 * 800-1200/hour band. Each sampled branch is projected from one event:
 *   - trait_count  <- a bounded trait budget (<= 45),
 *   - weight       <- derived from the event id, clamped to [1, 9],
 *   - economy      <- weight-9 branches study exactly two things,
 *   - maturity     <- reference-weighted from the event's relevance decay.
 */
typedef struct sampler_ctx {
    sl_event_tree *tree;
    uint64_t now;
    uint64_t ids[16];
    size_t id_count;
    size_t cursor;
} sampler_ctx;

static bool collect_ids(const sl_event *e, double w, void *ctx) {
    sampler_ctx *c = (sampler_ctx *)ctx;
    (void)w;
    if (c->id_count < sizeof(c->ids) / sizeof(c->ids[0]))
        c->ids[c->id_count++] = e->id;
    return true;
}

static bool branch_sampler(sl_branch_view *out, void *ctx) {
    sampler_ctx *c = (sampler_ctx *)ctx;
    static sl_reference refs[1];
    sl_event e;
    uint64_t id;
    double relevance;

    if (c->id_count == 0u) return false;
    id = c->ids[c->cursor % c->id_count];
    c->cursor++;
    if (sl_event_tree_find(c->tree, id, &e) != SL_STATUS_OK) return true;

    relevance = sl_event_relevance(&e, c->now);

    out->trait_count = (unsigned)(id % 44u) + 1u;        /* 1..44, within cap */
    out->weight = sl_branch_clamp_weight((long)(id % 9u) + 1u);
    out->studies_economy = (e.type == SL_EVENT_ECONOMIC_METRIC);
    /* A weight-9 economic branch studies exactly two things. */
    out->economy_arity = (out->weight == 9u && out->studies_economy) ? 2u : 0u;
    out->left_branchpoints = (unsigned)(id % 13u);       /* <= 12 */
    out->right_branchpoints = (unsigned)(id % 14u);      /* <= 13 */

    /* Maturity by reference weighting: a mature record is one whose relevance
     * is still high; it lands past the 0.80 / 0.97 gates when fresh and drops
     * below the decisive 0.51 majority as it decays. */
    refs[0].weight = 1.0;
    refs[0].truth = relevance;
    out->refs = refs;
    out->ref_count = 1u;
    return true;
}

static void seed_tree(sl_event_tree *tree, uint64_t now) {
    sl_event e;
    unsigned i;
    (void)sl_event_init_integer(&e, 1u, SL_EVENT_ECONOMIC_METRIC,
        SL_SOURCE_MEDIA_OR_PUBLICATION, "industrial_output", "index", 104,
        now - 86400u, 604800u, 0u, true, "named report", "illustrative");
    (void)sl_event_tree_insert(tree, &e);
    for (i = 100u; i < 160u; ++i) {
        if (sl_event_init_integer(&e, i, SL_EVENT_SOCIAL, SL_SOURCE_DIRECT_OBSERVATION,
            "event_marker", "count", (int64_t)i, now - 30u, 86400u, 0u, true,
            "explicitly submitted", "") == SL_STATUS_OK)
            (void)sl_event_tree_insert(tree, &e);
    }
}

int main(void) {
    const uint64_t now = UINT64_C(1793400000);
    sl_event_tree *tree = sl_event_tree_create();
    sampler_ctx ctx;
    sl_signal_toss *toss;
    sl_consistency_report snap;
    if (!tree) return 1;

    seed_tree(tree, now);

    ctx.tree = tree;
    ctx.now = now;
    ctx.id_count = 0u;
    ctx.cursor = 0u;
    (void)sl_event_tree_visit(tree, now, 0.0, collect_ids, &ctx);

    toss = sl_signal_toss_create(branch_sampler, &ctx, 1000u);
    if (!toss) { sl_event_tree_destroy(tree); return 1; }

    printf("signal toss: ~%u Hz sampling, commit interval %u ms "
           "(target 800-1200 updates/hour)\n",
           SL_TOSS_SAMPLE_HZ, sl_signal_toss_commit_interval_ms(toss));

    if (sl_signal_toss_start(toss) != 0) {
        sl_signal_toss_destroy(toss);
        sl_event_tree_destroy(tree);
        return 1;
    }
    sl_sleep_ms(500u); /* let the worker toss signals for half a second */
    sl_signal_toss_stop(toss);

    snap = sl_signal_toss_snapshot(toss);
    printf("branches considered : %" PRIu64 "\n", snap.branch_considerations);
    printf("mature lengths      : %" PRIu64 "\n", snap.mature_length_considerations);
    printf("accrued updates     : %" PRIu64 " (4/branch + 5/mature-length)\n", snap.updates);
    printf("trait warnings (>=44): %" PRIu64 "\n", snap.warnings);
    printf("inconsistencies     : %" PRIu64 " (%s)\n", snap.inconsistencies,
           sl_consistency_code_string(snap.last_code));
    printf("committed updates   : %" PRIu64 " (throttled consistency confirmations)\n",
           sl_signal_toss_commits(toss));

    sl_signal_toss_destroy(toss);
    sl_event_tree_destroy(tree);
    return 0;
}
