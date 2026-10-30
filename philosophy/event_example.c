#include "event_tree.h"
#include <inttypes.h>
#include <stdio.h>
static bool show(const sl_event *e,double w,void *ctx){
    size_t *n=(size_t *)ctx;
    printf("id=%" PRIu64 " %s %s w=%.3f %s ",e->id,
           sl_event_type_string(e->type),sl_source_kind_string(e->source),w,e->property);
    if(e->numeric.kind==SL_VALUE_INTEGER)printf("%" PRId64,e->numeric.as.integer);
    else if(e->numeric.kind==SL_VALUE_REAL)printf("%.4f",e->numeric.as.real);
    else printf("(none)");
    printf(" %s\n",e->unit);++*n;return true;
}
int main(void){
    const uint64_t now=UINT64_C(1793400000);sl_event_tree *tree=sl_event_tree_create();
    sl_event e;size_t n=0u,removed=0u;unsigned i;sl_tree_stats st;if(!tree)return 1;
    if(sl_event_init_integer(&e,1,SL_EVENT_ECONOMIC_METRIC,SL_SOURCE_MEDIA_OR_PUBLICATION,
       "industrial_output","index",104,now-86400u,604800u,0,true,
       "named TV economic report","Illustrative published value")!=SL_STATUS_OK ||
       sl_event_tree_insert(tree,&e)!=SL_STATUS_OK){sl_event_tree_destroy(tree);return 1;}
    if(sl_event_init_real(&e,2,SL_EVENT_HEALTH_CONTEXT,SL_SOURCE_SELF_REPORT,
       "self_reported_measure","mg",0.0,now-3600u,86400u,0,false,
       "subject-entered","Private health context; not treatment advice")!=SL_STATUS_OK ||
       sl_event_tree_insert(tree,&e)!=SL_STATUS_OK){sl_event_tree_destroy(tree);return 1;}
    if(sl_event_init_integer(&e,3,SL_EVENT_REFLECTION,SL_SOURCE_SELF_REPORT,
       "revisit","count",1,now-600u,7200u,1,true,
       "first-person reflection","A revisit, not independent corroboration")!=SL_STATUS_OK ||
       sl_event_tree_insert(tree,&e)!=SL_STATUS_OK){sl_event_tree_destroy(tree);return 1;}
    for(i=100u;i<1100u;++i){
        if(sl_event_init_integer(&e,i,SL_EVENT_SOCIAL,SL_SOURCE_DIRECT_OBSERVATION,
           "event_marker","count",(int64_t)i,now-30u,86400u,0,true,
           "explicitly submitted","")!=SL_STATUS_OK ||
           sl_event_tree_insert(tree,&e)!=SL_STATUS_OK){sl_event_tree_destroy(tree);return 1;}
    }
    st=sl_event_tree_get_stats(tree);printf("AVL count=%zu height=%zu\n",st.count,st.height);
    (void)sl_event_tree_visit(tree,now,0.5,show,&n);printf("Above floor: %zu\n",n);
    (void)sl_event_tree_prune(tree,now,0.25,&removed);printf("Pruned: %zu\n",removed);
    sl_event_tree_destroy(tree);return 0;
}
