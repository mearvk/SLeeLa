#ifndef HTTP3_ROUTE_NAVIGATION_H
#define HTTP3_ROUTE_NAVIGATION_H

#include <stddef.h>
#include <stdint.h>

#include "crypto_route_map.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_ROUTE_NAV_MAX_HOPS 64u
#define HTTP3_ROUTE_NAV_MAX_CANDIDATES 8u
#define HTTP3_ROUTE_NAV_STABILITY_THRESHOLD 3u
#define HTTP3_ROUTE_NAV_REPLAN_THRESHOLD 2u

typedef struct {
    uint32_t country_id;
    uint32_t network_id;
    uint32_t region_id;
    uint32_t asn;
    uint32_t risk_level;
    uint64_t geodata_epoch;
} http3_route_geodata_t;

typedef struct {
    uint32_t node_id;
    http3_route_geodata_t geodata;
} http3_route_hop_t;

typedef struct {
    uint64_t route_id;
    uint64_t generation;
    uint64_t geodata_epoch;
    size_t hop_count;
    http3_route_hop_t hops[HTTP3_ROUTE_NAV_MAX_HOPS];
    uint32_t stability_score;
} http3_route_plan_t;

typedef enum {
    HTTP3_ROUTE_NAV_KEEP = 0,
    HTTP3_ROUTE_NAV_REVALIDATE = 1,
    HTTP3_ROUTE_NAV_REPLAN = 2
} http3_route_nav_action_t;

typedef struct {
    http3_route_nav_action_t action;
    uint64_t next_generation;
    uint32_t changed_hops;
    uint32_t changed_countries;
    uint32_t changed_geodata;
    int crypto_rebuild_required;
} http3_route_nav_decision_t;

int http3_route_plan_validate(const http3_route_plan_t *plan);
int http3_route_plan_compare(const http3_route_plan_t *old_plan,
                             const http3_route_plan_t *new_plan,
                             http3_route_nav_decision_t *decision);
int http3_route_plan_rebind(http3_route_plan_t *plan,
                            uint64_t new_generation,
                            uint64_t new_geodata_epoch);

#ifdef __cplusplus
}
#endif

#endif
