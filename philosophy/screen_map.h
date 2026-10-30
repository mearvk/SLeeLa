#ifndef SLEELA_PHILOSOPHY_SCREEN_MAP_H
#define SLEELA_PHILOSOPHY_SCREEN_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SM_MAX_OBSERVATIONS 64u
#define SM_KEY_CAPACITY 48u
#define SM_UNIT_CAPACITY 24u

typedef enum sm_status {
    SM_STATUS_OK = 0,
    SM_STATUS_INVALID_ARGUMENT,
    SM_STATUS_INVALID_NUMBER,
    SM_STATUS_INVALID_RANGE,
    SM_STATUS_INCOMPATIBLE_OBSERVATIONS,
    SM_STATUS_CAPACITY_EXCEEDED,
    SM_STATUS_NOT_FOUND
} sm_status;

typedef enum sm_domain {
    SM_DOMAIN_CONTEXT = 0,
    SM_DOMAIN_LEARNING,
    SM_DOMAIN_SKILL,
    SM_DOMAIN_WELLBEING,
    SM_DOMAIN_BIOLOGY,
    SM_DOMAIN_HEALTH,
    SM_DOMAIN_MOTOR_TASK,
    SM_DOMAIN_RELATIONSHIP,
    SM_DOMAIN_EVIDENCE,
    SM_DOMAIN_COUNT
} sm_domain;

#define SM_DOMAIN_BIT(domain) (UINT32_C(1) << (unsigned)(domain))
#define SM_DOMAIN_MASK_ALL ((UINT32_C(1) << SM_DOMAIN_COUNT) - UINT32_C(1))

typedef enum sm_source {
    SM_SOURCE_SELF_REPORT = 0,
    SM_SOURCE_DIRECT_OBSERVATION,
    SM_SOURCE_STANDARDIZED_INSTRUMENT,
    SM_SOURCE_QUALIFIED_PROFESSIONAL,
    SM_SOURCE_PEER_REVIEWED_RESEARCH,
    SM_SOURCE_ANIMAL_MODEL,
    SM_SOURCE_COUNT
} sm_source;

typedef enum sm_rounding {
    SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO = 0,
    SM_ROUND_FLOOR,
    SM_ROUND_CEIL,
    SM_ROUND_TRUNCATE
} sm_rounding;

typedef struct sm_observation {
    char key[SM_KEY_CAPACITY];
    char unit[SM_UNIT_CAPACITY];
    double value;
    uint64_t observed_at_epoch_seconds;
    double confidence;
    uint32_t relationship_group;
    sm_domain domain;
    sm_source source;
    bool share_with_conversation;
} sm_observation;

typedef struct sm_profile {
    sm_observation observations[SM_MAX_OBSERVATIONS];
    size_t count;
} sm_profile;

typedef struct sm_screen_map {
    sm_observation observations[SM_MAX_OBSERVATIONS];
    size_t count;
    size_t omitted_private;
    size_t omitted_by_domain;
    bool unknowns_left_unknown;
} sm_screen_map;

typedef struct sm_reduction {
    int value;
    bool valid;
    bool was_clamped;
} sm_reduction;

sm_status sm_observation_init(
    sm_observation *out, const char *key, const char *unit,
    double value, uint64_t observed_at_epoch_seconds,
    sm_domain domain, sm_source source, double confidence,
    uint32_t relationship_group, bool share_with_conversation);

void sm_profile_init(sm_profile *profile);
sm_status sm_profile_add(sm_profile *profile, const sm_observation *observation);

sm_status sm_observation_delta(
    const sm_observation *earlier,
    const sm_observation *later,
    double *delta_out);

sm_status sm_screen_map_build(
    const sm_profile *profile,
    uint32_t allowed_domain_mask,
    sm_screen_map *map_out);

sm_status sm_reduce_integer(
    double raw_value, int minimum, int maximum,
    sm_rounding rounding, sm_reduction *out);

sm_status sm_reduce_ratio(
    double numerator, double denominator, double scale,
    int minimum, int maximum, sm_rounding rounding,
    sm_reduction *out);

const char *sm_status_string(sm_status status);
const char *sm_domain_string(sm_domain domain);
const char *sm_source_string(sm_source source);

#ifdef __cplusplus
}
#endif

#endif
