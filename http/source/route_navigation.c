#include "route_navigation.h"

#include <string.h>

static int route_hop_equal(const http3_route_hop_t *a,
                           const http3_route_hop_t *b)
{
    return a->node_id == b->node_id &&
           a->geodata.country_id == b->geodata.country_id &&
           a->geodata.network_id == b->geodata.network_id &&
           a->geodata.region_id == b->geodata.region_id &&
           a->geodata.asn == b->geodata.asn &&
           a->geodata.risk_level == b->geodata.risk_level &&
           a->geodata.geodata_epoch == b->geodata.geodata_epoch;
}

int http3_route_plan_validate(const http3_route_plan_t *plan)
{
    size_t i;

    if (plan == NULL || plan->route_id == 0 || plan->generation == 0 ||
        plan->hop_count > HTTP3_ROUTE_NAV_MAX_HOPS) {
        return -1;
    }

    for (i = 0; i < plan->hop_count; ++i) {
        if (plan->hops[i].node_id == 0 ||
            plan->hops[i].geodata.country_id == 0) {
            return -1;
        }
    }

    return 0;
}

int http3_route_plan_compare(const http3_route_plan_t *old_plan,
                             const http3_route_plan_t *new_plan,
                             http3_route_nav_decision_t *decision)
{
    size_t i;
    size_t common;
    uint32_t changed_hops = 0;
    uint32_t changed_countries = 0;
    uint32_t changed_geodata = 0;

    if (decision == NULL || http3_route_plan_validate(old_plan) != 0 ||
        http3_route_plan_validate(new_plan) != 0) {
        return -1;
    }

    memset(decision, 0, sizeof(*decision));
    decision->next_generation = old_plan->generation;

    common = old_plan->hop_count < new_plan->hop_count
                 ? old_plan->hop_count
                 : new_plan->hop_count;

    for (i = 0; i < common; ++i) {
        const http3_route_hop_t *old_hop = &old_plan->hops[i];
        const http3_route_hop_t *new_hop = &new_plan->hops[i];

        if (old_hop->node_id != new_hop->node_id) {
            ++changed_hops;
        }
        if (old_hop->geodata.country_id != new_hop->geodata.country_id) {
            ++changed_countries;
        }
        if (old_hop->geodata.geodata_epoch != new_hop->geodata.geodata_epoch ||
            old_hop->geodata.network_id != new_hop->geodata.network_id ||
            old_hop->geodata.region_id != new_hop->geodata.region_id ||
            old_hop->geodata.asn != new_hop->geodata.asn ||
            old_hop->geodata.risk_level != new_hop->geodata.risk_level) {
            ++changed_geodata;
        }

        (void)route_hop_equal;
    }

    if (old_plan->hop_count != new_plan->hop_count) {
        changed_hops += (uint32_t)(old_plan->hop_count > new_plan->hop_count
                                       ? old_plan->hop_count - new_plan->hop_count
                                       : new_plan->hop_count - old_plan->hop_count);
    }

    decision->changed_hops = changed_hops;
    decision->changed_countries = changed_countries;
    decision->changed_geodata = changed_geodata;
    decision->next_generation = new_plan->generation;

    if (changed_hops == 0 && changed_countries == 0 && changed_geodata == 0 &&
        old_plan->geodata_epoch == new_plan->geodata_epoch) {
        decision->action = HTTP3_ROUTE_NAV_KEEP;
        return 0;
    }

    if (changed_countries > 0 || changed_hops >= HTTP3_ROUTE_NAV_REPLAN_THRESHOLD) {
        decision->action = HTTP3_ROUTE_NAV_REPLAN;
        decision->crypto_rebuild_required = changed_countries > 0;
        return 0;
    }

    decision->action = HTTP3_ROUTE_NAV_REVALIDATE;
    decision->crypto_rebuild_required = changed_geodata > 0;
    return 0;
}

int http3_route_plan_rebind(http3_route_plan_t *plan,
                            uint64_t new_generation,
                            uint64_t new_geodata_epoch)
{
    if (http3_route_plan_validate(plan) != 0 || new_generation == 0) {
        return -1;
    }

    plan->generation = new_generation;
    plan->geodata_epoch = new_geodata_epoch;
    plan->stability_score = HTTP3_ROUTE_NAV_STABILITY_THRESHOLD;
    return 0;
}
