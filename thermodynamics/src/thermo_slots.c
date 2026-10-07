/*
 * thermo_slots.c - enforced slot caps for sorting futures
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Turns the design assumptions in MATH.KNOWNS.md into real, bounds-checked
 * behavior:
 *   - at most SL_THERMO_FUTURES_MAX (22) futures, kept sorted ascending;
 *   - at most SL_THERMO_POSITIVE_GAINS_MAX (6) futures above a baseline;
 *   - at most SL_THERMO_LONG_TERM_CONFIDENCES_MAX (2) long-term confident ones,
 *     keeping the highest confidences.
 *
 * The futures slot is a hard cap: once full, further offers are rejected (so
 * nothing is silently lost without a return code). The gains and long-term
 * slots are "keep the best" caps: they stay within capacity by retaining the
 * strongest entries, which is the natural meaning of "sort the top N".
 */
#include "thermodynamics.h"

void sl_thermo_slots_init(sl_thermo_slots *s, double baseline)
{
    if (!s) {
        return;
    }
    s->future_count = 0;
    s->positive_count = 0;
    s->long_term_count = 0;
    s->baseline = baseline;
    for (size_t i = 0; i < SL_THERMO_FUTURES_MAX; ++i) {
        s->futures[i] = 0.0;
    }
    for (size_t i = 0; i < SL_THERMO_POSITIVE_GAINS_MAX; ++i) {
        s->positive_gains[i] = 0.0;
    }
    for (size_t i = 0; i < SL_THERMO_LONG_TERM_CONFIDENCES_MAX; ++i) {
        s->long_term[i] = 0.0;
    }
}

/* Insert `value` into a sorted-ascending array of `count` entries that already
 * holds its own confidences in a parallel array when `conf` is non-NULL. Here
 * we only need numeric insertion sort for the futures array. */
static void insert_sorted(double *arr, size_t count, double value)
{
    size_t pos = count;
    while (pos > 0 && arr[pos - 1] > value) {
        arr[pos] = arr[pos - 1];
        --pos;
    }
    arr[pos] = value;
}

int sl_thermo_slots_offer(sl_thermo_slots *s, double value, double confidence,
                          double long_term_threshold)
{
    if (!s || confidence < 0.0 || confidence > 1.0) {
        return 1;
    }

    /* Hard cap on futures: reject once full. */
    if (s->future_count >= SL_THERMO_FUTURES_MAX) {
        return 2;
    }
    insert_sorted(s->futures, s->future_count, value);
    s->future_count += 1;

    /* Positive gain: value strictly above baseline. "Keep the best" cap: if
     * full, replace the smallest stored gain only when `value` beats it. */
    if (value > s->baseline) {
        if (s->positive_count < SL_THERMO_POSITIVE_GAINS_MAX) {
            insert_sorted(s->positive_gains, s->positive_count, value);
            s->positive_count += 1;
        } else {
            /* positive_gains is sorted ascending; index 0 is the weakest. */
            if (value > s->positive_gains[0]) {
                /* drop the weakest, re-insert the new value in order */
                for (size_t i = 1; i < SL_THERMO_POSITIVE_GAINS_MAX; ++i) {
                    s->positive_gains[i - 1] = s->positive_gains[i];
                }
                insert_sorted(s->positive_gains, SL_THERMO_POSITIVE_GAINS_MAX - 1, value);
            }
        }
    }

    /* Long-term confidence: confidence at or above threshold. "Keep the best"
     * cap by confidence. We store the confidence values themselves so the slot
     * reflects the strongest long-term convictions. */
    if (confidence >= long_term_threshold) {
        if (s->long_term_count < SL_THERMO_LONG_TERM_CONFIDENCES_MAX) {
            insert_sorted(s->long_term, s->long_term_count, confidence);
            s->long_term_count += 1;
        } else {
            if (confidence > s->long_term[0]) {
                for (size_t i = 1; i < SL_THERMO_LONG_TERM_CONFIDENCES_MAX; ++i) {
                    s->long_term[i - 1] = s->long_term[i];
                }
                insert_sorted(s->long_term, SL_THERMO_LONG_TERM_CONFIDENCES_MAX - 1, confidence);
            }
        }
    }

    return 0;
}
