/*
 * reg_events.c -- regression suite for impl/core/sleela_events.c  (POSIX build)
 *
 * Contract (impl/core/sleela_events.h):
 *   - manual-reset events: set wakes EVERY waiter and stays set until reset;
 *     wait(handle, ms) returns 0 when set, ETIMEDOUT after ms, EINVAL for an
 *     invalid handle; UINT32_MAX waits forever; wait(0) only polls;
 *   - the runtime queue is a bounded (64) FIFO: post -> 0 / EAGAIN when full,
 *     poll -> 1 and the oldest event, 0 when empty, EINVAL for NULL;
 *     it is safe for concurrent producers and a consumer;
 *   - signals: install / pending / clear report EINVAL (0 for pending) for
 *     out-of-range numbers.
 *
 * Threads never use the REG_ macros or allocate: their results are stored in
 * plain structs and checked by the main thread.
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../impl/core/sleela_events.h"
#include "regression.h"

#ifdef _WIN32
int main(void) { puts("SUITE events: skipped (POSIX-only suite)"); return 0; }
#else
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static uint64_t now_ms(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}
static void sleep_ms(unsigned ms) {
    struct timespec ts; ts.tv_sec = (time_t)(ms / 1000u); ts.tv_nsec = (long)(ms % 1000u) * 1000000L;
    nanosleep(&ts, NULL);
}

static void drain_queue(void) { SLEvent e; while (slevent_poll(&e) == 1) {} }

/* ---- platform ------------------------------------------------------------ */
static void t_platform(void) {
#if defined(__APPLE__)
    REG_EQ_I(SL_EVENT_MACOS, slevent_platform());
#else
    REG_EQ_I(SL_EVENT_LINUX, slevent_platform()); REG_EQ_S("linux-signals-events", slevent_platform_name());
#endif
    REG_EQ_I(1, slevent_platform_is_available(SL_EVENT_AUTO));
    REG_EQ_I(1, slevent_platform_is_available(slevent_platform()));
    REG_EQ_I(0, slevent_platform_is_available(SL_EVENT_WINDOWS));
}

/* ---- handles ------------------------------------------------------------- */
static void t_handle_state_machine(void) {
    SLEventHandle h = slevent_create(0), s = slevent_create(1);
    REG_CHECK(slevent_valid(h)); REG_CHECK(slevent_valid(s));
    REG_EQ_I(ETIMEDOUT, slevent_wait(h, 0));            /* initially clear */
    REG_EQ_I(0, slevent_wait(s, 0));                    /* initially set   */
    REG_EQ_I(0, slevent_set(h));
    REG_EQ_I(0, slevent_wait(h, 0));
    REG_EQ_I(0, slevent_wait(h, 0));                    /* manual reset: stays set */
    REG_EQ_I(0, slevent_set(h));                        /* idempotent */
    REG_EQ_I(0, slevent_reset(h));
    REG_EQ_I(ETIMEDOUT, slevent_wait(h, 0));
    REG_EQ_I(0, slevent_reset(h));                      /* idempotent */
    REG_EQ_I(0, slevent_reset(s));
    REG_EQ_I(ETIMEDOUT, slevent_wait(s, 0));
    REG_EQ_I(0, slevent_close(h)); REG_EQ_I(0, slevent_close(s));
}

static void t_invalid_handles_are_rejected_not_dereferenced(void) {
    const SLEventHandle bad[] = {SL_EVENT_INVALID, 0};
    REG_CHECK(!slevent_valid(SL_EVENT_INVALID)); REG_CHECK(!slevent_valid(0));
    for (size_t i = 0; i < sizeof bad / sizeof bad[0]; ++i) {
        REG_EQ_I(EINVAL, slevent_set(bad[i]));
        REG_EQ_I(EINVAL, slevent_reset(bad[i]));
        REG_EQ_I(EINVAL, slevent_wait(bad[i], 0));
        REG_EQ_I(EINVAL, slevent_wait(bad[i], 10));
        REG_EQ_I(EINVAL, slevent_close(bad[i]));
    }
}

static void t_timeout_waits_at_least_the_requested_time(void) {
    SLEventHandle h = slevent_create(0);
    uint64_t t0 = now_ms(), t1;
    REG_EQ_I(ETIMEDOUT, slevent_wait(h, 60));
    t1 = now_ms();
    REG_CHECK(t1 - t0 >= 59);                           /* did not return early (1 ms clock slack) */
    REG_CHECK(t1 - t0 < 2000);                          /* did not hang */
    t0 = now_ms();
    REG_EQ_I(ETIMEDOUT, slevent_wait(h, 0));
    REG_CHECK(now_ms() - t0 < 50);                      /* a zero timeout only polls */
    REG_EQ_I(0, slevent_close(h));
}

typedef struct { SLEventHandle h; uint32_t timeout; int rc; uint64_t waited_ms; } Waiter;

static void *waiter_thread(void *arg) {
    Waiter *w = (Waiter *)arg; uint64_t t0 = now_ms();
    w->rc = slevent_wait(w->h, w->timeout);
    w->waited_ms = now_ms() - t0;
    return NULL;
}

static void t_set_from_another_thread_wakes_the_waiter(void) {
    SLEventHandle h = slevent_create(0); Waiter w; pthread_t th;
    w.h = h; w.timeout = 5000; w.rc = -1; w.waited_ms = 0;
    REG_REQUIRE(pthread_create(&th, NULL, waiter_thread, &w) == 0);
    sleep_ms(80);
    REG_EQ_I(0, slevent_set(h));
    pthread_join(th, NULL);
    REG_EQ_I(0, w.rc);                                  /* woken by the set, not by the timeout */
    REG_CHECK(w.waited_ms >= 60 && w.waited_ms < 3000);
    REG_EQ_I(0, slevent_close(h));
}

static void t_set_wakes_every_waiter_and_infinite_wait_works(void) {
    enum { N = 6 };
    SLEventHandle h = slevent_create(0); Waiter w[N]; pthread_t th[N];
    for (int i = 0; i < N; ++i) {
        w[i].h = h; w[i].timeout = (i % 2) ? UINT32_MAX : 5000; w[i].rc = -1; w[i].waited_ms = 0;
        REG_REQUIRE(pthread_create(&th[i], NULL, waiter_thread, &w[i]) == 0);
    }
    sleep_ms(100);
    REG_EQ_I(0, slevent_set(h));                        /* manual reset: one set releases all of them */
    for (int i = 0; i < N; ++i) pthread_join(th[i], NULL);
    for (int i = 0; i < N; ++i) { REG_EQ_I(0, w[i].rc); REG_CHECK(w[i].waited_ms < 3000); }
    REG_EQ_I(0, slevent_close(h));
}

static void t_wait_on_an_already_set_event_returns_at_once(void) {
    SLEventHandle h = slevent_create(1); uint64_t t0 = now_ms();
    REG_EQ_I(0, slevent_wait(h, 5000)); REG_EQ_I(0, slevent_wait(h, UINT32_MAX));
    REG_CHECK(now_ms() - t0 < 200);
    REG_EQ_I(0, slevent_close(h));
}

static void t_reset_after_set_makes_waiters_block_again(void) {
    SLEventHandle h = slevent_create(0);
    REG_EQ_I(0, slevent_set(h)); REG_EQ_I(0, slevent_reset(h));
    REG_EQ_I(ETIMEDOUT, slevent_wait(h, 30));
    REG_EQ_I(0, slevent_close(h));
}

static void t_handles_do_not_leak(void) {
    size_t before = reg_alloc_live();
    for (int i = 0; i < 200; ++i) {
        SLEventHandle h = slevent_create(i & 1);
        REG_REQUIRE(slevent_valid(h));
        REG_EQ_I(0, slevent_close(h));
    }
    REG_EQ_U(before, reg_alloc_live());
    reg_alloc_fail_nth(1);
    REG_EQ_I(SL_EVENT_INVALID, slevent_create(0));       /* allocation failure is reported, not crashed on */
    reg_alloc_fail_nth(0);
}

/* ---- runtime queue ------------------------------------------------------- */
static void t_queue_is_fifo_and_bounded(void) {
    SLEvent e; drain_queue();
    REG_EQ_I(0, slevent_poll(&e));                       /* empty */
    REG_EQ_I(EINVAL, slevent_poll(NULL));
    for (uint32_t i = 0; i < 64; ++i) REG_EQ_I(0, slevent_post(i, i * 2u, (intptr_t)i - 5));
    REG_EQ_I(EAGAIN, slevent_post(999, 0, 0));           /* full: capacity 64 */
    for (uint32_t i = 0; i < 64; ++i) {
        REG_EQ_I(1, slevent_poll(&e));
        REG_EQ_U(i, e.type); REG_EQ_U(i * 2u, e.code); REG_EQ_I((long long)i - 5, e.value);
    }
    REG_EQ_I(0, slevent_poll(&e));
    REG_EQ_I(0, slevent_post(7, 8, 9));                  /* usable again after draining */
    REG_EQ_I(1, slevent_poll(&e)); REG_EQ_U(7, e.type);
}

static void t_queue_wraps_around_many_times(void) {
    SLEvent e; uint32_t next_post = 0, next_poll = 0; drain_queue();
    reg_seed(0xEE1ull);
    for (int step = 0; step < 20000; ++step) {
        if (reg_below(2)) {
            if (slevent_post(next_post, 0, 0) == 0) ++next_post;
        } else if (slevent_poll(&e) == 1) {
            if (e.type != next_poll) { REG_CHECK(!"queue order violated"); return; }
            ++next_poll;
        }
    }
    while (slevent_poll(&e) == 1) { REG_EQ_U(next_poll, e.type); ++next_poll; }
    REG_EQ_U(next_post, next_poll);                      /* nothing lost, nothing duplicated */
}

#define PRODUCERS 3
#define PER_PRODUCER 4000
typedef struct { uint32_t id; uint32_t posted; } Producer;

static void *producer_thread(void *arg) {
    Producer *p = (Producer *)arg;
    for (uint32_t i = 0; i < PER_PRODUCER; ++i) {
        while (slevent_post(p->id, i, 0) != 0) sched_yield();   /* full: retry */
        ++p->posted;
    }
    return NULL;
}

static void t_concurrent_producers_single_consumer_lose_nothing(void) {
    pthread_t th[PRODUCERS]; Producer pr[PRODUCERS]; SLEvent e;
    uint32_t last[PRODUCERS]; long received[PRODUCERS] = {0}; long total = 0; int order_ok = 1;
    drain_queue();
    for (uint32_t i = 0; i < PRODUCERS; ++i) { pr[i].id = i; pr[i].posted = 0; last[i] = 0; }
    for (int i = 0; i < PRODUCERS; ++i) REG_REQUIRE(pthread_create(&th[i], NULL, producer_thread, &pr[i]) == 0);
    {   uint64_t deadline = now_ms() + 20000;
        while (total < (long)PRODUCERS * PER_PRODUCER && now_ms() < deadline) {
            if (slevent_poll(&e) == 1) {
                if (e.type >= PRODUCERS) { order_ok = 0; continue; }
                if (received[e.type] > 0 && e.code != last[e.type] + 1u) order_ok = 0;   /* per-producer FIFO */
                if (received[e.type] == 0 && e.code != 0) order_ok = 0;
                last[e.type] = e.code; ++received[e.type]; ++total;
            } else sched_yield();
        }
    }
    for (int i = 0; i < PRODUCERS; ++i) pthread_join(th[i], NULL);
    REG_EQ_I((long)PRODUCERS * PER_PRODUCER, total);
    for (int i = 0; i < PRODUCERS; ++i) REG_EQ_I(PER_PRODUCER, received[i]);
    REG_CHECK(order_ok);
    REG_EQ_I(0, slevent_poll(&e));
}

/* ---- signals ------------------------------------------------------------- */
static void t_signal_flags(void) {
#ifdef SIGUSR2
    REG_EQ_I(0, slevent_install_signal(SIGUSR2));
    REG_EQ_I(0, slevent_signal_pending(SIGUSR2));
    raise(SIGUSR2);
    REG_EQ_I(1, slevent_signal_pending(SIGUSR2));
    REG_EQ_I(1, slevent_signal_pending(SIGUSR2));         /* reading does not clear */
    REG_EQ_I(0, slevent_clear_signal(SIGUSR2));
    REG_EQ_I(0, slevent_signal_pending(SIGUSR2));
    raise(SIGUSR2); raise(SIGUSR2);
    REG_EQ_I(1, slevent_signal_pending(SIGUSR2));
    slevent_clear_signal(SIGUSR2);
#endif
    REG_EQ_I(EINVAL, slevent_install_signal(0)); REG_EQ_I(EINVAL, slevent_install_signal(-1));
    REG_EQ_I(EINVAL, slevent_install_signal(64)); REG_EQ_I(EINVAL, slevent_install_signal(100000));
    REG_EQ_I(0, slevent_signal_pending(0)); REG_EQ_I(0, slevent_signal_pending(-3)); REG_EQ_I(0, slevent_signal_pending(64));
    REG_EQ_I(EINVAL, slevent_clear_signal(0)); REG_EQ_I(EINVAL, slevent_clear_signal(64)); REG_EQ_I(EINVAL, slevent_clear_signal(-1));
    REG_EQ_I(0, slevent_signal_pending(SIGTERM));          /* nothing installed or raised */
}

int main(void) {
    REG_RUN(t_platform);
    REG_RUN(t_handle_state_machine);
    REG_RUN(t_invalid_handles_are_rejected_not_dereferenced);
    REG_RUN(t_timeout_waits_at_least_the_requested_time);
    REG_RUN(t_set_from_another_thread_wakes_the_waiter);
    REG_RUN(t_set_wakes_every_waiter_and_infinite_wait_works);
    REG_RUN(t_wait_on_an_already_set_event_returns_at_once);
    REG_RUN(t_reset_after_set_makes_waiters_block_again);
    REG_RUN(t_handles_do_not_leak);
    REG_RUN(t_queue_is_fifo_and_bounded);
    REG_RUN(t_queue_wraps_around_many_times);
    REG_RUN(t_concurrent_producers_single_consumer_lose_nothing);
    REG_RUN(t_signal_flags);
    return reg_finish("events");
}
#endif
