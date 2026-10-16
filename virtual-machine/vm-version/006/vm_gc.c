/*
 * virtual-machine/vm-version/005/vm_gc.c
 * VM v005 -- GC quality layer (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_gc.h"
#include "vm_result.h"
#include "vm_report.h"
#include "vm_telemetry.h"

#include <stdio.h>

static double        g_min_reclaim = 0.25;
static double        g_thrash_per_k = 50.0;
static VmGcSnapshot  g_prev;
static int           g_have_prev = 0;
static VmGcCondition g_cond = VM_GC_HEALTHY;
static VmGcCondition g_reported = VM_GC_HEALTHY;  /* edge-trigger latch */
static int           g_collect_young = 0;

void vm_gc_init(void) {
    g_prev = (VmGcSnapshot){0,0,0,0,0,0};
    g_have_prev = 0;
    g_cond = g_reported = VM_GC_HEALTHY;
    g_collect_young = 0;
}

void vm_gc_set_thresholds(double min_reclaim_ratio, double thrash_per_k) {
    if (min_reclaim_ratio < 0.0) min_reclaim_ratio = 0.0;
    if (min_reclaim_ratio > 1.0) min_reclaim_ratio = 1.0;
    if (thrash_per_k < 0.0) thrash_per_k = 0.0;
    g_min_reclaim = min_reclaim_ratio;
    g_thrash_per_k = thrash_per_k;
}

static VmGcCondition classify(uint64_t d_alloc, uint64_t d_reclaim,
                              uint64_t d_coll) {
    if (d_alloc == 0) return VM_GC_HEALTHY;
    double reclaim_ratio = (double)d_reclaim / (double)d_alloc;
    double coll_per_k = (double)d_coll * 1000.0 / (double)d_alloc;

    if (coll_per_k >= g_thrash_per_k && reclaim_ratio < g_min_reclaim)
        return VM_GC_THRASH;
    if (reclaim_ratio < g_min_reclaim)
        return VM_GC_PRESSURE;
    if (d_coll > 0)
        return VM_GC_BUSY;
    return VM_GC_HEALTHY;
}

VmGcCondition vm_gc_observe(const VmGcSnapshot *s) {
    if (!s) return g_cond;
    if (!g_have_prev) { g_prev = *s; g_have_prev = 1; g_cond = VM_GC_HEALTHY; return g_cond; }

    uint64_t d_alloc   = (s->allocated  >= g_prev.allocated)  ? s->allocated  - g_prev.allocated  : 0;
    uint64_t d_reclaim = (s->reclaimed  >= g_prev.reclaimed)  ? s->reclaimed  - g_prev.reclaimed  : 0;
    uint64_t d_coll    = (s->collections>= g_prev.collections)? s->collections- g_prev.collections: 0;

    /* feed telemetry: each collection is 'work'; allocs tracked */
    vm_telemetry_add(VM_COUNTER_ALLOCS, d_alloc);
    vm_telemetry_add(VM_COUNTER_BRIDGE, d_coll);

    VmGcCondition c = classify(d_alloc, d_reclaim, d_coll);

    /* pacing: recommend a young collection when allocating fast but not yet
     * thrashing (so a timely minor collection relieves pressure). */
    g_collect_young = (d_alloc > 0 && c == VM_GC_PRESSURE) ? 1 : 0;

    /* edge-triggered reporting on worsening / recovery */
    if (c > g_reported) {
        double ratio = d_alloc ? (double)d_reclaim / (double)d_alloc : 1.0;
        VmResult r = vm_fail(
            c == VM_GC_THRASH ? VM_E_LIMIT : VM_E_EXHAUSTED, "gc",
            "%s: reclaim/alloc=%.2f over %llu allocs, %llu collections",
            vm_gc_condition_name(c), ratio,
            (unsigned long long)d_alloc, (unsigned long long)d_coll);
        vm_result_report(&r);
        g_reported = c;
    } else if (c == VM_GC_HEALTHY && g_reported != VM_GC_HEALTHY) {
        vm_notice("gc", "pressure cleared -- reclamation keeping up");
        g_reported = VM_GC_HEALTHY;
    }

    g_prev = *s;
    g_cond = c;
    return c;
}

VmGcCondition vm_gc_condition(void)      { return g_cond; }
int           vm_gc_should_collect_young(void) { return g_collect_young; }

int vm_gc_report(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    return snprintf(buf, cap,
        "gc: condition=%s collections=%llu reclaimed=%llu promoted=%llu phase=%d",
        vm_gc_condition_name(g_cond),
        (unsigned long long)g_prev.collections,
        (unsigned long long)g_prev.reclaimed,
        (unsigned long long)g_prev.promoted,
        g_prev.phase);
}

const char *vm_gc_condition_name(VmGcCondition c) {
    switch (c) {
        case VM_GC_HEALTHY:  return "HEALTHY";
        case VM_GC_BUSY:     return "BUSY";
        case VM_GC_PRESSURE: return "PRESSURE";
        case VM_GC_THRASH:   return "THRASH";
        default:             return "HEALTHY";
    }
}
