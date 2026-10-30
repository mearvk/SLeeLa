#include "signal_toss.h"
#include <stdlib.h>

struct sl_signal_toss {
    sl_toss_sampler sampler;
    void *ctx;
    sl_consistency_report report;
    uint64_t commits;            /* throttled consistency updates emitted */
    uint64_t last_commit_ms;     /* monotonic time of last commit */
    uint32_t commit_interval_ms; /* derived from commit_per_hour */
    unsigned commit_per_hour;
    sl_thread thread;
    sl_mutex lock;
    bool running;
    bool thread_started;
    bool stop_requested;
};

static unsigned clamp_commit_rate(unsigned per_hour) {
    if (per_hour == 0u)
        return (SL_TOSS_COMMIT_MIN_PER_HOUR + SL_TOSS_COMMIT_MAX_PER_HOUR) / 2u;
    if (per_hour < SL_TOSS_COMMIT_MIN_PER_HOUR) return SL_TOSS_COMMIT_MIN_PER_HOUR;
    if (per_hour > SL_TOSS_COMMIT_MAX_PER_HOUR) return SL_TOSS_COMMIT_MAX_PER_HOUR;
    return per_hour;
}

uint32_t sl_signal_toss_sample_interval_ms(void) {
    return 1000u / SL_TOSS_SAMPLE_HZ; /* ~22 ms => ~44 Hz */
}

uint32_t sl_signal_toss_commit_interval_ms(const sl_signal_toss *toss) {
    if (!toss) return 0u;
    return toss->commit_interval_ms;
}

sl_signal_toss *sl_signal_toss_create(
    sl_toss_sampler sampler, void *ctx, unsigned commit_per_hour) {
    sl_signal_toss *toss;
    if (!sampler) return NULL;
    toss = (sl_signal_toss *)calloc(1u, sizeof(*toss));
    if (!toss) return NULL;
    toss->sampler = sampler;
    toss->ctx = ctx;
    toss->commit_per_hour = clamp_commit_rate(commit_per_hour);
    /* 3600000 ms per hour / commits per hour. */
    toss->commit_interval_ms = (uint32_t)(3600000u / toss->commit_per_hour);
    sl_consistency_report_init(&toss->report);
    if (sl_mutex_init(&toss->lock) != 0) { free(toss); return NULL; }
    return toss;
}

bool sl_signal_toss_step(sl_signal_toss *toss, uint64_t now_ms) {
    sl_branch_view view;
    bool keep_going;
    if (!toss) return false;

    /* Zero the view so an incomplete sampler can't leak stack garbage. */
    view.trait_count = 0u;
    view.weight = SL_BRANCH_WEIGHT_MIN;
    view.studies_economy = false;
    view.economy_arity = 0u;
    view.left_branchpoints = 0u;
    view.right_branchpoints = 0u;
    view.refs = NULL;
    view.ref_count = 0u;

    keep_going = toss->sampler(&view, toss->ctx);

    sl_mutex_lock(&toss->lock);
    (void)sl_consistency_consider(&toss->report, &view);
    /* Commit a consistency update only when the throttle interval elapsed and
     * the branch just considered was consistent. */
    if (toss->report.last_code == SL_CONSISTENT) {
        if (toss->last_commit_ms == 0u ||
            now_ms - toss->last_commit_ms >= toss->commit_interval_ms) {
            ++toss->commits;
            toss->last_commit_ms = now_ms;
        }
    }
    sl_mutex_unlock(&toss->lock);
    return keep_going;
}

static int toss_worker(void *arg) {
    sl_signal_toss *toss = (sl_signal_toss *)arg;
    const uint32_t sample_ms = sl_signal_toss_sample_interval_ms();
    for (;;) {
        bool stop;
        sl_mutex_lock(&toss->lock);
        stop = toss->stop_requested;
        sl_mutex_unlock(&toss->lock);
        if (stop) break;

        if (!sl_signal_toss_step(toss, sl_monotonic_ms())) break;
        sl_sleep_ms(sample_ms);
    }
    sl_mutex_lock(&toss->lock);
    toss->running = false;
    sl_mutex_unlock(&toss->lock);
    return 0;
}

int sl_signal_toss_start(sl_signal_toss *toss) {
    if (!toss) return 1;
    sl_mutex_lock(&toss->lock);
    if (toss->running) { sl_mutex_unlock(&toss->lock); return 0; }
    toss->stop_requested = false;
    toss->running = true;
    sl_mutex_unlock(&toss->lock);

    if (sl_thread_start(&toss->thread, toss_worker, toss) != 0) {
        sl_mutex_lock(&toss->lock);
        toss->running = false;
        sl_mutex_unlock(&toss->lock);
        return 1;
    }
    toss->thread_started = true;
    return 0;
}

void sl_signal_toss_stop(sl_signal_toss *toss) {
    if (!toss) return;
    sl_mutex_lock(&toss->lock);
    toss->stop_requested = true;
    sl_mutex_unlock(&toss->lock);
    if (toss->thread_started) {
        (void)sl_thread_join(&toss->thread);
        toss->thread_started = false;
    }
}

void sl_signal_toss_destroy(sl_signal_toss *toss) {
    if (!toss) return;
    sl_signal_toss_stop(toss);
    sl_mutex_destroy(&toss->lock);
    free(toss);
}

sl_consistency_report sl_signal_toss_snapshot(sl_signal_toss *toss) {
    sl_consistency_report snap;
    if (!toss) { sl_consistency_report_init(&snap); return snap; }
    sl_mutex_lock(&toss->lock);
    snap = toss->report;
    sl_mutex_unlock(&toss->lock);
    return snap;
}

uint64_t sl_signal_toss_commits(sl_signal_toss *toss) {
    uint64_t c;
    if (!toss) return 0u;
    sl_mutex_lock(&toss->lock);
    c = toss->commits;
    sl_mutex_unlock(&toss->lock);
    return c;
}
