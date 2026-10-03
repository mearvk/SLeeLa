#include "garbage_collector.h"
#include <stdio.h>
static int failures;
static void check(const char *n,int ok){printf("  %s %s\n",ok?"ok  ":"FAIL",n);if(!ok)++failures;}
typedef struct { SLGCObject *child; } Pair;
static void mark_pair(SLGCObject *o,void *ctx){GarbageCollector*g=(GarbageCollector*)ctx;Pair*p=(Pair*)o->payload;if(p&&p->child)gc_mark(g,p->child);}
static int destroyed;
static void destroy_count(void*p){(void)p;++destroyed;}
int main(void){
    GarbageCollector gc;gc_init(&gc,128);
    SLGCObject*root=gc_allocate(&gc,sizeof(Pair),mark_pair,NULL,&gc);
    SLGCObject*child=gc_allocate(&gc,sizeof(int),NULL,NULL,NULL);
    SLGCObject*dead=gc_allocate(&gc,sizeof(int),NULL,destroy_count,NULL);
    check("three objects allocated",root&&child&&dead);
    ((Pair*)root->payload)->child=child;
    check("root registration",gc_add_root(&gc,root)==1);
    gc_write_barrier(&gc,root,NULL,child);
    destroyed=0;
    size_t reclaimed=gc_collect_young(&gc);
    check("minor collection preserves root graph",gc_live_objects(&gc)==2);
    check("minor collection reclaims unreachable object",reclaimed==sizeof(int));
    check("destructor executes",destroyed==1);
    reclaimed=gc_collect_full(&gc);
    check("persistent root survives full collection",gc_live_objects(&gc)==2&&reclaimed==0);
    check("root removal",gc_remove_root(&gc,root)==1);
    reclaimed=gc_collect_full(&gc);
    check("unrooted graph reclaimed",reclaimed==sizeof(Pair)+sizeof(int));
    check("collector reports collections",gc_collections(&gc)>=3);
    gc_free(&gc);
    if(failures){printf("GC SMOKE: FAIL (%d)\n",failures);return 1;}
    printf("GC SMOKE: PASS\n");return 0;
}
