/*
 * reg_support.c -- implementation of regression.h: harness, PRNG and the
 * instrumented allocator.  Linked into every C and C++ regression binary with
 *     -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free
 * so every allocation made by the code under test passes through here.
 *
 * Blocks are laid out as  [16 guard][user bytes][16 guard].  Pointers that were
 * not produced by this wrapper (for example posix_memalign results released
 * through free) are passed straight to the real allocator.
 */
#include "regression.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

void *__real_malloc(size_t);
void *__real_calloc(size_t, size_t);
void *__real_realloc(void *, size_t);
void __real_free(void *);

#define GUARD 16u
#define TABLE_BITS 17u
#define TABLE_SIZE (1u << TABLE_BITS)
#define TOMBSTONE ((void *)1)

typedef struct { void *user; size_t size; } Rec;
static Rec g_table[TABLE_SIZE];
static size_t g_live;
static unsigned long g_calls, g_corruptions, g_arm, g_since;
static int g_injected;

static unsigned slot_of(const void *p) {
    return (unsigned)((((uint64_t)(uintptr_t)p >> 4) * 11400714819323198485ull) >>
                      (64u - TABLE_BITS));
}

static Rec *find(const void *p) {
    unsigned i = slot_of(p);
    for (unsigned n = 0; n < TABLE_SIZE; ++n, i = (i + 1u) & (TABLE_SIZE - 1u)) {
        if (g_table[i].user == NULL) return NULL;
        if (g_table[i].user == p) return &g_table[i];
    }
    return NULL;
}

static void insert(void *p, size_t size) {
    unsigned i = slot_of(p);
    for (unsigned n = 0; n < TABLE_SIZE; ++n, i = (i + 1u) & (TABLE_SIZE - 1u)) {
        if (g_table[i].user == NULL || g_table[i].user == TOMBSTONE) {
            g_table[i].user = p; g_table[i].size = size; ++g_live; return;
        }
    }
    fputs("reg_support: allocation table full\n", stderr);
    abort();
}

/* Guard bytes depend on the block address and the offset, so copying the
 * guard of one block over the guard of another (a classic over-read + over-write
 * pair) is still detected. */
static unsigned char canary(const unsigned char *user, unsigned slot) {
    return (unsigned char)(0xA5u ^ (unsigned)(((uintptr_t)user >> 4) & 0xFFu) ^ (slot * 37u));
}

static void paint_guards(unsigned char *user, size_t size) {
    for (unsigned i = 1; i <= GUARD; ++i) user[-(long)i] = canary(user, i);
    for (unsigned i = 0; i < GUARD; ++i) user[size + i] = canary(user, GUARD + 1u + i);
}

static int guards_intact(const Rec *r) {
    const unsigned char *u = (const unsigned char *)r->user;
    for (unsigned i = 1; i <= GUARD; ++i)
        if (u[-(long)i] != canary(u, i)) return 0;
    for (unsigned i = 0; i < GUARD; ++i)
        if (u[r->size + i] != canary(u, GUARD + 1u + i)) return 0;
    return 1;
}

static void check_guards(Rec *r, const char *what) {
    if (!guards_intact(r)) {
        ++g_corruptions;
        fprintf(stderr, "reg_support: heap guard violated (%s) on %zu-byte block\n",
                what, r->size);
    }
}

static int should_fail(void) {
    ++g_calls;
    if (g_arm != 0 && ++g_since == g_arm) {
        g_arm = 0; g_since = 0; g_injected = 1; return 1;
    }
    return 0;
}

static void *tracked_alloc(size_t size) {
    unsigned char *base;
    if (should_fail()) { errno = ENOMEM; return NULL; }
    if (size > (size_t)-1 - 2u * GUARD) { errno = ENOMEM; return NULL; }
    base = (unsigned char *)__real_malloc(size + 2u * GUARD);
    if (!base) return NULL;
    memset(base + GUARD, 0xCD, size);   /* fresh memory is junk: uninitialised reads become visible */
    paint_guards(base + GUARD, size);
    insert(base + GUARD, size);
    return base + GUARD;
}

static void tracked_free(void *p) {
    Rec *r;
    if (!p) return;
    r = find(p);
    if (!r) { __real_free(p); return; }
    check_guards(r, "free");
    memset(p, 0xDD, r->size); /* poison: stale reads become visible */
    __real_free((unsigned char *)p - GUARD);
    r->user = TOMBSTONE; r->size = 0; --g_live;
}

/* One lock serialises the table, so suites that run worker threads (memmgr,
 * events) can allocate safely.  The public entry points take it; the helpers
 * above and below do not, so realloc can call them without re-entering. */
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

void *__wrap_malloc(size_t size) {
    void *p;
    pthread_mutex_lock(&g_lock);
    p = tracked_alloc(size);
    pthread_mutex_unlock(&g_lock);
    return p;
}

void *__wrap_calloc(size_t n, size_t size) {
    void *p;
    if (size != 0 && n > (size_t)-1 / size) { errno = ENOMEM; return NULL; }
    pthread_mutex_lock(&g_lock);
    p = tracked_alloc(n * size);
    if (p) memset(p, 0, n * size);
    pthread_mutex_unlock(&g_lock);
    return p;
}

static void *realloc_locked(void *p, size_t size);

void *__wrap_realloc(void *p, size_t size) {
    void *q;
    pthread_mutex_lock(&g_lock);
    q = realloc_locked(p, size);
    pthread_mutex_unlock(&g_lock);
    return q;
}

static void *realloc_locked(void *p, size_t size) {
    Rec *r; void *q; size_t keep;
    if (!p) return tracked_alloc(size);
    r = find(p);
    if (!r) return __real_realloc(p, size);
    if (size == 0) { tracked_free(p); return NULL; }
    check_guards(r, "realloc");
    q = tracked_alloc(size);
    if (!q) return NULL; /* original block stays valid, as with real realloc */
    keep = r->size < size ? r->size : size;
    memcpy(q, p, keep);
    tracked_free(p);
    return q;
}

void __wrap_free(void *p) {
    pthread_mutex_lock(&g_lock);
    tracked_free(p);
    pthread_mutex_unlock(&g_lock);
}

size_t reg_alloc_live(void) { return g_live; }
unsigned long reg_alloc_calls(void) { return g_calls; }
unsigned long reg_alloc_corruptions(void) { return g_corruptions; }
void reg_alloc_fail_nth(unsigned long n) { g_arm = n; g_since = 0; g_injected = 0; }
int reg_alloc_injected(void) { return g_injected; }

/* ---- harness ------------------------------------------------------------ */
static unsigned long g_checks, g_failed, g_cases, g_failed_cases;
static unsigned long g_case_failed_mark, g_known_open, g_known_fixed;

void reg_known(int holds, const char *id, const char *description) {
    ++g_checks;
    if (holds) {
        ++g_known_fixed;
        printf("    KNOWN-ISSUE %s appears FIXED: promote to REG_CHECK (%s)\n", id, description);
    } else {
        ++g_known_open;
        printf("    KNOWN-ISSUE %s open: %s\n", id, description);
    }
}

void reg_note_check(void) { ++g_checks; }

void reg_fail(const char *file, int line, const char *fmt, ...) {
    va_list ap;
    ++g_failed;
    fprintf(stderr, "    FAIL %s:%d: ", file, line);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}

void reg_run_case(const char *name, void (*fn)(void)) {
    size_t live_before = g_live;
    unsigned long corrupt_before = g_corruptions;
    unsigned long failed_before = g_failed;
    ++g_cases;
    reg_alloc_fail_nth(0);
    fn();
    reg_alloc_fail_nth(0);
    ++g_checks;
    if (g_live != live_before) {
        reg_fail(__FILE__, __LINE__, "%s leaked %zd allocation(s)", name,
                 (ssize_t)g_live - (ssize_t)live_before);
    }
    ++g_checks;
    if (g_corruptions != corrupt_before) {
        reg_fail(__FILE__, __LINE__, "%s corrupted the heap", name);
    }
    g_case_failed_mark = g_failed - failed_before;
    if (g_case_failed_mark) ++g_failed_cases;
    printf("  %s  %s\n", g_case_failed_mark ? "FAIL" : "ok  ", name);
    fflush(stdout);   /* a later crash must not swallow the results so far */
}

int reg_finish(const char *suite) {
    printf("SUITE %s: cases=%lu failed_cases=%lu checks=%lu failed_checks=%lu known_open=%lu known_fixed=%lu\n",
           suite, g_cases, g_failed_cases, g_checks, g_failed, g_known_open, g_known_fixed);
    return g_failed ? 1 : 0;
}

/* ---- PRNG --------------------------------------------------------------- */
static uint64_t g_rng = 0x9E3779B97F4A7C15ull;

void reg_seed(uint64_t seed) { g_rng = seed ? seed : 0x9E3779B97F4A7C15ull; }

uint64_t reg_next(void) {
    g_rng ^= g_rng >> 12; g_rng ^= g_rng << 25; g_rng ^= g_rng >> 27;
    return g_rng * 2685821657736338717ull;
}

uint32_t reg_below(uint32_t bound) {
    return (uint32_t)((reg_next() >> 32) % bound);
}
