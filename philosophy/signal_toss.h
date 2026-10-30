#ifndef SLEELA_PHILOSOPHY_SIGNAL_TOSS_H
#define SLEELA_PHILOSOPHY_SIGNAL_TOSS_H
/*
 * Signal toss: a background worker that samples the event tree for branch
 * consistency at ~44 Hz and emits throttled consistency updates into the
 * 800-1200 updates/hour band.
 *
 * Two cadences on one mechanism:
 *   - SAMPLE cadence (~44 Hz): the worker wakes ~44 times a second and tosses
 *     a signal at a branch to check it. Each sample feeds the consistency
 *     monitor (which accrues +4 per branch / +5 per mature length).
 *   - COMMIT cadence (800-1200/hour): a committed "tree is consistent" update
 *     is emitted only when the throttle interval has elapsed, keeping the
 *     logged consistency-confirmation rate inside the requested band.
 *
 * The worker owns no tree memory. The caller supplies a sampler callback that
 * fills one sl_branch_view per toss (e.g. walking the AVL tree). Threading is
 * provided by thread_compat.* (C11 threads / Win32 / pthreads).
 */
#include "consistency_monitor.h"
#include "thread_compat.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SL_TOSS_SAMPLE_HZ            44u    /* ~44 Hz sampling cadence */
#define SL_TOSS_COMMIT_MIN_PER_HOUR 800u    /* lower bound of commit band */
#define SL_TOSS_COMMIT_MAX_PER_HOUR 1200u   /* upper bound of commit band */

/* Fill *out for one toss. Return true to continue, false to request stop.
 * ctx is the user pointer passed to sl_signal_toss_start. */
typedef bool (*sl_toss_sampler)(sl_branch_view *out, void *ctx);

typedef struct sl_signal_toss sl_signal_toss;

/* Create a toss bound to a sampler. commit_per_hour is clamped into
 * [800, 1200]; pass 0 to use the midpoint (1000). Returns NULL on failure. */
sl_signal_toss *sl_signal_toss_create(
    sl_toss_sampler sampler, void *ctx, unsigned commit_per_hour);

/* Start / stop the background worker. Idempotent. */
int sl_signal_toss_start(sl_signal_toss *toss);
void sl_signal_toss_stop(sl_signal_toss *toss);
void sl_signal_toss_destroy(sl_signal_toss *toss);

/* Thread-safe snapshot of the running tally. */
sl_consistency_report sl_signal_toss_snapshot(sl_signal_toss *toss);

/* Committed consistency updates emitted so far (the throttled 800-1200/hr
 * counter), thread-safe. */
uint64_t sl_signal_toss_commits(sl_signal_toss *toss);

/* The effective commit interval in milliseconds for the chosen rate. */
uint32_t sl_signal_toss_commit_interval_ms(const sl_signal_toss *toss);
/* The sample period in milliseconds (1000 / 44 Hz). */
uint32_t sl_signal_toss_sample_interval_ms(void);

/* Run exactly one toss synchronously (no thread). Useful for tests and for
 * callers that drive their own loop. Returns false once the sampler asks to
 * stop. Updates the internal report and may emit a commit. */
bool sl_signal_toss_step(sl_signal_toss *toss, uint64_t now_ms);

#ifdef __cplusplus
}
#endif
#endif
