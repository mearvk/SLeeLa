#ifndef SLEELAVIRIN_SCAN_H
#define SLEELAVIRIN_SCAN_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELAVIRIN_SHA256_HEX 65u
#define SLEELAVIRIN_MAX_NAME 256u
#define SLEELAVIRIN_MAX_PROVIDER 128u
#define SLEELAVIRIN_MAX_RULE 128u
typedef enum {
    SLEELAVIRIN_CLEAN = 0,
    SLEELAVIRIN_EXACT_MATCH,
    SLEELAVIRIN_SEMI_MATCH,
    SLEELAVIRIN_SUSPICIOUS,
    SLEELAVIRIN_UNKNOWN,
    SLEELAVIRIN_SCAN_ERROR,
    SLEELAVIRIN_UNSUPPORTED
} sleelavirin_match_kind_t;
typedef struct {
    uint8_t sha256[32];
    uint64_t feature_fp;
    uint64_t feature_mask;
    uint64_t file_size;
} sleelavirin_file_fingerprint_t;
typedef struct {
    char name[SLEELAVIRIN_MAX_NAME];
    char provider[SLEELAVIRIN_MAX_PROVIDER];
    char rule_id[SLEELAVIRIN_MAX_RULE];
    sleelavirin_file_fingerprint_t fingerprint;
    uint8_t severity;
} sleelavirin_known_file_t;
typedef struct sleelavirin_hashmap sleelavirin_hashmap_t;
typedef struct {
    sleelavirin_match_kind_t kind;
    uint8_t confidence;
    uint32_t distance_bits;
    const sleelavirin_known_file_t *known;
} sleelavirin_match_t;
sleelavirin_hashmap_t *sleelavirin_hashmap_create(size_t capacity);
void sleelavirin_hashmap_destroy(sleelavirin_hashmap_t *map);
int sleelavirin_hashmap_add(sleelavirin_hashmap_t *map,
                            const sleelavirin_known_file_t *known);
int sleelavirin_hashmap_scan(const sleelavirin_hashmap_t *map,
                             const sleelavirin_file_fingerprint_t *file,
                             uint32_t max_feature_distance,
                             sleelavirin_match_t *out);
#ifdef __cplusplus
}
#endif
#endif
