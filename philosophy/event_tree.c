#include "event_tree.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct sl_node {
    sl_event event;
    struct sl_node *left, *right;
    int height;
} sl_node;
struct sl_event_tree { sl_node *root; size_t count; };

static bool copy_text(char *dst,size_t cap,const char *src,bool empty_ok) {
    size_t n;
    if(!dst || !src || cap==0u) return false;
    n=strlen(src);
    if((!empty_ok && n==0u) || n>=cap) return false;
    memcpy(dst,src,n+1u); return true;
}
static bool valid_type(sl_event_type t) { return (unsigned)t<=(unsigned)SL_EVENT_REFLECTION; }
static bool valid_source(sl_source_kind s) { return (unsigned)s<=(unsigned)SL_SOURCE_SYSTEM_IMPORT; }
static bool valid_event(const sl_event *e) {
    if(!e || e->id==0u || e->occurred_at==0u || e->half_life_seconds==0u ||
       !valid_type(e->type) || !valid_source(e->source)) return false;
    if(!memchr(e->property,'\0',sizeof(e->property)) || e->property[0]=='\0' ||
       !memchr(e->unit,'\0',sizeof(e->unit)) ||
       !memchr(e->source_reference,'\0',sizeof(e->source_reference)) ||
       !memchr(e->note,'\0',sizeof(e->note)) ||
       !memchr(e->assessor_role,'\0',sizeof(e->assessor_role))) return false;
    if(e->ended_at && e->ended_at<e->occurred_at) return false;
    if(e->numeric.kind==SL_VALUE_REAL && !isfinite(e->numeric.as.real)) return false;
    if(e->numeric.kind!=SL_VALUE_NONE && e->numeric.kind!=SL_VALUE_INTEGER &&
       e->numeric.kind!=SL_VALUE_REAL) return false;
    if(e->type==SL_EVENT_ASSESSOR_OPINION)
        return e->source==SL_SOURCE_ASSESSOR_AUTHORED && e->assessor_attested &&
               e->consent_to_share && e->assessor_role[0] && e->note[0];
    return !e->assessor_attested;
}
static sl_status init_event(
    sl_event *out,uint64_t id,sl_event_type type,sl_source_kind source,
    const char *property,const char *unit,sl_numeric_value numeric,
    uint64_t at,uint64_t half,uint64_t related,bool consent,
    const char *reference,const char *note) {
    sl_event e;
    if(!out || !property || !unit || !reference || !note || !id || !at || !half ||
       !valid_type(type) || !valid_source(source)) return SL_STATUS_INVALID_ARGUMENT;
    if(numeric.kind==SL_VALUE_REAL && !isfinite(numeric.as.real)) return SL_STATUS_INVALID_NUMBER;
    if(numeric.kind!=SL_VALUE_NONE && numeric.kind!=SL_VALUE_INTEGER &&
       numeric.kind!=SL_VALUE_REAL) return SL_STATUS_INVALID_ARGUMENT;
    memset(&e,0,sizeof(e));
    if(!copy_text(e.property,sizeof(e.property),property,false) ||
       !copy_text(e.unit,sizeof(e.unit),unit,true) ||
       !copy_text(e.source_reference,sizeof(e.source_reference),reference,true) ||
       !copy_text(e.note,sizeof(e.note),note,true)) return SL_STATUS_INVALID_ARGUMENT;
    e.id=id;e.type=type;e.source=source;e.numeric=numeric;e.occurred_at=at;
    e.half_life_seconds=half;e.related_event_id=related;e.consent_to_share=consent;
    *out=e;return SL_STATUS_OK;
}
sl_status sl_event_init_integer(
    sl_event *out,uint64_t id,sl_event_type type,sl_source_kind source,
    const char *property,const char *unit,int64_t value,uint64_t at,
    uint64_t half,uint64_t related,bool consent,const char *reference,const char *note) {
    sl_numeric_value n;n.kind=SL_VALUE_INTEGER;n.as.integer=value;
    return init_event(out,id,type,source,property,unit,n,at,half,related,consent,reference,note);
}
sl_status sl_event_init_real(
    sl_event *out,uint64_t id,sl_event_type type,sl_source_kind source,
    const char *property,const char *unit,double value,uint64_t at,
    uint64_t half,uint64_t related,bool consent,const char *reference,const char *note) {
    sl_numeric_value n;n.kind=SL_VALUE_REAL;n.as.real=value;
    return init_event(out,id,type,source,property,unit,n,at,half,related,consent,reference,note);
}
sl_status sl_event_set_assessor_opinion(
    sl_event *e,const char *role,const char *statement,bool attested) {
    char r[SL_EVENT_ROLE_CAPACITY],s[SL_EVENT_NOTE_CAPACITY];
    if(!e || !role || !statement || e->type!=SL_EVENT_ASSESSOR_OPINION ||
       e->source!=SL_SOURCE_ASSESSOR_AUTHORED || !e->consent_to_share || !attested)
        return SL_STATUS_INVALID_ARGUMENT;
    if(!copy_text(r,sizeof(r),role,false) || !copy_text(s,sizeof(s),statement,false))
        return SL_STATUS_INVALID_ARGUMENT;
    memset(e->assessor_role,0,sizeof(e->assessor_role));
    memset(e->note,0,sizeof(e->note));
    memcpy(e->assessor_role,r,strlen(r)+1u);
    memcpy(e->note,s,strlen(s)+1u);
    e->assessor_attested=true;return SL_STATUS_OK;
}

static int height(const sl_node *n){return n?n->height:0;}
static int max2(int a,int b){return a>b?a:b;}
static void fix_height(sl_node *n){n->height=1+max2(height(n->left),height(n->right));}
static sl_node *rotate_right(sl_node *y){
    sl_node *x=y->left,*middle=x->right;x->right=y;y->left=middle;
    fix_height(y);fix_height(x);return x;
}
static sl_node *rotate_left(sl_node *x){
    sl_node *y=x->right,*middle=y->left;y->left=x;x->right=middle;
    fix_height(x);fix_height(y);return y;
}
static sl_node *rebalance(sl_node *n){
    int b;if(!n)return NULL;fix_height(n);b=height(n->left)-height(n->right);
    if(b>1){if(height(n->left->left)<height(n->left->right))n->left=rotate_left(n->left);return rotate_right(n);}
    if(b< -1){if(height(n->right->right)<height(n->right->left))n->right=rotate_right(n->right);return rotate_left(n);}
    return n;
}
static sl_node *insert_rec(sl_node *root,sl_node *added,sl_status *status){
    if(!root)return added;
    if(added->event.id<root->event.id)root->left=insert_rec(root->left,added,status);
    else if(added->event.id>root->event.id)root->right=insert_rec(root->right,added,status);
    else{*status=SL_STATUS_DUPLICATE_ID;return root;}
    return rebalance(root);
}
sl_event_tree *sl_event_tree_create(void){return (sl_event_tree *)calloc(1u,sizeof(sl_event_tree));}
static void free_nodes(sl_node *n){if(n){free_nodes(n->left);free_nodes(n->right);free(n);}}
void sl_event_tree_destroy(sl_event_tree *t){if(t){free_nodes(t->root);free(t);}}
sl_status sl_event_tree_insert(sl_event_tree *t,const sl_event *e){
    sl_node *n;sl_status s=SL_STATUS_OK;
    if(!t || !valid_event(e))return SL_STATUS_INVALID_EVENT;
    n=(sl_node *)calloc(1u,sizeof(*n));if(!n)return SL_STATUS_OUT_OF_MEMORY;
    n->event=*e;n->height=1;t->root=insert_rec(t->root,n,&s);
    if(s!=SL_STATUS_OK){free(n);return s;}++t->count;return SL_STATUS_OK;
}
static sl_node *find_mut(sl_node *n,uint64_t id){
    while(n){if(id<n->event.id)n=n->left;else if(id>n->event.id)n=n->right;else return n;}return NULL;
}
sl_status sl_event_tree_find(const sl_event_tree *t,uint64_t id,sl_event *out){
    const sl_node *n;
    if(!t || !out || !id)return SL_STATUS_INVALID_ARGUMENT;
    n=t->root;
    while(n){if(id<n->event.id)n=n->left;else if(id>n->event.id)n=n->right;else{*out=n->event;return SL_STATUS_OK;}}
    return SL_STATUS_NOT_FOUND;
}
sl_status sl_event_tree_end(sl_event_tree *t,uint64_t id,uint64_t ended){
    sl_node *n;if(!t || !id || !ended)return SL_STATUS_INVALID_ARGUMENT;
    n=find_mut(t->root,id);if(!n)return SL_STATUS_NOT_FOUND;
    if(ended<n->event.occurred_at || (n->event.ended_at && ended<n->event.ended_at))
        return SL_STATUS_INVALID_ARGUMENT;
    n->event.ended_at=ended;return SL_STATUS_OK;
}
double sl_event_relevance(const sl_event *e,uint64_t now){
    uint64_t anchor,elapsed;if(!valid_event(e))return 0.0;
    anchor=e->ended_at?e->ended_at:e->occurred_at;elapsed=now>anchor?now-anchor:0u;
    return exp2(-((double)elapsed/(double)e->half_life_seconds));
}
static bool visit_rec(const sl_node *n,uint64_t now,double floor_w,sl_event_visitor fn,void *ctx){
    double w;if(!n)return true;
    if(!visit_rec(n->left,now,floor_w,fn,ctx))return false;
    w=sl_event_relevance(&n->event,now);
    if(w>=floor_w && !fn(&n->event,w,ctx))return false;
    return visit_rec(n->right,now,floor_w,fn,ctx);
}
sl_status sl_event_tree_visit(const sl_event_tree *t,uint64_t now,double floor_w,sl_event_visitor fn,void *ctx){
    if(!t || !fn || !isfinite(floor_w) || floor_w<0.0 || floor_w>1.0)
        return SL_STATUS_INVALID_ARGUMENT;
    (void)visit_rec(t->root,now,floor_w,fn,ctx);return SL_STATUS_OK;
}
static sl_node *detach_min(sl_node *n,sl_node **out){
    if(!n->left){sl_node *r=n->right;n->right=NULL;*out=n;return r;}
    n->left=detach_min(n->left,out);return rebalance(n);
}
static sl_node *remove_root(sl_node *n){
    sl_node *r,*successor;
    if(!n->left){r=n->right;free(n);return r;}
    if(!n->right){r=n->left;free(n);return r;}
    r=detach_min(n->right,&successor);successor->left=n->left;successor->right=r;
    free(n);return rebalance(successor);
}
static sl_node *prune_rec(sl_node *n,uint64_t now,double floor_w,size_t *removed){
    if(!n)return NULL;
    n->left=prune_rec(n->left,now,floor_w,removed);
    n->right=prune_rec(n->right,now,floor_w,removed);
    if(sl_event_relevance(&n->event,now)<floor_w){++*removed;return remove_root(n);}
    return rebalance(n);
}
sl_status sl_event_tree_prune(sl_event_tree *t,uint64_t now,double floor_w,size_t *out){
    size_t removed=0u;if(!t || !out || !isfinite(floor_w) || floor_w<0.0 || floor_w>1.0)
        return SL_STATUS_INVALID_ARGUMENT;
    t->root=prune_rec(t->root,now,floor_w,&removed);t->count-=removed;*out=removed;
    return SL_STATUS_OK;
}
sl_tree_stats sl_event_tree_get_stats(const sl_event_tree *t){
    sl_tree_stats s={0u,0u};if(t){s.count=t->count;s.height=(size_t)height(t->root);}return s;
}
const char *sl_status_string(sl_status s){
    switch(s){case SL_STATUS_OK:return "ok";case SL_STATUS_INVALID_ARGUMENT:return "invalid argument";
    case SL_STATUS_INVALID_NUMBER:return "invalid number";case SL_STATUS_INVALID_EVENT:return "invalid event";
    case SL_STATUS_DUPLICATE_ID:return "duplicate event ID";case SL_STATUS_NOT_FOUND:return "not found";
    case SL_STATUS_OUT_OF_MEMORY:return "out of memory";default:return "unknown status";}
}
const char *sl_event_type_string(sl_event_type t){
    switch(t){case SL_EVENT_SOCIAL:return "social";case SL_EVENT_ECONOMIC_METRIC:return "economic-metric";
    case SL_EVENT_HEALTH_CONTEXT:return "health-context";case SL_EVENT_MEDICAL_REQUEST:return "medical-request";
    case SL_EVENT_ASSESSOR_OPINION:return "assessor-opinion";case SL_EVENT_REFLECTION:return "reflection";
    default:return "unknown";}
}
const char *sl_source_kind_string(sl_source_kind s){
    switch(s){case SL_SOURCE_SELF_REPORT:return "self-report";case SL_SOURCE_DIRECT_OBSERVATION:return "direct-observation";
    case SL_SOURCE_MEDIA_OR_PUBLICATION:return "media-or-publication";case SL_SOURCE_ASSESSOR_AUTHORED:return "assessor-authored";
    case SL_SOURCE_SYSTEM_IMPORT:return "system-import";default:return "unknown";}
}
