/* ==========================================================================
 * sleela_bestof.h -- Best-of: configurable route/accuracy selection for
 * Synchro measurement and Munction/RMI packet transport.
 *
 * A "best-of" is a small, transparent selection engine. It holds a bounded set
 * of CANDIDATES -- each a bundle of an internet ROUTE (endpoint/URI) plus its
 * SETTINGS (timeout, payload size, min-gap), FLAGS (crypto/retry/pacing/dscp/
 * replay), an internet QoS ARCHITECTURE (best-effort / DiffServ / IntServ /
 * MPLS -- see NETWORK.md), a protocol VERSION, and a COST -- and it is
 * configured with WEIGHTS across five axes:
 *
 *     data          how much the measured quality (latency/loss/jitter) matters
 *     decisions     how much the advisory QoS certainty (QOS.md) matters
 *     costs         how much the candidate's cost counts against it
 *     versions      how much protocol/packet version fitness matters
 *     architecture  how much the realized QoS architecture fitness matters
 *
 * Measurements from Synchro (or any honest source) are folded in per candidate
 * with bestof_record(). bestof_best() then picks the highest-scoring candidate
 * that satisfies the minimum version and the cost budget, and bestof_choice()
 * reports exactly which parts of the internet (route, settings, flags, replays)
 * were chosen -- so a caller can adjust Synchro accuracy or aim Munction/RMI
 * packets down the selected route.
 *
 * The engine is honest, like the rest of the timing work: it SELECTS and
 * REPORTS from measured data; it never fabricates accuracy and never promises a
 * delivery guarantee the transport cannot make. A candidate with no
 * measurements is unproven and scored as such, never assumed good.
 *
 * A best-of is a Sleela-owned bounded resource (a handle into a fixed table),
 * the same discipline used for sockets/synchro/munction.
 * ========================================================================== */
#ifndef SLEELA_BESTOF_H
#define SLEELA_BESTOF_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Max concurrent best-of selectors owned by one VM. */
#define SL_BESTOF_MAX 32
/* Max candidate routes per selector. */
#define SL_BESTOF_CANDIDATES 64

/* Weight axes (bestof_weight). */
#define SL_BESTOF_AXIS_DATA         0  /* measured latency/loss/jitter quality */
#define SL_BESTOF_AXIS_DECISIONS    1  /* advisory QoS carrier certainty        */
#define SL_BESTOF_AXIS_COSTS        2  /* cost penalty                          */
#define SL_BESTOF_AXIS_VERSIONS     3  /* protocol/packet version fitness       */
#define SL_BESTOF_AXIS_ARCHITECTURE 4  /* realized QoS-architecture fitness     */

/* Candidate flags (bitset). These name the internet "settings/flags" a route
 * may carry; they are advisory selection inputs, not transport guarantees. */
#define SL_BESTOF_FLAG_CRYPTO  0x01  /* sealed transport preferred            */
#define SL_BESTOF_FLAG_RETRY   0x02  /* retry-on-bump allowed                  */
#define SL_BESTOF_FLAG_PACING  0x04  /* honor min-gap pacing                   */
#define SL_BESTOF_FLAG_DSCP    0x08  /* set DSCP/ToS class where supported     */
#define SL_BESTOF_FLAG_REPLAY  0x10  /* replay/average multiple probes         */

/* Internet QoS architecture a candidate route is served by (see NETWORK.md
 * "Internet Architectures"). Best-of stays advisory: naming an architecture
 * declares how the route asks the network to treat its packets; whether the
 * network honors it is measured, never assumed. Each architecture carries one
 * integer parameter (see slbestof_candidate_arch): */
#define SL_BESTOF_ARCH_BESTEFFORT 0  /* no QoS marking; ordinary best-effort   */
#define SL_BESTOF_ARCH_DIFFSERV   1  /* DiffServ: hop-by-hop DSCP marking      */
                                     /*   param = DSCP class (0..63)           */
#define SL_BESTOF_ARCH_INTSERV    2  /* IntServ: end-to-end RSVP reservation   */
                                     /*   param = reserved kbps (0 = none yet) */
#define SL_BESTOF_ARCH_MPLS       3  /* MPLS: label-switched, traffic-engineered */
                                     /*   param = MPLS label (0 = unlabeled)   */

/* Realization state of a candidate's architecture, reported honestly: a route
 * may REQUEST an architecture but the network may not have realized it yet. */
#define SL_BESTOF_ARCH_REQUESTED  0  /* declared, not yet confirmed realized   */
#define SL_BESTOF_ARCH_REALIZED   1  /* confirmed: marked/reserved/labeled OK  */
#define SL_BESTOF_ARCH_DENIED     2  /* the network declined (e.g. RSVP reject) */

typedef struct SLBestOf SLBestOf;

/* Create a best-of selector. Returns a heap selector or NULL on error.
 * Weights default to data=45 decisions=25 costs=10 versions=5 architecture=15;
 * min_version=0; cost_budget = INT unbounded (0 == unbounded). */
SLBestOf* slbestof_new(void);

/* Configure a weight axis (0..100 clamped). Returns 0, or -1 on a bad axis. */
int slbestof_weight(SLBestOf* b, int axis, int weight);

/* Set the minimum acceptable protocol version and the cost budget (0 budget =
 * unbounded). Candidates below min_version or above the budget are excluded
 * from bestof_best(). */
void slbestof_min_version(SLBestOf* b, int min_version);
void slbestof_cost_budget(SLBestOf* b, int cost_budget);

/* Register a candidate route. `name` is a stable label; `route` is the internet
 * endpoint/URI. Returns the candidate index (>=0) or -1 if the table is full.
 * timeout_ms/payload_len/min_gap_ms/flags/version/cost/replays are the settings
 * and flags this route carries. */
int slbestof_add_candidate(SLBestOf* b, const char* name, const char* route,
                           int timeout_ms, int payload_len, int min_gap_ms,
                           int flags, int version, int cost, int replays);

/* Declare the internet QoS ARCHITECTURE a candidate route is served by
 * (SL_BESTOF_ARCH_*), its architecture parameter (DiffServ: DSCP class 0..63;
 * IntServ: reserved kbps; MPLS: label; best-effort: ignored), and the current
 * realization state (SL_BESTOF_ARCH_REQUESTED/REALIZED/DENIED). A route that
 * only *requests* an architecture earns no architecture bonus; one confirmed
 * REALIZED earns the full bonus; a DENIED reservation (e.g. RSVP reject) is a
 * penalty. Call after slbestof_add_candidate. Returns 0 or -1 on a bad idx.
 * A candidate defaults to best-effort/requested when this is not called, so
 * existing callers are unaffected. */
int slbestof_candidate_arch(SLBestOf* b, int idx, int architecture,
                            int arch_param, int realized);

/* Update just the realization state of a candidate's architecture -- e.g. after
 * an RSVP reservation is confirmed or denied, or a DSCP/MPLS path is verified
 * by measurement. Returns 0 or -1 on a bad idx. */
int slbestof_arch_realized(SLBestOf* b, int idx, int realized);

/* Per-candidate architecture accessors. */
int slbestof_arch(const SLBestOf* b, int idx);        /* SL_BESTOF_ARCH_*        */
int slbestof_arch_param(const SLBestOf* b, int idx);  /* DSCP/kbps/label         */
int slbestof_arch_state(const SLBestOf* b, int idx);  /* requested/realized/denied */

/* Fold one honest measurement into candidate `idx`: a measured RTT in
 * microseconds (>=0), or -1 to record a loss. Updates the candidate's running
 * mean/jitter/loss and its carrier certainty. Returns 0 or -1 on a bad idx. */
int slbestof_record(SLBestOf* b, int idx, int64_t rtt_us);

/* The transparent score for candidate `idx` (higher is better), computed from
 * the configured weights and the candidate's measured data. Returns the score,
 * or a large-negative sentinel for a bad/excluded candidate. */
int64_t slbestof_score(SLBestOf* b, int idx);

/* The index of the best candidate that satisfies min_version and cost_budget,
 * or -1 when none qualifies (e.g. nothing measured yet). */
int slbestof_best(SLBestOf* b);

/* Per-candidate measured accessors (microseconds / permille / [0..1000]). */
int64_t slbestof_mean_us(const SLBestOf* b, int idx);
int64_t slbestof_loss_permille(const SLBestOf* b, int idx);
int64_t slbestof_jitter_us(const SLBestOf* b, int idx);
int slbestof_certainty_permille(const SLBestOf* b, int idx); /* 0..1000 */

/* Write the winning choice as a one-line description of the parts of the
 * internet selected: "name route timeout=..ms payload=..B gap=..ms
 * flags=[..] version=.. replays=.. score=..". Returns bytes written (0 if no
 * winner). This is what a caller applies to Synchro/Munction/RMI. */
int slbestof_choice(SLBestOf* b, char* out, size_t cap);

/* Write a multi-line report of every candidate and its score. Returns length. */
int slbestof_report(SLBestOf* b, char* out, size_t cap);

/* Release the selector. Safe on NULL. */
void slbestof_close(SLBestOf* b);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BESTOF_H */
