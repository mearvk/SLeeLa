#ifndef SLEELAVIRIN_HEURISTIC_H
#define SLEELAVIRIN_HEURISTIC_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELAVIRIN_HEURISTIC_MAX_EVIDENCE 32u
#define SLEELAVIRIN_TRAP2_VERSION 1u
typedef struct {
    const char *id;
    uint16_t weight;
    uint8_t observed;
    const char *reason;
} sleelavirin_indicator_t;
typedef struct {
    const char *operator_name;
    const char *license_id;
    const char *basis_id;
    uint8_t operator_acknowledged;
    uint8_t allow_action;
    uint16_t threshold;
    uint16_t trap_threshold;
} sleelavirin_trap2_profile_t;
typedef struct {
    uint16_t score;
    uint8_t triggered;
    uint8_t action_permitted;
    uint8_t evidence_count;
    uint8_t trap_level;
} sleelavirin_heuristic_result_t;
int sleelavirin_trap2_evaluate(
    const sleelavirin_trap2_profile_t *profile,
    const sleelavirin_indicator_t *indicators,
    size_t indicator_count,
    sleelavirin_heuristic_result_t *out);
#ifdef __cplusplus
}
#endif
#endif
