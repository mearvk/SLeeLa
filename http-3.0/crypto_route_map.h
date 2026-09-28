#ifndef SLEELA_HTTP3_CRYPTO_ROUTE_MAP_H
#define SLEELA_HTTP3_CRYPTO_ROUTE_MAP_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HTTP3_CRYPTO_CR1 = 1,
    HTTP3_CRYPTO_CR2 = 2,
    HTTP3_CRYPTO_CR3 = 3,
    HTTP3_CRYPTO_CR4 = 4
} http3_crypto_grade_t;

typedef struct {
    const char *node_id;
    const char *country_code;
    uint8_t technology_grade;
    uint8_t measured_risk;
    uint8_t emergency_capsule_allowed;
} http3_route_node_t;

typedef struct {
    http3_crypto_grade_t source_grade;
    http3_crypto_grade_t destination_grade;
    http3_crypto_grade_t minimum_grade;
    size_t hop_threshold;
    uint8_t strengthen_after_threshold;
    uint8_t allow_jurisdictional_capsules;
} http3_route_policy_t;

typedef struct {
    http3_crypto_grade_t grade;
    size_t hop_count;
    size_t capsule_country_count;
    const char *reason;
} http3_route_decision_t;

http3_crypto_grade_t http3_grade_for_risk(uint8_t measured_risk);

int http3_route_evaluate(const http3_route_policy_t *policy,
                         const http3_route_node_t *nodes,
                         size_t node_count,
                         http3_route_decision_t *decision,
                         http3_crypto_grade_t *country_capsule_grades,
                         size_t capsule_capacity);

#ifdef __cplusplus
}
#endif

#endif
