/* bestof_smoke.c -- C-level smoke test for the best-of selection engine.
 *
 * Verifies the honest selection contract: a measured-good route beats an
 * unproven one; weights actually steer the choice; version and cost gates
 * exclude candidates; certainty tracks delivered fraction; and the winning
 * choice reports the parts of the internet selected.
 */
#include "sleela_bestof.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;
static void check(int cond, const char* what) {
    if (!cond) { printf("FAIL: %s\n", what); failures++; }
}

int main(void) {
    SLBestOf* b = slbestof_new();
    check(b != NULL, "selector opens");
    if (!b) { printf("bestof smoke: FAILED\n"); return 1; }

    /* Three candidate routes with different settings/flags/versions/costs. */
    int fast = slbestof_add_candidate(b, "fast-edge", "sdps://edge:19866",
                                      200, 64, 5, SL_BESTOF_FLAG_CRYPTO | SL_BESTOF_FLAG_PACING,
                                      2, 10, 3);
    int slow = slbestof_add_candidate(b, "slow-relay", "tcp://relay:8080",
                                      2000, 512, 0, SL_BESTOF_FLAG_RETRY,
                                      1, 30, 1);
    int unproven = slbestof_add_candidate(b, "spare", "tcp://spare:9090",
                                          1000, 128, 0, 0, 2, 5, 1);
    check(fast == 0 && slow == 1 && unproven == 2, "three candidates registered");

    /* Fold honest Synchro-style measurements: fast route is quick + reliable;
     * slow route is slow with some loss; spare route is never measured. */
    for (int i = 0; i < 10; i++) slbestof_record(b, fast, 800 + (i % 3) * 50);  /* ~0.8ms */
    for (int i = 0; i < 10; i++) slbestof_record(b, slow, (i % 4 == 0) ? -1 : 40000); /* 40ms + 25% loss */
    /* spare: no measurements -> unproven */

    check(slbestof_mean_us(b, fast) > 0 && slbestof_mean_us(b, fast) < 2000, "fast mean is sub-2ms");
    check(slbestof_loss_permille(b, slow) >= 200, "slow route shows real loss");
    check(slbestof_certainty_permille(b, unproven) == 0, "unproven route has zero certainty (never assumed good)");
    check(slbestof_certainty_permille(b, fast) > slbestof_certainty_permille(b, slow),
          "fast route earns higher certainty than lossy slow route");

    /* Data-weighted: the measured-fast route should win. */
    slbestof_weight(b, SL_BESTOF_AXIS_DATA, 70);
    slbestof_weight(b, SL_BESTOF_AXIS_DECISIONS, 20);
    slbestof_weight(b, SL_BESTOF_AXIS_COSTS, 5);
    slbestof_weight(b, SL_BESTOF_AXIS_VERSIONS, 5);
    int best = slbestof_best(b);
    check(best == fast, "data-weighted best-of picks the measured-fast route");
    check(slbestof_score(b, fast) > slbestof_score(b, unproven),
          "a measured-good route outscores an unproven one");

    /* Version gate: require v2 -> the slow v1 route is excluded. */
    slbestof_min_version(b, 2);
    check(slbestof_best(b) != slow, "min-version gate excludes the v1 route");

    /* Cost budget: cap at 8 -> only the spare (cost 5) qualifies among v>=2. */
    slbestof_min_version(b, 0);
    slbestof_cost_budget(b, 8);
    int budgeted = slbestof_best(b);
    check(budgeted == unproven, "cost budget selects the only affordable candidate");

    /* The winning choice reports the parts of the internet selected. */
    slbestof_cost_budget(b, 0);
    slbestof_min_version(b, 0);
    char choice[512];
    int n = slbestof_choice(b, choice, sizeof(choice));
    check(n > 0 && strstr(choice, "sdps://edge:19866") != NULL, "choice names the winning route");
    check(strstr(choice, "flags=[") != NULL, "choice names the selected flags");
    printf("choice: %s\n", choice);

    char report[2048];
    slbestof_report(b, report, sizeof(report));
    printf("%s", report);

    slbestof_close(b);

    if (failures) { printf("bestof smoke: FAILED (%d)\n", failures); return 1; }
    printf("bestof smoke: PASS\n");
    return 0;
}
