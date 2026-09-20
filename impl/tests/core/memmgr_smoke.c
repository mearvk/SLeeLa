/* ===========================================================================
 * memmgr_smoke.c -- behavioral tests for the SLeeLa Memory Manager.
 *
 * Exercises real behavior of impl/core/sleela_memmgr.c rather than merely
 * compiling it:
 *
 *   * disabled by default: allocations succeed but are NOT counted;
 *   * enabled: live/peak byte accounting and live/total allocation counts are
 *     exact across alloc / calloc (zeroed) / realloc (preserving) / free;
 *   * a hard byte limit fails allocations closed (returns NULL, records a
 *     refusal) instead of over-committing;
 *   * reset() zeroes the counters while keeping the enabled state/limit.
 *
 * Exit 0 = all checks passed; non-zero = a check failed (with a message).
 * =========================================================================== */
#include "sleela_memmgr.h"
#include <stdio.h>
#include <string.h>

static int failures = 0;
static void check(const char *name, int ok) {
    if (ok) {
        printf("  ok   %s\n", name);
    } else {
        printf("  FAIL %s\n", name);
        failures++;
    }
}

static void test_disabled_is_untracked(void) {
    printf("[mm] disabled: allocations work but are not counted\n");
    SLMMStats s;
    void *p = slmm_alloc(128);
    check("alloc while disabled returns memory", p != NULL);
    slmm_stats(&s);
    check("disabled: live_bytes stays 0", s.live_bytes == 0);
    check("disabled: last_status == DISABLED", s.last_status == SLMM_DISABLED);
    slmm_free(p); /* header present even while disabled, so free is safe */
}

static void test_accounting(void) {
    printf("[mm] enabled: exact byte/alloc accounting\n");
    SLMMStats s;
    slmm_enable(0);      /* no limit */
    slmm_reset();

    void *a = slmm_alloc(1000);
    void *b = slmm_calloc(10, 20);   /* 200 bytes, must be zeroed */
    check("alloc + calloc return memory", a && b);

    int zeroed = 1;
    for (int i = 0; i < 200; i++) if (((unsigned char *)b)[i] != 0) zeroed = 0;
    check("calloc memory is zeroed", zeroed);

    slmm_stats(&s);
    check("live_bytes == 1200", s.live_bytes == 1200);
    check("live_allocs == 2", s.live_allocs == 2);
    check("peak_bytes >= 1200", s.peak_bytes >= 1200);

    /* realloc grows and preserves the existing prefix. */
    memset(a, 0x7, 1000);
    a = slmm_realloc(a, 2000);
    check("realloc returns memory", a != NULL);
    int preserved = 1;
    for (int i = 0; i < 1000; i++) if (((unsigned char *)a)[i] != 0x7) preserved = 0;
    check("realloc preserves the old contents", preserved);
    slmm_stats(&s);
    check("live_bytes == 2200 after realloc", s.live_bytes == 2200);

    slmm_free(a);
    slmm_free(b);
    slmm_stats(&s);
    check("live_bytes == 0 after freeing all", s.live_bytes == 0);
    check("live_allocs == 0 after freeing all", s.live_allocs == 0);
    check("peak_bytes retained (>= 2200)", s.peak_bytes >= 2200);
}

static void test_hard_limit(void) {
    printf("[mm] hard limit fails allocation closed\n");
    SLMMStats s;
    slmm_enable(500);
    slmm_reset();
    slmm_set_limit(500);

    void *c = slmm_alloc(400);
    check("alloc within limit succeeds", c != NULL);
    void *d = slmm_alloc(400);   /* 400 + 400 > 500 */
    check("alloc that would exceed limit returns NULL", d == NULL);
    check("last_status == LIMIT", slmm_last_status() == SLMM_LIMIT);
    slmm_stats(&s);
    check("refused counter == 1", s.refused == 1);
    check("live_bytes unchanged at 400", s.live_bytes == 400);

    slmm_free(c);
    slmm_stats(&s);
    check("live_bytes == 0 after free", s.live_bytes == 0);
}

int main(void) {
    printf("=== SLeeLa Memory Manager -- behavioral smoke ===\n");
    test_disabled_is_untracked();
    test_accounting();
    test_hard_limit();
    if (failures == 0) {
        printf("MEMMGR SMOKE: PASS\n");
        return 0;
    }
    printf("MEMMGR SMOKE: FAIL (%d)\n", failures);
    return 1;
}
