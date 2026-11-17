/*
 * reg_thread.c -- regression suite for impl/core/sleela_thread.c  (POSIX build)
 *
 * Contract (impl/core/sleela_thread.h):
 *   - every function returns 0 on success and a nonzero value on failure;
 *   - a NULL mutex, condition variable, handle or start routine is refused with
 *     a nonzero result, never dereferenced (the Windows branch always did this);
 *   - a mutex gives mutual exclusion; broadcast wakes every waiter;
 *   - the platform report is consistent with the build.
 *
 * Worker threads never use the REG_ macros.
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../impl/core/sleela_thread.h"
#include "regression.h"

#ifdef _WIN32
#include <stdio.h>
int main(void) { puts("SUITE thread: skipped (POSIX-only suite)"); return 0; }
#else
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static void t_platform(void) {
#if defined(__APPLE__)
    REG_EQ_I(SL_THREAD_MACOS, slthread_platform());
    REG_EQ_S("macos", slthread_platform_name());
    REG_EQ_I(0, slthread_platform_is_available(SL_THREAD_LINUX));
#else
    REG_EQ_I(SL_THREAD_LINUX, slthread_platform());
    REG_EQ_S("linux", slthread_platform_name());
    REG_EQ_I(0, slthread_platform_is_available(SL_THREAD_MACOS));
#endif
    REG_EQ_I(1, slthread_platform_is_available(SL_THREAD_AUTO));
    REG_EQ_I(1, slthread_platform_is_available(slthread_platform()));
    REG_EQ_I(0, slthread_platform_is_available(SL_THREAD_WINDOWS));
}

static void t_null_arguments_refused(void) {
    SLThreadMutex m;
    SLThreadCond c;
    REG_CHECK(slthread_mutex_init(NULL) != 0);
    REG_CHECK(slthread_mutex_destroy(NULL) != 0);
    REG_CHECK(slthread_mutex_lock(NULL) != 0);
    REG_CHECK(slthread_mutex_unlock(NULL) != 0);
    REG_CHECK(slthread_cond_init(NULL) != 0);
    REG_CHECK(slthread_cond_destroy(NULL) != 0);
    REG_CHECK(slthread_cond_broadcast(NULL) != 0);
    REG_CHECK(slthread_cond_wait(NULL, NULL) != 0);
    REG_EQ_I(0, slthread_mutex_init(&m));
    REG_EQ_I(0, slthread_cond_init(&c));
    REG_CHECK(slthread_cond_wait(&c, NULL) != 0);
    REG_CHECK(slthread_cond_wait(NULL, &m) != 0);
    REG_EQ_I(0, slthread_cond_destroy(&c));
    REG_EQ_I(0, slthread_mutex_destroy(&m));
    REG_CHECK(slthread_create(NULL, (SLThreadStart)0, NULL) != 0);
}

static void *noop(void *arg) { return arg; }

static void t_create_requires_handle_and_start(void) {
    SLThreadHandle h;
    REG_CHECK(slthread_create(NULL, noop, NULL) != 0);
    REG_CHECK(slthread_create(&h, NULL, NULL) != 0);
}

/* ---- mutual exclusion ------------------------------------------------------ */
#define THREADS 6
#define ROUNDS 20000

typedef struct { SLThreadMutex *m; long *counter; int rc; } Count;

static void *incrementer(void *arg) {
    Count *c = (Count *)arg;
    int i;
    for (i = 0; i < ROUNDS; ++i) {
        if (slthread_mutex_lock(c->m) != 0) { c->rc = 1; return NULL; }
        { long v = *c->counter; *c->counter = v + 1; }        /* a data race if the lock fails */
        if (slthread_mutex_unlock(c->m) != 0) { c->rc = 2; return NULL; }
    }
    return NULL;
}

static void t_mutual_exclusion(void) {
    SLThreadMutex m;
    SLThreadHandle th[THREADS];
    Count jobs[THREADS];
    long counter = 0;
    int i, created = 0;
    REG_EQ_I(0, slthread_mutex_init(&m));
    for (i = 0; i < THREADS; ++i) {
        jobs[i].m = &m; jobs[i].counter = &counter; jobs[i].rc = 0;
        if (slthread_create(&th[i], incrementer, &jobs[i]) == 0) ++created;
    }
    REG_EQ_I(THREADS, created);
    for (i = 0; i < created; ++i) REG_EQ_I(0, slthread_join(th[i]));
    for (i = 0; i < THREADS; ++i) REG_EQ_I(0, jobs[i].rc);
    REG_EQ_I((long long)THREADS * ROUNDS, counter);
    REG_EQ_I(0, slthread_mutex_destroy(&m));
}

/* ---- start routine arguments ---------------------------------------------- */
typedef struct { int in; int out; } Echo;

static void *doubler(void *arg) {
    Echo *e = (Echo *)arg;
    e->out = e->in * 2;
    return NULL;
}

static void t_argument_reaches_thread(void) {
    SLThreadHandle th;
    Echo e = { 21, 0 };
    REG_EQ_I(0, slthread_create(&th, doubler, &e));
    REG_EQ_I(0, slthread_join(th));
    REG_EQ_I(42, e.out);
}

/* ---- condition variable ---------------------------------------------------- */
typedef struct {
    SLThreadMutex m;
    SLThreadCond c;
    int open;
    int waiting;
    int woke;
} Gate;

static void *waiter(void *arg) {
    Gate *g = (Gate *)arg;
    slthread_mutex_lock(&g->m);
    g->waiting++;
    while (!g->open) slthread_cond_wait(&g->c, &g->m);
    g->woke++;
    slthread_mutex_unlock(&g->m);
    return NULL;
}

static void t_broadcast_wakes_everyone(void) {
    Gate g;
    SLThreadHandle th[THREADS];
    int i, created = 0, ready = 0, spins;
    memset(&g, 0, sizeof g);
    REG_REQUIRE(slthread_mutex_init(&g.m) == 0);
    REG_REQUIRE(slthread_cond_init(&g.c) == 0);
    for (i = 0; i < THREADS; ++i)
        if (slthread_create(&th[i], waiter, &g) == 0) ++created;
    REG_EQ_I(THREADS, created);

    /* wait (bounded) until every waiter is parked on the condition */
    for (spins = 0; spins < 4000 && !ready; ++spins) {
        slthread_mutex_lock(&g.m);
        ready = g.waiting == created;
        slthread_mutex_unlock(&g.m);
        if (!ready) {
            struct timespec ts = { 0, 1000000L };
            nanosleep(&ts, NULL);
        }
    }
    REG_CHECK(ready);

    slthread_mutex_lock(&g.m);
    REG_EQ_I(0, g.woke);                              /* nobody leaves before the gate opens */
    g.open = 1;
    REG_EQ_I(0, slthread_cond_broadcast(&g.c));
    slthread_mutex_unlock(&g.m);

    for (i = 0; i < created; ++i) REG_EQ_I(0, slthread_join(th[i]));
    REG_EQ_I(created, g.woke);
    REG_EQ_I(0, slthread_cond_destroy(&g.c));
    REG_EQ_I(0, slthread_mutex_destroy(&g.m));
}

int main(void) {
    alarm(30);                                         /* a lost wake-up must fail the run, not hang it */
    printf("thread regression suite\n");
    REG_RUN(t_platform);
    REG_RUN(t_null_arguments_refused);
    REG_RUN(t_create_requires_handle_and_start);
    REG_RUN(t_mutual_exclusion);
    REG_RUN(t_argument_reaches_thread);
    REG_RUN(t_broadcast_wakes_everyone);
    return reg_finish("thread");
}
#endif
