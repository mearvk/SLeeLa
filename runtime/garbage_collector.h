#ifndef SLEELA_GARBAGE_COLLECTOR_H
#define SLEELA_GARBAGE_COLLECTOR_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct SLGCObject SLGCObject;
typedef void (*SLGCMarkFn)(SLGCObject *, void *);
typedef void (*SLGCDestroyFn)(void *);
typedef struct { SLGCObject **objects; size_t count, capacity, bytes_allocated, bytes_threshold, collections, reclaimed; } GarbageCollector;
struct SLGCObject { void *payload; size_t bytes; uint8_t marked; SLGCMarkFn mark_children; SLGCDestroyFn destroy; void *context; };
void gc_init(GarbageCollector *, size_t);
void gc_free(GarbageCollector *);
SLGCObject *gc_allocate(GarbageCollector *, size_t, SLGCMarkFn, SLGCDestroyFn, void *);
void gc_add_root(GarbageCollector *, SLGCObject *);
void gc_mark(GarbageCollector *, SLGCObject *);
size_t gc_collect(GarbageCollector *);
size_t gc_live_objects(const GarbageCollector *);
size_t gc_bytes(const GarbageCollector *);
#ifdef __cplusplus
}
#endif
#endif