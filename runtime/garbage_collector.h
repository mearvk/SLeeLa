#ifndef SLEELA_GARBAGE_COLLECTOR_H
#define SLEELA_GARBAGE_COLLECTOR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GarbageCollector GarbageCollector;
typedef struct SLGCObject SLGCObject;
typedef enum { SLGC_GENERATION_YOUNG = 0, SLGC_GENERATION_OLD = 1 } SLGCGeneration;
typedef enum { SLGC_IDLE = 0, SLGC_MARKING = 1, SLGC_SWEEPING = 2 } SLGCPhase;
typedef void (*SLGCMarkFn)(SLGCObject *, void *);
typedef void (*SLGCDestroyFn)(void *);
typedef void (*SLGCMarkRootsFn)(GarbageCollector *, void *);

struct SLGCObject {
    void *payload; size_t bytes;
    uint8_t marked, queued, remembered, age, external;
    SLGCGeneration generation;
    SLGCMarkFn mark_children;
    SLGCDestroyFn destroy;
    void *context;
};

struct GarbageCollector {
    SLGCObject **objects; size_t count, capacity;
    SLGCObject **roots; size_t root_count, root_capacity;
    SLGCObject **mark_stack; size_t mark_count, mark_capacity;
    SLGCObject **remembered; size_t remembered_count, remembered_capacity;
    size_t bytes_allocated, bytes_threshold, young_bytes_threshold;
    size_t collections, young_collections, reclaimed, promoted, incremental_steps;
    size_t target_step_work;
    SLGCPhase phase; uint8_t collecting_young, initialized;
};

void gc_init(GarbageCollector *, size_t threshold);
void gc_free(GarbageCollector *);
SLGCObject *gc_allocate(GarbageCollector *, size_t, SLGCMarkFn, SLGCDestroyFn, void *);
SLGCObject *gc_allocate_external(GarbageCollector *, void *, size_t, SLGCMarkFn, SLGCDestroyFn, void *);
int gc_add_root(GarbageCollector *, SLGCObject *);
int gc_remove_root(GarbageCollector *, SLGCObject *);
void gc_mark(GarbageCollector *, SLGCObject *);
void gc_write_barrier(GarbageCollector *, SLGCObject *, SLGCObject *, SLGCObject *);
void gc_start_cycle(GarbageCollector *, int young_only);
size_t gc_step(GarbageCollector *, size_t budget);
size_t gc_collect_young(GarbageCollector *);
size_t gc_collect_full(GarbageCollector *);
size_t gc_collect(GarbageCollector *);
size_t gc_collect_with_roots(GarbageCollector *, SLGCMarkRootsFn, void *, int young_only);
void gc_safepoint(GarbageCollector *, size_t budget);
size_t gc_live_objects(const GarbageCollector *);
size_t gc_bytes(const GarbageCollector *);
size_t gc_collections(const GarbageCollector *);
size_t gc_young_collections(const GarbageCollector *);
size_t gc_promoted_objects(const GarbageCollector *);
SLGCPhase gc_phase(const GarbageCollector *);

#ifdef __cplusplus
}
#endif
#endif