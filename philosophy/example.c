#include "screen_map.h"

#include <stdio.h>

static int add(
    sm_profile *profile, const char *key, const char *unit,
    double value, uint64_t timestamp, sm_domain domain,
    sm_source source, bool share) {

    sm_observation observation;
    sm_status status = sm_observation_init(
        &observation, key, unit, value, timestamp, domain, source,
        0.95, 0u, share);
    if (status != SM_STATUS_OK) {
        fprintf(stderr, "%s: %s\n", key, sm_status_string(status));
        return 0;
    }
    status = sm_profile_add(profile, &observation);
    if (status != SM_STATUS_OK) {
        fprintf(stderr, "%s: %s\n", key, sm_status_string(status));
        return 0;
    }
    return 1;
}

int main(void) {
    const uint64_t day_one = UINT64_C(1780000000);
    sm_profile profile;
    sm_screen_map screen;
    sm_reduction reduction;
    double delta = 0.0;
    size_t index;

    sm_profile_init(&profile);

    if (!add(&profile, "chronological_age", "years", 34.0, day_one,
             SM_DOMAIN_CONTEXT, SM_SOURCE_SELF_REPORT, true) ||
        !add(&profile, "task_accuracy", "proportion", 0.72, day_one,
             SM_DOMAIN_MOTOR_TASK, SM_SOURCE_DIRECT_OBSERVATION, true) ||
        !add(&profile, "task_accuracy", "proportion", 0.84,
             day_one + UINT64_C(2592000), SM_DOMAIN_MOTOR_TASK,
             SM_SOURCE_DIRECT_OBSERVATION, true) ||
        !add(&profile, "head_lice_observation_count", "count", 0.0,
             day_one, SM_DOMAIN_HEALTH, SM_SOURCE_SELF_REPORT, false) ||
        !add(&profile, "mouse_model_memory_result", "study-specific",
             0.61, day_one, SM_DOMAIN_EVIDENCE, SM_SOURCE_ANIMAL_MODEL, true)) {
        return 1;
    }

    if (sm_observation_delta(
            &profile.observations[1], &profile.observations[2], &delta)
        != SM_STATUS_OK ||
        sm_reduce_integer(delta * 100.0, -100, 100,
                          SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO,
                          &reduction) != SM_STATUS_OK) {
        return 1;
    }
    if (sm_screen_map_build(&profile, 0u, &screen) != SM_STATUS_OK) {
        return 1;
    }

    printf("Comparable task-accuracy change: %+d percentage points\n",
           reduction.value);
    printf("Conversation-visible records: %zu; private omitted: %zu\n",
           screen.count, screen.omitted_private);
    for (index = 0u; index < screen.count; ++index) {
        const sm_observation *item = &screen.observations[index];
        printf("  %-30s %g %s [%s; %s]\n",
               item->key, item->value, item->unit,
               sm_domain_string(item->domain),
               sm_source_string(item->source));
    }
    puts("The animal-model result remains research evidence, not a human score.");
    return 0;
}
