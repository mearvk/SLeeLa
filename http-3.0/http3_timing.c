/* ==========================================================================
 * http3_timing.c -- connection-level timing: max speed, carrier certainty,
 * on-time arrival, and temporal balance. Advisory; no wire change.
 * ========================================================================== */
#include "http3_timing.h"

void http3_timing_init(http3_timing_t *t)
{
    if (t == NULL) {
        return;
    }
    t->min_gap_ns = HTTP3_TIMING_DEFAULT_MIN_GAP_NS;
    t->lateness_ns = HTTP3_TIMING_DEFAULT_LATENESS_NS;
    t->balance_ns = HTTP3_TIMING_DEFAULT_BALANCE_NS;
    t->have_last = 0;
    t->last_arrival_ns = 0;
    t->mean_gap_ns = 0.0;
    t->observed = 0;
    t->certainty = 1.0; /* optimistic until evidence says otherwise */
}

void http3_timing_configure(http3_timing_t *t,
                            uint64_t min_gap_ns,
                            uint64_t lateness_ns,
                            uint64_t balance_ns)
{
    if (t == NULL) {
        return;
    }
    if (min_gap_ns != 0ull)  { t->min_gap_ns = min_gap_ns; }
    if (lateness_ns != 0ull) { t->lateness_ns = lateness_ns; }
    if (balance_ns != 0ull)  { t->balance_ns = balance_ns; }
}

unsigned http3_timing_observe(http3_timing_t *t,
                              uint64_t arrival_ns,
                              uint64_t deadline_ns)
{
    unsigned flags = HTTP3_TIMING_OK;
    double w;
    if (t == NULL) {
        return HTTP3_TIMING_OK;
    }

    /* ON TIME: only meaningful when a deadline is supplied. */
    if (deadline_ns != 0ull && arrival_ns > deadline_ns + t->lateness_ns) {
        flags |= HTTP3_TIMING_LATE;
    }

    if (t->have_last) {
        uint64_t gap = (arrival_ns >= t->last_arrival_ns)
                           ? (arrival_ns - t->last_arrival_ns)
                           : 0ull;

        /* MAX SPEED: a gap smaller than the minimum is faster than allowed. */
        if (gap < t->min_gap_ns) {
            flags |= HTTP3_TIMING_OVER_RATE;
        }

        /* BALANCED: jitter = |gap - running mean| beyond the band. Skip the
         * very first gap (mean not yet established). */
        if (t->observed >= 2u) {
            double dev = (double)gap - t->mean_gap_ns;
            if (dev < 0.0) {
                dev = -dev;
            }
            if (dev > (double)t->balance_ns) {
                flags |= HTTP3_TIMING_UNBALANCED;
            }
        }

        /* Update running mean gap (simple EWMA, alpha = 1/window). */
        {
            double alpha = 1.0 / (double)HTTP3_TIMING_CERTAINTY_WINDOW;
            if (t->observed == 1u) {
                t->mean_gap_ns = (double)gap; /* seed with first gap */
            } else {
                t->mean_gap_ns += alpha * ((double)gap - t->mean_gap_ns);
            }
        }
    }

    /* CARRIER CERTAINTY: EWMA of "this packet was clean (flags == 0)". */
    w = 1.0 / (double)HTTP3_TIMING_CERTAINTY_WINDOW;
    {
        double clean = (flags == HTTP3_TIMING_OK) ? 1.0 : 0.0;
        t->certainty += w * (clean - t->certainty);
        if (t->certainty < 0.0) { t->certainty = 0.0; }
        if (t->certainty > 1.0) { t->certainty = 1.0; }
    }

    t->last_arrival_ns = arrival_ns;
    t->have_last = 1;
    ++t->observed;
    return flags;
}

double http3_timing_certainty(const http3_timing_t *t)
{
    return (t != NULL) ? t->certainty : 0.0;
}

const char *http3_timing_flag_name(http3_timing_flag_t flag)
{
    switch (flag) {
        case HTTP3_TIMING_OK:         return "OK";
        case HTTP3_TIMING_OVER_RATE:  return "OVER_RATE";
        case HTTP3_TIMING_LATE:       return "LATE";
        case HTTP3_TIMING_UNBALANCED: return "UNBALANCED";
        default:                      return "MIXED";
    }
}
