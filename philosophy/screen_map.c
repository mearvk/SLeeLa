#include "screen_map.h"

#include <math.h>
#include <string.h>

static bool sm_valid_domain(sm_domain domain) {
    return (unsigned)domain < (unsigned)SM_DOMAIN_COUNT;
}

static bool sm_valid_source(sm_source source) {
    return (unsigned)source < (unsigned)SM_SOURCE_COUNT;
}

static bool sm_copy_text(char *destination, size_t capacity,
                         const char *source, bool allow_empty) {
    size_t length;

    if (destination == NULL || source == NULL || capacity == 0u) {
        return false;
    }
    length = strlen(source);
    if ((!allow_empty && length == 0u) || length >= capacity) {
        return false;
    }
    memcpy(destination, source, length + 1u);
    return true;
}

static bool sm_observation_is_valid(const sm_observation *observation) {
    if (observation == NULL) {
        return false;
    }
    if (memchr(observation->key, '\0', sizeof(observation->key)) == NULL ||
        observation->key[0] == '\0' ||
        memchr(observation->unit, '\0', sizeof(observation->unit)) == NULL) {
        return false;
    }
    if (!isfinite(observation->value) ||
        !isfinite(observation->confidence) ||
        observation->confidence < 0.0 ||
        observation->confidence > 1.0) {
        return false;
    }
    if (!sm_valid_domain(observation->domain) ||
        !sm_valid_source(observation->source)) {
        return false;
    }
    if (observation->source == SM_SOURCE_ANIMAL_MODEL &&
        observation->domain != SM_DOMAIN_EVIDENCE) {
        return false;
    }
    return true;
}

sm_status sm_observation_init(
    sm_observation *out, const char *key, const char *unit,
    double value, uint64_t observed_at_epoch_seconds,
    sm_domain domain, sm_source source, double confidence,
    uint32_t relationship_group, bool share_with_conversation) {

    sm_observation candidate;

    if (out == NULL || key == NULL || unit == NULL ||
        !sm_valid_domain(domain) || !sm_valid_source(source)) {
        return SM_STATUS_INVALID_ARGUMENT;
    }
    if (!isfinite(value) || !isfinite(confidence) ||
        confidence < 0.0 || confidence > 1.0) {
        return SM_STATUS_INVALID_NUMBER;
    }
    if (source == SM_SOURCE_ANIMAL_MODEL && domain != SM_DOMAIN_EVIDENCE) {
        return SM_STATUS_INVALID_ARGUMENT;
    }

    memset(&candidate, 0, sizeof(candidate));
    if (!sm_copy_text(candidate.key, sizeof(candidate.key), key, false) ||
        !sm_copy_text(candidate.unit, sizeof(candidate.unit), unit, true)) {
        return SM_STATUS_INVALID_ARGUMENT;
    }

    candidate.value = value;
    candidate.observed_at_epoch_seconds = observed_at_epoch_seconds;
    candidate.confidence = confidence;
    candidate.relationship_group = relationship_group;
    candidate.domain = domain;
    candidate.source = source;
    candidate.share_with_conversation = share_with_conversation;
    *out = candidate;
    return SM_STATUS_OK;
}

void sm_profile_init(sm_profile *profile) {
    if (profile != NULL) {
        memset(profile, 0, sizeof(*profile));
    }
}

sm_status sm_profile_add(sm_profile *profile, const sm_observation *observation) {
    size_t index;

    if (profile == NULL || !sm_observation_is_valid(observation)) {
        return SM_STATUS_INVALID_ARGUMENT;
    }
    if (profile->count > SM_MAX_OBSERVATIONS) {
        return SM_STATUS_INVALID_ARGUMENT;
    }
    if (profile->count == SM_MAX_OBSERVATIONS) {
        return SM_STATUS_CAPACITY_EXCEEDED;
    }

    for (index = 0u; index < profile->count; ++index) {
        const sm_observation *existing = &profile->observations[index];
        if (existing->observed_at_epoch_seconds ==
                observation->observed_at_epoch_seconds &&
            strcmp(existing->key, observation->key) == 0) {
            return SM_STATUS_INVALID_ARGUMENT;
        }
    }
    profile->observations[profile->count] = *observation;
    ++profile->count;
    return SM_STATUS_OK;
}

sm_status sm_observation_delta(
    const sm_observation *earlier,
    const sm_observation *later,
    double *delta_out) {

    double delta;

    if (delta_out == NULL ||
        !sm_observation_is_valid(earlier) ||
        !sm_observation_is_valid(later)) {
        return SM_STATUS_INVALID_ARGUMENT;
    }
    if (strcmp(earlier->key, later->key) != 0 ||
        strcmp(earlier->unit, later->unit) != 0 ||
        earlier->domain != later->domain ||
        later->observed_at_epoch_seconds <=
            earlier->observed_at_epoch_seconds) {
        return SM_STATUS_INCOMPATIBLE_OBSERVATIONS;
    }
    delta = later->value - earlier->value;
    if (!isfinite(delta)) {
        return SM_STATUS_INVALID_NUMBER;
    }
    *delta_out = delta;
    return SM_STATUS_OK;
}

sm_status sm_screen_map_build(
    const sm_profile *profile,
    uint32_t allowed_domain_mask,
    sm_screen_map *map_out) {

    size_t index;

    if (profile == NULL || map_out == NULL ||
        profile->count > SM_MAX_OBSERVATIONS) {
        return SM_STATUS_INVALID_ARGUMENT;
    }

    memset(map_out, 0, sizeof(*map_out));
    map_out->unknowns_left_unknown = true;

    for (index = 0u; index < profile->count; ++index) {
        const sm_observation *observation = &profile->observations[index];

        if (!sm_observation_is_valid(observation)) {
            return SM_STATUS_INVALID_ARGUMENT;
        }
        if (!observation->share_with_conversation) {
            ++map_out->omitted_private;
            continue;
        }
        if (allowed_domain_mask != 0u &&
            (allowed_domain_mask & SM_DOMAIN_BIT(observation->domain)) == 0u) {
            ++map_out->omitted_by_domain;
            continue;
        }
        map_out->observations[map_out->count] = *observation;
        ++map_out->count;
    }
    return SM_STATUS_OK;
}

sm_status sm_reduce_integer(
    double raw_value, int minimum, int maximum,
    sm_rounding rounding, sm_reduction *out) {

    double rounded;

    if (out == NULL) {
        return SM_STATUS_INVALID_ARGUMENT;
    }
    out->value = 0;
    out->valid = false;
    out->was_clamped = false;

    if (!isfinite(raw_value)) {
        return SM_STATUS_INVALID_NUMBER;
    }
    if (minimum > maximum ||
        (unsigned)rounding > (unsigned)SM_ROUND_TRUNCATE) {
        return SM_STATUS_INVALID_RANGE;
    }

    if (raw_value < (double)minimum) {
        out->value = minimum;
        out->valid = true;
        out->was_clamped = true;
        return SM_STATUS_OK;
    }
    if (raw_value > (double)maximum) {
        out->value = maximum;
        out->valid = true;
        out->was_clamped = true;
        return SM_STATUS_OK;
    }

    switch (rounding) {
        case SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO:
            rounded = raw_value >= 0.0
                ? floor(raw_value + 0.5)
                : ceil(raw_value - 0.5);
            break;
        case SM_ROUND_FLOOR: rounded = floor(raw_value); break;
        case SM_ROUND_CEIL: rounded = ceil(raw_value); break;
        case SM_ROUND_TRUNCATE: rounded = trunc(raw_value); break;
        default: return SM_STATUS_INVALID_RANGE;
    }

    if (rounded < (double)minimum) {
        rounded = (double)minimum;
        out->was_clamped = true;
    } else if (rounded > (double)maximum) {
        rounded = (double)maximum;
        out->was_clamped = true;
    }
    out->value = (int)rounded;
    out->valid = true;
    return SM_STATUS_OK;
}

sm_status sm_reduce_ratio(
    double numerator, double denominator, double scale,
    int minimum, int maximum, sm_rounding rounding,
    sm_reduction *out) {

    double ratio;

    if (out == NULL) {
        return SM_STATUS_INVALID_ARGUMENT;
    }
    out->value = 0;
    out->valid = false;
    out->was_clamped = false;
    if (!isfinite(numerator) || !isfinite(denominator) ||
        !isfinite(scale) || denominator == 0.0) {
        return SM_STATUS_INVALID_NUMBER;
    }
    ratio = (numerator / denominator) * scale;
    if (!isfinite(ratio)) {
        return SM_STATUS_INVALID_NUMBER;
    }
    return sm_reduce_integer(ratio, minimum, maximum, rounding, out);
}

const char *sm_status_string(sm_status status) {
    switch (status) {
        case SM_STATUS_OK: return "ok";
        case SM_STATUS_INVALID_ARGUMENT: return "invalid argument";
        case SM_STATUS_INVALID_NUMBER: return "invalid number";
        case SM_STATUS_INVALID_RANGE: return "invalid range";
        case SM_STATUS_INCOMPATIBLE_OBSERVATIONS: return "incompatible observations";
        case SM_STATUS_CAPACITY_EXCEEDED: return "capacity exceeded";
        case SM_STATUS_NOT_FOUND: return "not found";
        default: return "unknown status";
    }
}

const char *sm_domain_string(sm_domain domain) {
    switch (domain) {
        case SM_DOMAIN_CONTEXT: return "context";
        case SM_DOMAIN_LEARNING: return "learning";
        case SM_DOMAIN_SKILL: return "skill";
        case SM_DOMAIN_WELLBEING: return "wellbeing";
        case SM_DOMAIN_BIOLOGY: return "biology";
        case SM_DOMAIN_HEALTH: return "health";
        case SM_DOMAIN_MOTOR_TASK: return "motor-task";
        case SM_DOMAIN_RELATIONSHIP: return "relationship";
        case SM_DOMAIN_EVIDENCE: return "research-evidence";
        default: return "unknown";
    }
}

const char *sm_source_string(sm_source source) {
    switch (source) {
        case SM_SOURCE_SELF_REPORT: return "self-report";
        case SM_SOURCE_DIRECT_OBSERVATION: return "direct-observation";
        case SM_SOURCE_STANDARDIZED_INSTRUMENT: return "standardized-instrument";
        case SM_SOURCE_QUALIFIED_PROFESSIONAL: return "qualified-professional";
        case SM_SOURCE_PEER_REVIEWED_RESEARCH: return "peer-reviewed-research";
        case SM_SOURCE_ANIMAL_MODEL: return "animal-model";
        default: return "unknown";
    }
}
