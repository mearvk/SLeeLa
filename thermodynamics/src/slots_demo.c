/*
 * slots_demo.c - self-test for the enforced slot caps
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Verifies the three caps from MATH.KNOWNS.md are actually enforced:
 *   futures <= 22, positive gains <= 6, long-term confidences <= 2.
 */
#include "thermodynamics.h"

#include <math.h>
#include <stdio.h>

static int check(const char *name, int ok)
{
    printf("  [%s] %s\n", ok ? "PASS" : "FAIL", name);
    return ok ? 0 : 1;
}

int main(void)
{
    int failures = 0;
    puts("SLeeLa thermodynamics - enforced slot caps (22 / 6 / 2)");

    /* The cap macros must match the recorded assumptions exactly. */
    failures += check("FUTURES_MAX == 22", SL_THERMO_FUTURES_MAX == 22u);
    failures += check("POSITIVE_GAINS_MAX == 6", SL_THERMO_POSITIVE_GAINS_MAX == 6u);
    failures += check("LONG_TERM_CONFIDENCES_MAX == 2", SL_THERMO_LONG_TERM_CONFIDENCES_MAX == 2u);

    sl_thermo_slots s;
    sl_thermo_slots_init(&s, 100.0 /* baseline */);
    failures += check("init starts empty", s.future_count == 0 &&
                                           s.positive_count == 0 &&
                                           s.long_term_count == 0);

    /* Offer exactly 22 futures; all must be accepted. Values 90..111 so that
     * the ones above baseline 100 are the positive gains. Confidences chosen so
     * a few clear the long-term threshold. */
    int accepted = 0;
    for (int n = 0; n < 22; ++n) {
        const double value = 90.0 + n;              /* 90 .. 111 */
        const double conf = (n % 7 == 0) ? 0.95 : 0.5; /* n=0,7,14,21 -> high */
        const int rc = sl_thermo_slots_offer(&s, value, conf, 0.9);
        if (rc == 0) {
            ++accepted;
        }
    }
    failures += check("all 22 futures accepted", accepted == 22);
    failures += check("futures slot holds exactly 22", s.future_count == 22u);

    /* The 23rd future must be rejected with code 2 (hard cap). */
    const int rc23 = sl_thermo_slots_offer(&s, 999.0, 0.99, 0.9);
    failures += check("23rd future rejected (cap enforced)", rc23 == 2);
    failures += check("count did not grow past 22", s.future_count == 22u);

    /* Futures are kept sorted ascending. */
    int sorted = 1;
    for (size_t i = 1; i < s.future_count; ++i) {
        if (s.futures[i] < s.futures[i - 1]) { sorted = 0; break; }
    }
    failures += check("futures stored in ascending order", sorted);
    printf("      futures span [%.1f .. %.1f]\n", s.futures[0], s.futures[s.future_count - 1]);

    /* Positive gains: values above 100 were 101..111 = 11 candidates, capped
     * at 6, keeping the largest. So the slot must hold 6, min >= 106. */
    failures += check("positive gains capped at 6", s.positive_count == 6u);
    failures += check("positive gains kept the strongest (min >= 106)",
                      s.positive_gains[0] >= 106.0 - 1e-9);
    printf("      positive gains: ");
    for (size_t i = 0; i < s.positive_count; ++i) printf("%.0f ", s.positive_gains[i]);
    printf("\n");

    /* Long-term confidences: capped at 2, keeping the highest. We offered
     * several at 0.95; the slot must hold exactly 2. */
    failures += check("long-term confidences capped at 2", s.long_term_count == 2u);
    failures += check("long-term kept high confidence (>= 0.9)",
                      s.long_term[0] >= 0.9 - 1e-9);
    printf("      long-term confidences: ");
    for (size_t i = 0; i < s.long_term_count; ++i) printf("%.2f ", s.long_term[i]);
    printf("\n");

    /* Argument validation. */
    failures += check("null collector rejected",
                      sl_thermo_slots_offer(0, 1.0, 0.5, 0.9) == 1);
    sl_thermo_slots s2;
    sl_thermo_slots_init(&s2, 0.0);
    failures += check("out-of-range confidence rejected",
                      sl_thermo_slots_offer(&s2, 1.0, 1.5, 0.9) == 1);

    if (failures == 0) {
        puts("\nAll slot-cap checks passed.");
        return 0;
    }
    printf("\n%d check(s) failed.\n", failures);
    return 1;
}
