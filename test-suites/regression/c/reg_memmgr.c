/*
 * reg_memmgr.c -- regression suite for impl/core/sleela_memmgr.c
 *
 * Contract (impl/core/sleela_memmgr.h):
 *   - the byte limit is 256 MiB .. 2 GiB; 0 means the 2 GiB default; an invalid
 *     limit is refused and leaves the manager unchanged;
 *   - while enabled, every block is counted (live bytes, peak, live allocations,
 *     totals) and refused when it would push live bytes over the limit;
 *   - while disabled, blocks are still usable and freeable but are not counted,
 *     and freeing such a block later never disturbs the counters;
 *   - size arithmetic that overflows is refused with SLMM_OVERFLOW;
 *   - realloc keeps the old block valid when it fails, and a realloc whose
 *     RESULT fits under the limit must succeed;
 *   - payloads are 16-byte aligned; the manager is safe for concurrent use.
 *
 * Worker threads never use the REG_ macros.
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../impl/core/sleela_memmgr.h"
#include "regression.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define MIB (1024u * 1024u)

/* Put the manager in a known state: enabled, 256 MiB, counters zero. */
static void fresh(void) {
    slmm_disable();
    slmm_reset();
    (void)slmm_enable(SLMM_MIN_MEMORY_BYTES);
    slmm_reset();
}

static void settle(void) {
    slmm_reset();
    slmm_disable();
}

/* ---- limits --------------------------------------------------------------- */
static void t_limit_validation(void) {
    REG_EQ_I(0, slmm_valid_limit(0));
    REG_EQ_I(0, slmm_valid_limit(SLMM_MIN_MEMORY_BYTES - 1u));
    REG_EQ_I(1, slmm_valid_limit(SLMM_MIN_MEMORY_BYTES));
    REG_EQ_I(1, slmm_valid_limit(SLMM_MAX_MEMORY_BYTES));
    REG_EQ_I(0, slmm_valid_limit(SLMM_MAX_MEMORY_BYTES + 1u));
    REG_EQ_I(0, slmm_valid_limit((size_t)-1));
}

static void t_enable_and_defaults(void) {
    slmm_disable();
    REG_EQ_I(0, slmm_is_enabled());

    REG_EQ_I(-1, slmm_enable(1u * MIB));              /* below the minimum */
    REG_EQ_I(0, slmm_is_enabled());                   /* refusal changes nothing */
    REG_EQ_I(-1, slmm_enable(SLMM_MAX_MEMORY_BYTES + 1u));
    REG_EQ_I(0, slmm_is_enabled());

    REG_EQ_I(0, slmm_enable(0));                      /* 0 selects the default */
    REG_EQ_I(1, slmm_is_enabled());
    REG_EQ_U(SLMM_DEFAULT_MEMORY_BYTES, slmm_limit());

    REG_EQ_I(0, slmm_enable(SLMM_MIN_MEMORY_BYTES));
    REG_EQ_U(SLMM_MIN_MEMORY_BYTES, slmm_limit());

    slmm_set_limit(1u * MIB);                         /* invalid: ignored */
    REG_EQ_U(SLMM_MIN_MEMORY_BYTES, slmm_limit());
    slmm_set_limit(512u * MIB);
    REG_EQ_U(512u * MIB, slmm_limit());
    slmm_set_limit(0);                                /* default again */
    REG_EQ_U(SLMM_DEFAULT_MEMORY_BYTES, slmm_limit());

    slmm_disable();
    REG_EQ_I(0, slmm_is_enabled());
}

/* ---- accounting ----------------------------------------------------------- */
static void t_accounting(void) {
    SLMMStats st;
    void *a, *b;
    fresh();
    a = slmm_alloc(1000);
    b = slmm_alloc(24);
    REG_REQUIRE(a && b);
    REG_EQ_U(0, (uintptr_t)a % 16u);                  /* payload alignment */
    REG_EQ_U(0, (uintptr_t)b % 16u);
    slmm_stats(&st);
    REG_EQ_U(1024, st.live_bytes);
    REG_EQ_U(1024, st.peak_bytes);
    REG_EQ_U(2, st.live_allocs);
    REG_EQ_U(2, st.total_allocs);
    REG_EQ_U(0, st.total_frees);
    REG_EQ_U(0, st.refused);
    REG_EQ_I(SLMM_OK, st.last_status);
    REG_EQ_I(1, st.enabled);

    slmm_free(a);
    slmm_stats(&st);
    REG_EQ_U(24, st.live_bytes);
    REG_EQ_U(1024, st.peak_bytes);                    /* peak never goes down */
    REG_EQ_U(1, st.live_allocs);
    REG_EQ_U(1, st.total_frees);
    slmm_free(b);
    slmm_stats(&st);
    REG_EQ_U(0, st.live_bytes);
    REG_EQ_U(0, st.live_allocs);
    REG_EQ_U(2, st.total_frees);
    settle();
}

static void t_zero_and_null(void) {
    SLMMStats st;
    void *p;
    fresh();
    slmm_free(NULL);                                  /* no-op */
    slmm_stats(NULL);                                 /* no-op, no crash */
    p = slmm_alloc(0);
    REG_CHECK(p != NULL);                             /* a distinct, freeable block */
    slmm_free(p);
    slmm_stats(&st);
    REG_EQ_U(0, st.live_bytes);
    REG_EQ_U(0, st.live_allocs);
    REG_EQ_U(1, st.total_allocs);
    REG_EQ_U(1, st.total_frees);
    settle();
}

static void t_calloc(void) {
    unsigned char *p;
    size_t i, bad = 0;
    fresh();
    p = (unsigned char *)slmm_calloc(10, 100);
    REG_REQUIRE(p != NULL);
    for (i = 0; i < 1000; ++i) bad += p[i] != 0;
    REG_EQ_U(0, bad);                                 /* really zeroed */
    slmm_free(p);

    {
        void *z = slmm_calloc(0, 100);                /* either answer is legal; must not crash */
        slmm_free(z);
    }
    settle();
}

/* ---- refusals ------------------------------------------------------------- */
static void t_overflow_refused(void) {
    SLMMStats st;
    fresh();
    REG_CHECK(slmm_alloc((size_t)-1) == NULL);
    REG_EQ_I(SLMM_OVERFLOW, slmm_last_status());
    REG_CHECK(slmm_alloc((size_t)-1 - 31u) == NULL);  /* header span pushes it over */
    REG_EQ_I(SLMM_OVERFLOW, slmm_last_status());
    REG_CHECK(slmm_calloc((size_t)1 << 40, (size_t)1 << 40) == NULL);
    REG_EQ_I(SLMM_OVERFLOW, slmm_last_status());
    REG_CHECK(slmm_calloc((size_t)-1, 2) == NULL);
    REG_EQ_I(SLMM_OVERFLOW, slmm_last_status());
    slmm_stats(&st);
    REG_EQ_U(4, st.refused);
    REG_EQ_U(0, st.live_allocs);
    REG_EQ_U(0, st.total_allocs);
    settle();
}

static void t_limit_refused(void) {
    SLMMStats st;
    void *a, *b, *c;
    fresh();                                          /* 256 MiB */
    REG_CHECK(slmm_alloc(SLMM_MIN_MEMORY_BYTES + 1u) == NULL);
    REG_EQ_I(SLMM_LIMIT, slmm_last_status());

    a = slmm_alloc(100u * MIB);
    b = slmm_alloc(100u * MIB);
    REG_REQUIRE(a && b);
    c = slmm_alloc(100u * MIB);                       /* 300 MiB live would exceed 256 */
    REG_CHECK(c == NULL);
    REG_EQ_I(SLMM_LIMIT, slmm_last_status());
    c = slmm_alloc(56u * MIB);                        /* exactly the limit: allowed */
    REG_CHECK(c != NULL);
    REG_CHECK(slmm_alloc(1) == NULL);                 /* and one more byte is not */
    slmm_stats(&st);
    REG_EQ_U(SLMM_MIN_MEMORY_BYTES, st.live_bytes);
    REG_EQ_U(SLMM_MIN_MEMORY_BYTES, st.peak_bytes);
    REG_EQ_U(3, st.refused);
    slmm_free(a); slmm_free(b); slmm_free(c);
    a = slmm_alloc(100u * MIB);                       /* everything was returned: room again */
    REG_CHECK(a != NULL);
    slmm_free(a);
    settle();
}

/* Releasing memory makes room again. */
static void t_limit_recovers_after_free(void) {
    void *a, *b;
    fresh();
    a = slmm_alloc(200u * MIB);
    REG_REQUIRE(a != NULL);
    REG_CHECK(slmm_alloc(100u * MIB) == NULL);
    slmm_free(a);
    b = slmm_alloc(100u * MIB);
    REG_CHECK(b != NULL);
    slmm_free(b);
    settle();
}

/* ---- disabled manager ----------------------------------------------------- */
static void t_disabled_does_not_count(void) {
    SLMMStats st;
    void *p;
    slmm_disable();
    slmm_reset();
    p = slmm_alloc(5000);
    REG_REQUIRE(p != NULL);
    REG_EQ_I(SLMM_DISABLED, slmm_last_status());
    memset(p, 0x5A, 5000);                            /* usable */
    slmm_stats(&st);
    REG_EQ_I(0, st.enabled);
    REG_EQ_U(0, st.live_bytes);
    REG_EQ_U(0, st.total_allocs);

    /* A block taken while disabled and released after enabling must not
     * subtract from counters it never contributed to. */
    (void)slmm_enable(SLMM_MIN_MEMORY_BYTES);
    slmm_reset();
    {
        void *q = slmm_alloc(100);
        REG_REQUIRE(q != NULL);
        slmm_free(p);
        slmm_stats(&st);
        REG_EQ_U(100, st.live_bytes);
        REG_EQ_U(1, st.live_allocs);
        REG_EQ_U(0, st.total_frees);
        slmm_free(q);
    }
    settle();
}

/* A block taken while enabled and released after disabling still balances. */
static void t_enabled_block_freed_after_disable(void) {
    SLMMStats st;
    void *p;
    fresh();
    p = slmm_alloc(300);
    REG_REQUIRE(p != NULL);
    slmm_disable();
    slmm_free(p);
    slmm_stats(&st);
    REG_EQ_U(0, st.live_bytes);
    REG_EQ_U(0, st.live_allocs);
    REG_EQ_U(1, st.total_frees);
    settle();
}

/* ---- realloc -------------------------------------------------------------- */
static void t_realloc_basic(void) {
    unsigned char *p, *q;
    SLMMStats st;
    size_t i, bad = 0;
    fresh();
    p = (unsigned char *)slmm_realloc(NULL, 64);      /* NULL pointer == alloc */
    REG_REQUIRE(p != NULL);
    for (i = 0; i < 64; ++i) p[i] = (unsigned char)i;

    q = (unsigned char *)slmm_realloc(p, 4096);       /* grow: prefix preserved */
    REG_REQUIRE(q != NULL);
    for (i = 0; i < 64; ++i) bad += q[i] != (unsigned char)i;
    REG_EQ_U(0, bad);
    slmm_stats(&st);
    REG_EQ_U(4096, st.live_bytes);
    REG_EQ_U(1, st.live_allocs);

    p = (unsigned char *)slmm_realloc(q, 16);         /* shrink: prefix preserved */
    REG_REQUIRE(p != NULL);
    for (i = 0, bad = 0; i < 16; ++i) bad += p[i] != (unsigned char)i;
    REG_EQ_U(0, bad);
    slmm_stats(&st);
    REG_EQ_U(16, st.live_bytes);

    REG_CHECK(slmm_realloc(p, 0) == NULL);            /* size 0 frees */
    slmm_stats(&st);
    REG_EQ_U(0, st.live_bytes);
    REG_EQ_U(0, st.live_allocs);
    settle();
}

static void t_realloc_failure_keeps_old_block(void) {
    unsigned char *p;
    SLMMStats st;
    fresh();
    p = (unsigned char *)slmm_alloc(128);
    REG_REQUIRE(p != NULL);
    memset(p, 0x77, 128);
    REG_CHECK(slmm_realloc(p, SLMM_MIN_MEMORY_BYTES + 1u) == NULL);
    REG_EQ_I(SLMM_LIMIT, slmm_last_status());
    REG_EQ_I(0x77, p[0]);                             /* old block untouched ... */
    REG_EQ_I(0x77, p[127]);
    slmm_stats(&st);
    REG_EQ_U(128, st.live_bytes);                     /* ... and still counted */
    slmm_free(p);
    settle();
}

/* Regression (was MEMMGR-REALLOC-HEADROOM): growing a block whose RESULT fits
 * under the limit succeeds even when old + new together would not. */
static void t_realloc_needs_no_double_headroom(void) {
    unsigned char *p, *q;
    SLMMStats st;
    fresh();                                          /* 256 MiB limit */
    p = (unsigned char *)slmm_alloc(129u * MIB);
    REG_REQUIRE(p != NULL);
    p[0] = 0x42;
    q = (unsigned char *)slmm_realloc(p, 130u * MIB); /* 130 MiB fits; 259 MiB would not */
    if (!q) {                                         /* the old block must still be valid */
        REG_EQ_I(0x42, p[0]);
        REG_CHECK(q != NULL);
        slmm_free(p);
        settle();
        return;
    }
    REG_EQ_I(0x42, q[0]);
    slmm_stats(&st);
    REG_EQ_U(130u * MIB, st.live_bytes);
    REG_EQ_U(1, st.live_allocs);
    slmm_free(q);
    settle();
}

/* ---- reset ---------------------------------------------------------------- */
static void t_reset(void) {
    SLMMStats st;
    void *p;
    fresh();
    p = slmm_alloc(500);
    REG_REQUIRE(p != NULL);
    REG_CHECK(slmm_alloc(SLMM_MAX_MEMORY_BYTES) == NULL);   /* a refusal to clear */
    slmm_reset();                                     /* with a live block: invariants kept */
    slmm_stats(&st);
    REG_EQ_U(500, st.live_bytes);
    REG_EQ_U(1, st.live_allocs);
    REG_EQ_U(500, st.peak_bytes);                     /* peak restarts at live */
    REG_EQ_U(0, st.total_allocs);
    REG_EQ_U(0, st.refused);
    REG_EQ_I(1, st.enabled);                          /* reset keeps enabled + limit */
    REG_EQ_U(SLMM_MIN_MEMORY_BYTES, st.limit_bytes);
    slmm_free(p);
    slmm_stats(&st);
    REG_EQ_U(0, st.live_bytes);                       /* and the books still balance */
    settle();
}

/* ---- status names and report --------------------------------------------- */
static void t_status_names(void) {
    REG_EQ_S("ok", slmm_status_name(SLMM_OK));
    REG_EQ_S("disabled", slmm_status_name(SLMM_DISABLED));
    REG_EQ_S("limit-exceeded", slmm_status_name(SLMM_LIMIT));
    REG_EQ_S("out-of-memory", slmm_status_name(SLMM_OOM));
    REG_EQ_S("size-overflow", slmm_status_name(SLMM_OVERFLOW));
    REG_EQ_S("unknown", slmm_status_name((SLMMStatus)99));
}

static void t_report(void) {
    char buf[1024], tiny[16];
    int n;
    fresh();
    n = slmm_format_report(buf, sizeof buf);
    REG_CHECK(n > 0 && (size_t)n < sizeof buf);
    REG_CHECK(strstr(buf, "enabled=yes") != NULL);
    REG_CHECK(strstr(buf, "limit=268,435,456 bytes") != NULL);   /* thousands grouping */
    REG_CHECK(strstr(buf, "live=0 bytes") != NULL);
    REG_CHECK(strstr(buf, "last=ok") != NULL);

    memset(tiny, 'x', sizeof tiny);
    n = slmm_format_report(tiny, sizeof tiny);        /* snprintf semantics: full length reported */
    REG_CHECK(n >= (int)sizeof tiny);
    REG_EQ_I(0, tiny[sizeof tiny - 1]);               /* but never an unterminated buffer */
    settle();
}

/* ---- concurrency ---------------------------------------------------------- */
#include <pthread.h>

#define THREADS 6
#define ROUNDS 4000

typedef struct { int done; int bad; } Job;

static void *worker(void *arg) {
    Job *j = (Job *)arg;
    void *held[8] = {0};
    int i;
    for (i = 0; i < ROUNDS; ++i) {
        int slot = i & 7;
        if (held[slot]) {
            slmm_free(held[slot]);
            held[slot] = NULL;
        } else {
            held[slot] = slmm_alloc((size_t)(16 + (i % 200)));
            if (!held[slot]) j->bad++;
        }
    }
    for (i = 0; i < 8; ++i) slmm_free(held[i]);
    j->done = 1;
    return NULL;
}

static void t_concurrent_balance(void) {
    pthread_t th[THREADS];
    Job jobs[THREADS];
    SLMMStats st;
    int i, started = 0;
    fresh();
    memset(jobs, 0, sizeof jobs);
    for (i = 0; i < THREADS; ++i)
        if (pthread_create(&th[i], NULL, worker, &jobs[i]) == 0) ++started;
    REG_EQ_I(THREADS, started);
    for (i = 0; i < started; ++i) pthread_join(th[i], NULL);
    for (i = 0; i < THREADS; ++i) { REG_EQ_I(1, jobs[i].done); REG_EQ_I(0, jobs[i].bad); }
    slmm_stats(&st);
    REG_EQ_U(0, st.live_bytes);                       /* every thread's blocks came back */
    REG_EQ_U(0, st.live_allocs);
    REG_EQ_U(st.total_allocs, st.total_frees);
    REG_CHECK(st.total_allocs >= (uint64_t)THREADS * (ROUNDS / 2 - 8));
    settle();
}

int main(void) {
    reg_seed(1);
    printf("memmgr regression suite\n");
    REG_RUN(t_limit_validation);
    REG_RUN(t_enable_and_defaults);
    REG_RUN(t_accounting);
    REG_RUN(t_zero_and_null);
    REG_RUN(t_calloc);
    REG_RUN(t_overflow_refused);
    REG_RUN(t_limit_refused);
    REG_RUN(t_limit_recovers_after_free);
    REG_RUN(t_disabled_does_not_count);
    REG_RUN(t_enabled_block_freed_after_disable);
    REG_RUN(t_realloc_basic);
    REG_RUN(t_realloc_failure_keeps_old_block);
    REG_RUN(t_realloc_needs_no_double_headroom);
    REG_RUN(t_reset);
    REG_RUN(t_status_names);
    REG_RUN(t_report);
    REG_RUN(t_concurrent_balance);
    return reg_finish("memmgr");
}
