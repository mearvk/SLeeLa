/*
 * virtual-machine/vm-version/005/vm_gc.h
 * VM v005 -- GC quality: pressure, pacing, and observability.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001-004. The repo's generational mark-sweep collector
 * (runtime/garbage_collector.*) exposes phase, generations, a write barrier,
 * and stats (collections, reclaimed, promoted). v005 adds a thin quality layer:
 *
 *   - a GC pressure signal derived from allocation vs. reclamation rate;
 *   - collection pacing advice (should we collect young now?);
 *   - surfacing it all through v004 telemetry counters and v003 typed results,
 *     so GC behaviour is observable and the user is warned on sustained
 *     pressure before the heap thrashes.
 *
 * Pure C over the GC stats when VM_HAVE_GC is defined; otherwise it models the
 * same signals from fed-in numbers so the layer builds and is tested standalone.
 */
#ifndef VM_GC_H
#define VM_GC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VM_GC_HEALTHY   = 0,  /* reclamation keeps up with allocation           */
    VM_GC_BUSY      = 1,   /* collecting often but reclaiming well           */
    VM_GC_PRESSURE  = 2,   /* allocation outpacing reclamation               */
    VM_GC_THRASH    = 3    /* frequent collections, little reclaimed         */
} VmGcCondition;

/* A snapshot of GC activity the quality layer reasons over. Field names mirror
 * the collector's GCStats. */
typedef struct {
    uint64_t collections;        /* total collections                       */
    uint64_t young_collections;  /* minor collections                       */
    uint64_t reclaimed;          /* objects/bytes reclaimed (cumulative)    */
    uint64_t promoted;           /* young -> old promotions                 */
    uint64_t allocated;          /* allocations since start (cumulative)    */
    int      phase;              /* 0 idle, 1 marking, 2 sweeping           */
} VmGcSnapshot;

/* Reset the quality layer's running baselines. Call once at startup. */
void vm_gc_init(void);

/* Set the pressure watermark (reclaim/alloc ratio below which = pressure;
 * default 0.25) and the thrash threshold (collections per 1000 allocs above
 * which, combined with low reclaim, = thrash; default 50). */
void vm_gc_set_thresholds(double min_reclaim_ratio, double thrash_per_k);

/* Feed a fresh snapshot; the layer computes deltas since the last call,
 * updates v004 telemetry (bridge/errors as appropriate), classifies the
 * condition, and edge-triggers a typed vm_result (via v003) when it worsens to
 * PRESSURE/THRASH (and a recovery notice back to HEALTHY). Returns the current
 * condition. */
VmGcCondition vm_gc_observe(const VmGcSnapshot *s);

/* The condition from the most recent observe, without re-evaluating. */
VmGcCondition vm_gc_condition(void);

/* Pacing advice: 1 if a young collection is recommended now given recent
 * allocation pressure, else 0. */
int vm_gc_should_collect_young(void);

/* Render a one-line GC quality report (snprintf semantics). */
int vm_gc_report(char *buf, size_t cap);

const char *vm_gc_condition_name(VmGcCondition c);

#ifdef __cplusplus
}
#endif

#endif /* VM_GC_H */
