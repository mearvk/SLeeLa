#include "sleelavirin_scan.h"
#include <stdlib.h>
#include <string.h>
typedef struct {
    sleelavirin_known_file_t item;
    uint64_t key;
    uint8_t used;
} sleelavirin_slot_t;
struct sleelavirin_hashmap {
    size_t capacity;
    size_t count;
    sleelavirin_slot_t *slots;
};
static uint64_t hash64(const uint8_t digest[32])
{
    uint64_t h = UINT64_C(1469598103934665603);
    size_t i;
    for (i = 0; i < 32; ++i) {
        h ^= digest[i];
        h *= UINT64_C(1099511628211);
    }
    return h;
}
static int digest_equal(const uint8_t a[32], const uint8_t b[32])
{
    return memcmp(a, b, 32) == 0;
}
static uint32_t popcount64(uint64_t v)
{
    uint32_t n = 0;
    while (v != 0) { v &= v - 1; ++n; }
    return n;
}
static size_t next_pow2(size_t value)
{
    size_t n = 1;
    while (n < value && n <= ((size_t)-1) / 2) n <<= 1;
    return n;
}
sleelavirin_hashmap_t *sleelavirin_hashmap_create(size_t capacity)
{
    sleelavirin_hashmap_t *map;
    size_t actual = next_pow2(capacity < 8 ? 8 : capacity);
    if (actual < capacity) return NULL;
    map = (sleelavirin_hashmap_t *)calloc(1, sizeof(*map));
    if (!map) return NULL;
    map->slots = (sleelavirin_slot_t *)calloc(actual, sizeof(*map->slots));
    if (!map->slots) { free(map); return NULL; }
    map->capacity = actual;
    return map;
}
void sleelavirin_hashmap_destroy(sleelavirin_hashmap_t *map)
{
    if (!map) return;
    free(map->slots);
    free(map);
}
int sleelavirin_hashmap_add(sleelavirin_hashmap_t *map,
                            const sleelavirin_known_file_t *known)
{
    size_t index, probes = 0;
    uint64_t key;
    if (!map || !known || map->count * 10 >= map->capacity * 7) return -1;
    key = hash64(known->fingerprint.sha256);
    index = (size_t)(key & (map->capacity - 1));
    while (probes++ < map->capacity) {
        sleelavirin_slot_t *slot = &map->slots[index];
        if (!slot->used) {
            slot->item = *known; slot->key = key; slot->used = 1; ++map->count;
            return 0;
        }
        if (slot->key == key &&
            digest_equal(slot->item.fingerprint.sha256, known->fingerprint.sha256))
            return 1;
        index = (index + 1) & (map->capacity - 1);
    }
    return -1;
}
int sleelavirin_hashmap_scan(const sleelavirin_hashmap_t *map,
                             const sleelavirin_file_fingerprint_t *file,
                             uint32_t max_feature_distance,
                             sleelavirin_match_t *out)
{
    size_t index, probes = 0;
    uint64_t key;
    if (!map || !file || !out) return -1;
    memset(out, 0, sizeof(*out));
    out->kind = SLEELAVIRIN_UNKNOWN;
    key = hash64(file->sha256);
    index = (size_t)(key & (map->capacity - 1));
    while (probes++ < map->capacity) {
        const sleelavirin_slot_t *slot = &map->slots[index];
        if (!slot->used) break;
        if (slot->key == key &&
            digest_equal(slot->item.fingerprint.sha256, file->sha256)) {
            out->kind = SLEELAVIRIN_EXACT_MATCH;
            out->confidence = 100;
            out->known = &slot->item;
            return 0;
        }
        index = (index + 1) & (map->capacity - 1);
    }
    if (file->feature_mask != 0) {
        size_t i;
        for (i = 0; i < map->capacity; ++i) {
            const sleelavirin_slot_t *slot = &map->slots[i];
            uint64_t mask;
            uint32_t distance, confidence;
            if (!slot->used) continue;
            mask = file->feature_mask & slot->item.fingerprint.feature_mask;
            if (mask == 0) continue;
            distance = popcount64((file->feature_fp ^ slot->item.fingerprint.feature_fp) & mask);
            if (distance <= max_feature_distance) {
                confidence = (uint32_t)(((uint64_t)(64u - distance) * 100u) / 64u);
                out->kind = SLEELAVIRIN_SEMI_MATCH;
                out->confidence = (uint8_t)(confidence > 99u ? 99u : confidence);
                out->distance_bits = distance;
                out->known = &slot->item;
                return 0;
            }
        }
    }
    out->kind = SLEELAVIRIN_CLEAN;
    out->confidence = 100;
    return 0;
}
