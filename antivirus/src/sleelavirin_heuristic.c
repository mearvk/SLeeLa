#include "sleelavirin_heuristic.h"
#include <string.h>
int sleelavirin_trap2_evaluate(
    const sleelavirin_trap2_profile_t *profile,
    const sleelavirin_indicator_t *indicators,
    size_t indicator_count,
    sleelavirin_heuristic_result_t *out)
{
    size_t i;
    uint32_t score = 0;
    uint8_t evidence = 0;
    if (!profile || !indicators || !out) return -1;
    memset(out, 0, sizeof(*out));
    for (i = 0; i < indicator_count; ++i) {
        if (!indicators[i].observed) continue;
        score += indicators[i].weight;
        if (evidence < SLEELAVIRIN_HEURISTIC_MAX_EVIDENCE) ++evidence;
    }
    out->score = (uint16_t)(score > 65535u ? 65535u : score);
    out->evidence_count = evidence;
    if (profile->trap_threshold != 0 && out->score >= profile->trap_threshold)
        out->trap_level = 2;
    else if (profile->threshold != 0 && out->score >= profile->threshold)
        out->trap_level = 1;
    out->triggered = (out->trap_level != 0);
    out->action_permitted =
        (out->triggered &&
         profile->operator_acknowledged &&
         profile->allow_action &&
         profile->operator_name &&
         profile->license_id &&
         profile->basis_id) ? 1u : 0u;
    return 0;
}
