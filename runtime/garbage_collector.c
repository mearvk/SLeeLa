#include "garbage_collector.h"
#include <stdlib.h>
#include <string.h>
static int grow(GarbageCollector*g){if(g->count<g->capacity)return 1;size_t c=g->capacity?g->capacity*2:16;SLGCObject**p=realloc(g->objects,c*sizeof(*p));if(!p)return 0;g->objects=p;g->capacity=c;return 1;}
void gc_init(GarbageCollector*g,size_t t){memset(g,0,sizeof(*g));g->bytes_threshold=t?t:1024*1024;}
void gc_mark(GarbageCollector*g,SLGCObject*o){if(!g||!o||o->marked)return;o->marked=1;if(o->mark_children)o->mark_children(o,o->context);}
void gc_add_root(GarbageCollector*g,SLGCObject*o){gc_mark(g,o);}
SLGCObject*gc_allocate(GarbageCollector*g,size_t b,SLGCMarkFn m,SLGCDestroyFn d,void*c){if(!g||!grow(g))return NULL;SLGCObject*o=calloc(1,sizeof(*o));if(!o)return NULL;o->payload=b?calloc(1,b):NULL;if(b&&!o->payload){free(o);return NULL;}o->bytes=b;o->mark_children=m;o->destroy=d;o->context=c;g->objects[g->count++]=o;g->bytes_allocated+=b;return o;}
size_t gc_collect(GarbageCollector*g){if(!g)return 0;size_t r=0,w=0;for(size_t i=0;i<g->count;i++){SLGCObject*o=g->objects[i];if(o->marked){o->marked=0;g->objects[w++]=o;}else{if(o->destroy)o->destroy(o->payload);free(o->payload);r+=o->bytes;free(o);}}g->count=w;g->bytes_allocated-=r;g->collections++;g->reclaimed+=r;return r;}
size_t gc_live_objects(const GarbageCollector*g){return g?g->count:0;} size_t gc_bytes(const GarbageCollector*g){return g?g->bytes_allocated:0;}
void gc_free(GarbageCollector*g){if(!g)return;for(size_t i=0;i<g->count;i++){SLGCObject*o=g->objects[i];if(o->destroy)o->destroy(o->payload);free(o->payload);free(o);}free(g->objects);memset(g,0,sizeof(*g));}