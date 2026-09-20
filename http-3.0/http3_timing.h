/* ==========================================================================
 * http3_timing.h -- SLeeLa HTTP 3.0 connection-level timing: max speed,
 * carrier certainty, on-time arrival, and temporal balance.
 *
 * This layer answers four timing concerns for a stream of packets on one
 * connection:
 *
 *   - MAX SPEED     : packets must not arrive faster than a configured rate
 *                     (a minimum inter-arrival gap); a burst is flagged.
 *   - ON TIME       : each packet may carry a deadline window; arriving later
 *                     than the allowed lateness is flagged.
 *   - BALANCED      : arrivals should be evenly paced; jitter (deviation of the
 *                     inter-arrival gap from the running mean) beyond a band is
 *                     flagged as unbalanced.
 *   - CARRIER       : a running "carrier certainty" in [0,1] -- the fraction of
 *     CERTAINTY       recent packets that were on time, balanced, and within
 *                     rate -- an estimate of how dependable the path is.
 *
 * It is ADVISORY and CONNECTION-LEVEL: nothing new travels on the wire, and no
 * packet is rejected by timing. The pipeline observes each arrival and records
 * flags/counters (see http3_pipeline.h). Timestamps are caller-supplied
 * nanoseconds from a monotonic-ish clock; there is no synchronized network
 * clock here, so "on time" is measured against the caller's own reference.
 *
 * Pure data: no I/O, no allocation, no crypto.
 * ========================================================================== */
#ifndef HTTP3_TIMING_H
#define HTTP3_TIMING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Advisory flags returned by http3_timing_observe (bitwise-OR of concerns). */
typedef enum {
    HTTP3_TIMING_OK        = 0x00,
    HTTP3_TIMING_OVER_RATE = 0x01, /* arrived faster than max speed (burst)   */
    HTTP3_TIMING_LATE      = 0x02, /* arrived after its deadline + tolerance  */
    HTTP3_TIMING_UNBALANCED= 0x04  /* inter-arrival jitter outside the band   */
} http3_timing_flag_t;

/* Default configuration constants (all in nanoseconds unless noted). */
#define HTTP3_TIMING_DEFAULT_MIN_GAP_NS    1000ull       /* >= 1us between pkts  */
#define HTTP3_TIMING_DEFAULT_LATENESS_NS   50000000ull   /* 50ms grace          */
#define HTTP3_TIMING_DEFAULT_BALANCE_NS    10000000ull   /* +/-10ms jitter band */
#define HTTP3_TIMING_CERTAINTY_WINDOW      64u           /* EWMA-ish window      */

/*
 * Timing state for one connection. Initialize with http3_timing_init, then call
 * http3_timing_observe once per received packet with its arrival time (and an
 * optional deadline). The struct keeps a running inter-arrival mean and a
 * carrier-certainty estimate.
 */
typedef struct {
    uint64_t min_gap_ns;      /* max speed: minimum allowed inter-arrival gap  */
    uint64_t lateness_ns;     /* on time: allowed lateness past a deadline     */
    uint64_t balance_ns;      /* balanced: allowed jitter around the mean gap  */

    int      have_last;       /* 1 once a first packet has been observed       */
    uint64_t last_arrival_ns; /* arrival time of the previous packet           */
    double   mean_gap_ns;     /* running mean inter-arrival gap                 */
    uint64_t observed;        /* total packets observed                         */

    double   certainty;       /* carrier certainty in [0,1] (running estimate) */
} http3_timing_t;

/* Initialize timing with the default bounds (certainty starts at 1.0). */
void http3_timing_init(http3_timing_t *t);

/* Override the bounds (pass 0 for any field to keep its current value). */
void http3_timing_configure(http3_timing_t *t,
                            uint64_t min_gap_ns,
                            uint64_t lateness_ns,
                            uint64_t balance_ns);

/*
 * Observe one packet arriving at `arrival_ns`. If `deadline_ns` is non-zero it
 * is the time by which the packet should have arrived; arriving later than
 * deadline_ns + lateness_ns sets HTTP3_TIMING_LATE. Updates the running mean
 * gap and the carrier-certainty estimate. Returns the advisory flags for this
 * packet (0 == on time, balanced, within rate).
 */
unsigned http3_timing_observe(http3_timing_t *t,
                              uint64_t arrival_ns,
                              uint64_t deadline_ns);

/* Current carrier-certainty estimate in [0,1]. */
double http3_timing_certainty(const http3_timing_t *t);

/* Human-readable name for a single timing flag (for logs/diagnostics). */
const char *http3_timing_flag_name(http3_timing_flag_t flag);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_TIMING_H */
