#include "crypto_route_map.h"

#include <string.h>

http3_crypto_grade_t http3_grade_for_risk(uint8_t measured_risk)
{
    if (measured_risk >= 3U) {
        return HTTP3_CRYPTO_CR4;
    }
    if (measured_risk == 2U) {
        return HTTP3_CRYPTO_CR3;
    }
    if (measured_risk == 1U) {
        return HTTP3_CRYPTO_CR2;
    }
    return HTTP3_CRYPTO_CR1;
}

static http3_crypto_grade_t max_grade(http3_crypto_grade_t a,
                                       http3_crypto_grade_t b)
{
    return (a > b) ? a : b;
}

int http3_route_evaluate(const http3_route_policy_t *policy,
                         const http3_route_node_t *nodes,
                         size_t node_count,
                         http3_route_decision_t *decision,
                         http3_crypto_grade_t *country_capsule_grades,
                         size_t capsule_capacity)
{
    size_t i;
    http3_crypto_grade_t grade;
    size_t capsule_count = 0U;

    if (policy == NULL || nodes == NULL || decision == NULL || node_count == 0U) {
        return -1;
    }

    grade = policy->minimum_grade;
    grade = max_grade(grade, policy->source_grade);
    grade = max_grade(grade, policy->destination_grade);

    for (i = 0U; i < node_count; ++i) {
        http3_crypto_grade_t risk_grade = http3_grade_for_risk(nodes[i].measured_risk);
        grade = max_grade(grade, risk_grade);

        if (policy->allow_jurisdictional_capsules &&
            nodes[i].emergency_capsule_allowed &&
            country_capsule_grades != NULL &&
            capsule_count < capsule_capacity) {
            country_capsule_grades[capsule_count++] = risk_grade;
        }
    }

    if (policy->strengthen_after_threshold &&
        node_count > 0U &&
        (node_count - 1U) > policy->hop_threshold) {
        grade = max_grade(grade, HTTP3_CRYPTO_CR3);
    }

    decision->grade = grade;
    decision->hop_count = node_count - 1U;
    decision->capsule_country_count = capsule_count;
    decision->reason =
        "Protection is monotonic: route conditions may strengthen the selected grade but do not silently downgrade it.";

    return 0;
}
