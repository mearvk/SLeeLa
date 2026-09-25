#include "../src/sleelavirin_scan.h"
#include "../src/sleelavirin_heuristic.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static void fill_digest(uint8_t digest[32], uint8_t value)
{
    size_t i;
    for (i = 0; i < 32; ++i) digest[i] = value;
}
int main(void)
{
    sleelavirin_hashmap_t *map = sleelavirin_hashmap_create(16);
    sleelavirin_known_file_t known;
    sleelavirin_file_fingerprint_t file;
    sleelavirin_match_t match;
    sleelavirin_indicator_t indicators[2] = {
        {"structure.anomaly", 40, 1, "declared structure is inconsistent"},
        {"operator.trap", 70, 1, "operator-defined trap basis observed"}
    };
    sleelavirin_trap2_profile_t profile = {
        "operator", "SLEELAVIRIN-TEST-LICENSE", "basis.test.1", 1, 1, 50, 100
    };
    sleelavirin_heuristic_result_t heuristic;
    assert(map);
    memset(&known, 0, sizeof(known));
    strcpy(known.name, "known-test-artifact");
    strcpy(known.provider, "test");
    strcpy(known.rule_id, "hash.exact.1");
    fill_digest(known.fingerprint.sha256, 0x11);
    known.fingerprint.feature_fp = UINT64_C(0x0f0f0f0f0f0f0f0f);
    known.fingerprint.feature_mask = UINT64_MAX;
    assert(sleelavirin_hashmap_add(map, &known) == 0);
    file = known.fingerprint;
    assert(sleelavirin_hashmap_scan(map, &file, 2, &match) == 0);
    assert(match.kind == SLEELAVIRIN_EXACT_MATCH);
    assert(match.confidence == 100);
    fill_digest(file.sha256, 0x22);
    file.feature_fp = UINT64_C(0x0f0f0f0f0f0f0f0e);
    file.feature_mask = UINT64_MAX;
    assert(sleelavirin_hashmap_scan(map, &file, 2, &match) == 0);
    assert(match.kind == SLEELAVIRIN_SEMI_MATCH);
    assert(sleelavirin_trap2_evaluate(&profile, indicators, 2, &heuristic) == 0);
    assert(heuristic.trap_level == 2);
    assert(heuristic.action_permitted == 1);
    sleelavirin_hashmap_destroy(map);
    puts("Sleelavirin scan/Trap-2 selftest: PASS");
    return 0;
}
