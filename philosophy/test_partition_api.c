#include "partition_api.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static uint64_t g_observed = 0u;
static void observe(const sl_op_result *r, const sl_activity_outcome *o, void *ctx) {
    (void)r; (void)o; (void)ctx;
    ++g_observed;
}

static sl_event make_event(uint64_t id, uint64_t now) {
    sl_event e;
    memset(&e, 0, sizeof(e));
    (void)sl_event_init_integer(&e, id, SL_EVENT_SOCIAL, SL_SOURCE_DIRECT_OBSERVATION,
        "marker", "count", (int64_t)id, now - 10u, 86400u, 0u, true,
        "explicitly submitted", "");
    return e;
}

static void test_levels_and_ops(void) {
    const uint64_t now = 1793400000u;
    sl_partition_engine *e = sl_partition_engine_create(now);
    sl_op_result r;
    int pa, pb;
    sl_event ev;
    unsigned i;
    assert(e);

    /* Two partitions => basis 2 nodes. */
    pa = sl_partition_create(e, "social");
    pb = sl_partition_create(e, "economic");
    assert(pa == 0 && pb == 1);
    assert(sl_partition_create(e, "social") < 0); /* no duplicates */
    assert(sl_partition_count(e) == 2u);
    assert(sl_system_basis_nodes(e) == 2u);

    /* add at branch level fires all 11 activities. */
    ev = make_event(10u, now);
    assert(sl_partition_add(e, pa, &ev, &r) == SL_STATUS_OK);
    assert(r.level == SL_LEVEL_BRANCH && r.op == SL_OP_ADD);
    for (i = 0u; i < (unsigned)SL_ACT_COUNT; ++i)
        assert(r.outcomes[i].fired);
    assert(r.fired == SL_ACT_ALL);

    for (i = 20u; i < 60u; ++i) {
        ev = make_event(i, now);
        assert(sl_partition_add(e, pa, &ev, &r) == SL_STATUS_OK);
    }

    /* analysis runs the consistency monitor over the partition. */
    assert(sl_partition_analysis(e, pa, &r) == SL_STATUS_OK);
    assert(r.consistency.branch_considerations == 1u);

    /* prune at partition level. */
    assert(sl_partition_prune(e, pa, 0.9, &r) == SL_STATUS_OK);
    assert(r.op == SL_OP_PRUNE && r.level == SL_LEVEL_PARTITION);

    /* delete a branch. */
    assert(sl_partition_delete(e, pa, 25u, &r) == SL_STATUS_OK);

    /* renegotiate (nursing seam). */
    assert(sl_partition_renegotiate(e, pa, &r) == SL_STATUS_OK);

    /* control: start/stop workers. */
    assert(sl_partition_control(e, pa, true, &r) == SL_STATUS_OK);
    assert(sl_partition_control(e, pa, false, &r) == SL_STATUS_OK);

    /* feature: disable then re-enable the scandal activity. */
    assert(sl_partition_feature(e, pa, SL_ACT_SCANDAL, false, &r) == SL_STATUS_OK);
    /* With scandal disabled it must not be in the fired set next op. */
    assert(sl_partition_analysis(e, pa, &r) == SL_STATUS_OK);
    assert((r.fired & SL_ACT_BIT(SL_ACT_SCANDAL)) == 0u);
    assert(sl_partition_feature(e, pa, SL_ACT_SCANDAL, true, &r) == SL_STATUS_OK);
    assert(sl_partition_analysis(e, pa, &r) == SL_STATUS_OK);
    assert((r.fired & SL_ACT_BIT(SL_ACT_SCANDAL)) != 0u);

    sl_partition_engine_destroy(e);
}

static void test_maturation_cap(void) {
    sl_partition_engine *e = sl_partition_engine_create(1000u);
    sl_op_result r;
    unsigned i;
    assert(e);
    assert(sl_system_maturations(e) == 0u);
    /* System op matures the tree, capped at 3 even if fired more. */
    for (i = 0u; i < 10u; ++i)
        assert(sl_system_op(e, SL_OP_SYSTEM, SL_ACT_ALL, &r) == SL_STATUS_OK);
    assert(sl_system_maturations(e) == SL_TREE_MATURATION_MAX); /* == 3 */
    assert(SL_TREE_MATURATION_USUAL == 1u);
    sl_partition_engine_destroy(e);
}

static void test_basis_nodes(void) {
    sl_partition_engine *e = sl_partition_engine_create(1000u);
    char name[16];
    unsigned i;
    assert(e);
    /* 0 partitions still reports the 2-node minimum basis. */
    assert(sl_system_basis_nodes(e) == SL_TREE_BASIS_NODES_MIN);
    for (i = 0u; i < 6u; ++i) {
        sprintf(name, "p%u", i);
        (void)sl_partition_create(e, name);
    }
    /* Many partitions still clamp to the 3-node maximum basis. */
    assert(sl_system_basis_nodes(e) == SL_TREE_BASIS_NODES_MAX); /* == 3 */
    sl_partition_engine_destroy(e);
}

static void test_research_requests(void) {
    sl_partition_engine *e = sl_partition_engine_create(1000u);
    sl_research_request got;
    assert(e);
    /* "IQ" figures are recorded as open requests to scientists, never scores. */
    assert(sl_system_add_research_request(e,
        "Scientists: assess the base merit of this event-tree series", 200.0) == SL_STATUS_OK);
    assert(sl_system_add_research_request(e,
        "Scientists: evaluate autonomy claims for this series", 230.0) == SL_STATUS_OK);
    assert(sl_system_research_request_count(e) == 2u);
    assert(sl_system_research_request_at(e, 0u, &got));
    assert(got.claimed_figure == 200.0);
    assert(!got.validated); /* pending science */
    assert(sl_system_research_request_at(e, 1u, &got));
    assert(got.claimed_figure == 230.0 && !got.validated);
    sl_partition_engine_destroy(e);
}

static void test_observer(void) {
    sl_partition_engine *e = sl_partition_engine_create(1000u);
    sl_op_result r;
    sl_event ev;
    int p;
    assert(e);
    sl_partition_engine_set_observer(e, observe, NULL);
    p = sl_partition_create(e, "watched");
    g_observed = 0u;
    ev = make_event(1u, 1000u);
    assert(sl_partition_add(e, p, &ev, &r) == SL_STATUS_OK);
    /* All 11 activities fired => observer called 11 times. */
    assert(g_observed == (uint64_t)SL_ACT_COUNT);
    sl_partition_engine_destroy(e);
}

int main(void) {
    printf("test_levels_and_ops...\n");  test_levels_and_ops();
    printf("test_maturation_cap...\n");  test_maturation_cap();
    printf("test_basis_nodes...\n");     test_basis_nodes();
    printf("test_research_requests...\n"); test_research_requests();
    printf("test_observer...\n");        test_observer();
    printf("all partition API tests passed\n");
    return 0;
}
