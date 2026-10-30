#include "partition_api.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/*
 * Walks the uniform three-level API: the same eight operations and eleven
 * fired activities applied at branch, partition and system level, with the
 * whole-tree basis (2-3 nodes) and the 3-maturation cap.
 */
static void print_result(const sl_op_result *r) {
    unsigned a;
    printf("  %-9s @ %-9s status=%s fired={", sl_op_string(r->op),
           sl_level_string(r->level), sl_status_string(r->status));
    for (a = 0u; a < (unsigned)SL_ACT_COUNT; ++a)
        if (r->fired & SL_ACT_BIT((sl_activity)a))
            printf("%s ", sl_activity_string((sl_activity)a));
    printf("} scandals=%" PRIu64 " nursed=%" PRIu64 "\n", r->scandals, r->nursed);
}

static sl_event make_event(uint64_t id, uint64_t now) {
    sl_event e;
    memset(&e, 0, sizeof(e));
    (void)sl_event_init_integer(&e, id, SL_EVENT_SOCIAL, SL_SOURCE_DIRECT_OBSERVATION,
        "marker", "count", (int64_t)id, now - 10u, 86400u, 0u, true,
        "explicitly submitted", "");
    return e;
}

int main(void) {
    const uint64_t now = UINT64_C(1793400000);
    sl_partition_engine *e = sl_partition_engine_create(now);
    sl_op_result r;
    int social, economic;
    sl_event ev;
    unsigned i;
    if (!e) return 1;

    social = sl_partition_create(e, "social");
    economic = sl_partition_create(e, "economic");
    printf("partitions=%zu  system basis nodes=%u (usual 2-3)\n",
           sl_partition_count(e), sl_system_basis_nodes(e));

    puts("\n-- branch level (add) --");
    for (i = 1u; i <= 5u; ++i) {
        ev = make_event(i, now);
        (void)sl_partition_add(e, social, &ev, &r);
    }
    print_result(&r);

    puts("-- partition level (prune / renegotiate / analysis / control / feature) --");
    (void)sl_partition_prune(e, social, 0.5, &r);        print_result(&r);
    (void)sl_partition_renegotiate(e, social, &r);        print_result(&r);
    (void)sl_partition_analysis(e, social, &r);           print_result(&r);
    (void)sl_partition_control(e, social, true, &r);      print_result(&r);
    (void)sl_partition_feature(e, social, SL_ACT_PRO_BONO, true, &r); print_result(&r);
    (void)sl_partition_delete(e, social, 3u, &r);         print_result(&r);

    puts("-- system level (whole-tree maturation, capped at 3) --");
    for (i = 0u; i < 5u; ++i) {
        (void)sl_system_op(e, SL_OP_SYSTEM, SL_ACT_ALL, &r);
        printf("  maturation attempt %u -> maturations=%u (usual %u, max %u)\n",
               i + 1u, sl_system_maturations(e),
               SL_TREE_MATURATION_USUAL, SL_TREE_MATURATION_MAX);
    }

    puts("-- figures recorded as open requests to scientists (not scores) --");
    (void)sl_system_add_research_request(e,
        "Scientists: assess the base merit of this event-tree series", 200.0);
    (void)sl_system_add_research_request(e,
        "Scientists: evaluate the independent-nature autonomy of this series", 230.0);
    for (i = 0u; i < (unsigned)sl_system_research_request_count(e); ++i) {
        sl_research_request req;
        if (sl_system_research_request_at(e, i, &req))
            printf("  request: \"%s\" figure=%.0f validated=%s\n",
                   req.subject, req.claimed_figure, req.validated ? "yes" : "no (pending science)");
    }

    (void)economic;
    sl_partition_engine_destroy(e);
    return 0;
}
