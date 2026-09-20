/* ===========================================================================
 * runtime_smoke.c -- behavioral tests for the SLVM runtime services.
 *
 * Until now runtime/garbage_collector.c and runtime/security_supervisor.c were
 * only *compiled* by `make build-runtime` -- nothing asserted they actually
 * behave. This test exercises real behavior:
 *
 *   GC:  allocation + byte accounting; mark-and-sweep reclaims exactly the
 *        unmarked objects; destructors run on reclaimed payloads; collection
 *        counters advance.
 *   Sec: class/role policies gate allocation; reserve() honors per-class
 *        instance and byte caps and per-role totals; authorize() honors the
 *        allowed flag; release() gives quota back.
 *
 * Exit 0 = all checks passed; non-zero = a check failed (with a message).
 * =========================================================================== */
#include "garbage_collector.h"
#include "security_supervisor.h"
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

/* A destroy callback that records, via its context, that it ran. */
static int g_destroyed = 0;
static void count_destroy(void *payload) { (void)payload; g_destroyed++; }

static void test_gc(void) {
    printf("[gc] mark-sweep, byte accounting, destructors\n");
    GarbageCollector gc;
    gc_init(&gc, 0);

    /* Allocate three objects of known sizes. */
    SLGCObject *a = gc_allocate(&gc, 100, NULL, NULL, NULL);
    SLGCObject *b = gc_allocate(&gc, 200, NULL, count_destroy, NULL);
    SLGCObject *c = gc_allocate(&gc, 300, NULL, NULL, NULL);
    check("allocate returns non-NULL", a && b && c);
    check("live objects == 3 after allocate", gc_live_objects(&gc) == 3);
    check("bytes == 600 after allocate", gc_bytes(&gc) == 600);

    /* Root a and c; leave b unreachable. */
    gc_add_root(&gc, a);
    gc_add_root(&gc, c);
    g_destroyed = 0;
    size_t reclaimed = gc_collect(&gc);

    check("collect reclaimed b's 200 bytes", reclaimed == 200);
    check("live objects == 2 after collect", gc_live_objects(&gc) == 2);
    check("bytes == 400 after collect", gc_bytes(&gc) == 400);
    check("destructor ran exactly once (on b)", g_destroyed == 1);
    check("collections counter advanced", gc.collections == 1);
    check("reclaimed counter == 200", gc.reclaimed == 200);

    /* A second collect with nothing rooted reclaims the rest. */
    reclaimed = gc_collect(&gc);
    check("second collect reclaimed remaining 400", reclaimed == 400);
    check("live objects == 0 after second collect", gc_live_objects(&gc) == 0);
    check("bytes == 0 after second collect", gc_bytes(&gc) == 0);

    gc_free(&gc);
}

static void test_security(void) {
    printf("[sec] class/role policies gate allocation\n");
    SecuritySupervisor s;
    security_supervisor_init(&s, SLS_ROLE_BUSINESS);

    /* A class allowed up to 2 instances / 500 bytes. */
    security_supervisor_set_class(&s, "Widget", 1, 2, 500);
    /* A class explicitly disallowed. */
    security_supervisor_set_class(&s, "Forbidden", 0, 0, 0);
    /* Role total cap: 3 instances / 1000 bytes. */
    security_supervisor_set_role_limit(&s, SLS_ROLE_BUSINESS, 3, 1000);

    check("authorize allowed class == 1", security_supervisor_authorize_class(&s, "Widget") == 1);
    check("authorize disallowed class == 0", security_supervisor_authorize_class(&s, "Forbidden") == 0);
    check("authorize unknown class == 0", security_supervisor_authorize_class(&s, "Nope") == 0);

    check("reserve #1 within limits", security_supervisor_reserve(&s, "Widget", 100) == 1);
    check("reserve #2 within limits", security_supervisor_reserve(&s, "Widget", 100) == 1);
    check("reserve #3 exceeds class instance cap (2)", security_supervisor_reserve(&s, "Widget", 100) == 0);
    check("reserve on disallowed class refused", security_supervisor_reserve(&s, "Forbidden", 10) == 0);

    /* Release one Widget, then a byte-cap breach should still be refused. */
    security_supervisor_release(&s, "Widget", 100);
    check("reserve after release exceeds byte cap (500)", security_supervisor_reserve(&s, "Widget", 600) == 0);
    check("reserve after release within byte cap", security_supervisor_reserve(&s, "Widget", 100) == 1);

    /* Role total cap: set up a second allowed class and exhaust the role total. */
    SecuritySupervisor r;
    security_supervisor_init(&r, SLS_ROLE_USER);
    security_supervisor_set_class(&r, "Cell", 1, 100, 100000);
    security_supervisor_set_role_limit(&r, SLS_ROLE_USER, 2, 100000);
    check("role reserve #1", security_supervisor_reserve(&r, "Cell", 10) == 1);
    check("role reserve #2", security_supervisor_reserve(&r, "Cell", 10) == 1);
    check("role reserve #3 exceeds role total instances (2)", security_supervisor_reserve(&r, "Cell", 10) == 0);
}

int main(void) {
    printf("=== SLVM runtime services -- behavioral smoke ===\n");
    test_gc();
    test_security();
    if (failures == 0) {
        printf("RUNTIME SMOKE: PASS\n");
        return 0;
    }
    printf("RUNTIME SMOKE: FAIL (%d)\n", failures);
    return 1;
}
