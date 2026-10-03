#include "garbage_collector.h"
#include <stdlib.h>
#include <string.h>

static int grow(SLGCObject ***a, size_t *n, size_t *cap) {
    if (*n < *cap) return 1; size_t next = *cap ? *cap * 2u : 32u;
    SLGCObject **p = (SLGCObject **)realloc(*a, next * sizeof(*p));
    if (!p) return 0; *a = p; *cap = next; return 1;
}
static int live(const GarbageCollector *g, const SLGCObject *o) {
    if (!g || !o) return 0; for (size_t i=0;i<g->count;i++) if (g->objects[i]==o) return 1; return 0;
}
static int push_unique(SLGCObject ***a, size_t *n, size_t *cap, SLGCObject *o) {
    if (!o) return 1; for (size_t i=0;i<*n;i++) if ((*a)[i]==o) return 1;
    if (!grow(a,n,cap)) return 0; (*a)[(*n)++]=o; return 1;
}
static void mark_internal(GarbageCollector *g, SLGCObject *o) {
    if (!g || !o || !live(g,o) || o->marked) return;
    o->marked=1; if (!o->queued) { o->queued=1; (void)push_unique(&g->mark_stack,&g->mark_count,&g->mark_capacity,o); }
}
static void clear_marks(GarbageCollector *g) {
    for (size_t i=0;i<g->count;i++) { g->objects[i]->marked=0; g->objects[i]->queued=0; }
    g->mark_count=0;
}
void gc_init(GarbageCollector *g, size_t threshold) {
    if (!g) return; memset(g,0,sizeof(*g)); g->bytes_threshold=threshold?threshold:1024u*1024u;
    g->young_bytes_threshold=g->bytes_threshold/4u; if (g->young_bytes_threshold<64u*1024u) g->young_bytes_threshold=64u*1024u;
    g->target_step_work=32u; g->phase=SLGC_IDLE; g->initialized=1;
}
SLGCObject *gc_allocate(GarbageCollector *g,size_t bytes,SLGCMarkFn m,SLGCDestroyFn d,void *c) {
    if (!g||!g->initialized||!grow(&g->objects,&g->count,&g->capacity)) return NULL;
    SLGCObject *o=(SLGCObject*)calloc(1,sizeof(*o)); if(!o)return NULL;
    if(bytes){o->payload=calloc(1,bytes);if(!o->payload){free(o);return NULL;}}
    o->bytes=bytes;o->generation=SLGC_GENERATION_YOUNG;o->mark_children=m;o->destroy=d;o->context=c;o->external=0;
    g->objects[g->count++]=o;g->bytes_allocated+=bytes;return o;
}
SLGCObject *gc_allocate_external(GarbageCollector *g,void *payload,size_t bytes,SLGCMarkFn m,SLGCDestroyFn d,void *c) {
    if(!g||!g->initialized||!payload||!grow(&g->objects,&g->count,&g->capacity)) return NULL;
    SLGCObject *o=(SLGCObject*)calloc(1,sizeof(*o));if(!o)return NULL;
    o->payload=payload;o->bytes=bytes;o->generation=SLGC_GENERATION_YOUNG;o->mark_children=m;o->destroy=d;o->context=c;o->external=1;
    g->objects[g->count++]=o;g->bytes_allocated+=bytes;return o;
}
int gc_add_root(GarbageCollector *g,SLGCObject *o){if(!g||!o||!live(g,o))return 0;return push_unique(&g->roots,&g->root_count,&g->root_capacity,o);}
int gc_remove_root(GarbageCollector *g,SLGCObject *o){
    if(!g||!o)return 0;for(size_t i=0;i<g->root_count;i++)if(g->roots[i]==o){memmove(&g->roots[i],&g->roots[i+1],(g->root_count-i-1u)*sizeof(*g->roots));--g->root_count;return 1;}return 0;
}
void gc_mark(GarbageCollector *g,SLGCObject *o){mark_internal(g,o);}
void gc_write_barrier(GarbageCollector *g,SLGCObject *owner,SLGCObject *oldv,SLGCObject *newv){
    if(!g||!owner)return;
    if(g->phase==SLGC_MARKING&&oldv&&!oldv->marked)mark_internal(g,oldv);
    if(owner->generation==SLGC_GENERATION_OLD&&newv&&newv->generation==SLGC_GENERATION_YOUNG&&!owner->remembered){
        owner->remembered=1;(void)push_unique(&g->remembered,&g->remembered_count,&g->remembered_capacity,owner);
    }
}
void gc_start_cycle(GarbageCollector *g,int young_only){
    if(!g||!g->initialized)return;clear_marks(g);g->phase=SLGC_MARKING;g->collecting_young=young_only?1:0;
    for(size_t i=0;i<g->root_count;i++)mark_internal(g,g->roots[i]);
    if(young_only)for(size_t i=0;i<g->remembered_count;i++)mark_internal(g,g->remembered[i]);
}
size_t gc_step(GarbageCollector *g,size_t budget){
    if(!g||!g->initialized||g->phase!=SLGC_MARKING)return 0;if(!budget)budget=g->target_step_work;size_t work=0;
    while(g->mark_count&&work<budget){SLGCObject*o=g->mark_stack[--g->mark_count];o->queued=0;if(o->mark_children)o->mark_children(o,o->context);++work;++g->incremental_steps;}
    if(!g->mark_count)g->phase=SLGC_SWEEPING;return work;
}
static size_t sweep(GarbageCollector *g);\n\nstatic size_t sweep(GarbageCollector *g){
    size_t reclaimed=0,promoted=0,write=0;
    for(size_t i=0;i<g->count;i++){SLGCObject*o=g->objects[i];int collect=!o->marked&&(!g->collecting_young||o->generation==SLGC_GENERATION_YOUNG);
        if(collect){if(o->destroy)o->destroy(o->payload);if(!o->external)free(o->payload);reclaimed+=o->bytes;free(o);continue;}
        if(o->marked&&o->generation==SLGC_GENERATION_YOUNG){if(o->age<255)++o->age;if(!g->collecting_young||o->age>=2){o->generation=SLGC_GENERATION_OLD;++promoted;}}
        o->marked=0;o->queued=0;g->objects[write++]=o;}
    g->count=write;g->bytes_allocated=g->bytes_allocated>=reclaimed?g->bytes_allocated-reclaimed:0;g->reclaimed+=reclaimed;g->promoted+=promoted;
    size_t rw=0;for(size_t i=0;i<g->remembered_count;i++){SLGCObject*o=g->remembered[i];if(live(g,o)){o->remembered=0;g->remembered[rw++]=o;}}g->remembered_count=rw;
    g->phase=SLGC_IDLE;return reclaimed;
}
static size_t finish(GarbageCollector *g){while(g->phase==SLGC_MARKING)gc_step(g,g->target_step_work);return sweep(g);}
size_t gc_collect_young(GarbageCollector *g){if(!g||!g->initialized)return 0;gc_start_cycle(g,1);size_t r=finish(g);++g->collections;++g->young_collections;return r;}
size_t gc_collect_full(GarbageCollector *g){if(!g||!g->initialized)return 0;gc_start_cycle(g,0);size_t r=finish(g);++g->collections;return r;}
size_t gc_collect(GarbageCollector *g){return gc_collect_full(g);}
size_t gc_collect_with_roots(GarbageCollector *g,SLGCMarkRootsFn roots,void *ctx,int young_only){
    if(!g||!g->initialized)return 0;clear_marks(g);g->phase=SLGC_MARKING;g->collecting_young=young_only?1:0;
    if(roots)roots(g,ctx);for(size_t i=0;i<g->root_count;i++)mark_internal(g,g->roots[i]);
    if(young_only)for(size_t i=0;i<g->remembered_count;i++)mark_internal(g,g->remembered[i]);
    size_t r=finish(g);++g->collections;if(young_only)++g->young_collections;return r;
}
void gc_safepoint(GarbageCollector *g,size_t budget){
    if(!g||!g->initialized)return;if(g->phase==SLGC_MARKING){gc_step(g,budget?budget:g->target_step_work);return;}
    if(g->bytes_allocated>=g->bytes_threshold)gc_collect_young(g);
}
size_t gc_live_objects(const GarbageCollector*g){return g?g->count:0;}size_t gc_bytes(const GarbageCollector*g){return g?g->bytes_allocated:0;}
size_t gc_collections(const GarbageCollector*g){return g?g->collections:0;}size_t gc_young_collections(const GarbageCollector*g){return g?g->young_collections:0;}
size_t gc_promoted_objects(const GarbageCollector*g){return g?g->promoted:0;}SLGCPhase gc_phase(const GarbageCollector*g){return g?g->phase:SLGC_IDLE;}
void gc_free(GarbageCollector*g){
    if(!g)return;for(size_t i=0;i<g->count;i++){SLGCObject*o=g->objects[i];if(o->destroy)o->destroy(o->payload);if(!o->external)free(o->payload);free(o);}
    free(g->objects);free(g->roots);free(g->mark_stack);free(g->remembered);memset(g,0,sizeof(*g));
}