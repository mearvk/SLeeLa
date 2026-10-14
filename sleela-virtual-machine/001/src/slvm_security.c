#include "slvm_security.h"
#include <limits.h>
#include <string.h>
static void refresh(slvm_security_t*s){
    uint32_t score=0;
    if(s->window_requests>s->request_limit) score+=2;
    if(s->object_requests>s->object_limit) score+=3;
    if(s->io_requests>s->io_limit) score+=2;
    if(s->anomaly_score>s->anomaly_limit) score+=3;
    if(score>=7) s->state=SLVM_SECURITY_DENIED;
    else if(score>=5) s->state=SLVM_SECURITY_RESTRICTED;
    else if(score>=2) s->state=SLVM_SECURITY_ELEVATED;
    else s->state=SLVM_SECURITY_NORMAL;
}
void slvm_security_init(slvm_security_t*s){
    if(!s)return; memset(s,0,sizeof(*s));
    s->request_limit=1000000u; s->object_limit=100000u; s->io_limit=100000u; s->anomaly_limit=10u;
    s->state=SLVM_SECURITY_NORMAL;
}
int slvm_security_observe_instruction(slvm_security_t*s){
    if(!s)return 0;
    if(s->instruction_count!=UINT64_MAX)s->instruction_count++;
    if(s->window_requests!=UINT64_MAX)s->window_requests++;
    refresh(s);
    return s->state!=SLVM_SECURITY_DENIED;
}
int slvm_security_observe_resource(slvm_security_t*s,uint32_t objects){
    if(!s)return 0;
    if(s->resource_requests!=UINT64_MAX)s->resource_requests++;
    if(objects>0 && s->object_requests <= UINT64_MAX-objects)s->object_requests+=objects;
    if(s->window_requests!=UINT64_MAX)s->window_requests++;
    refresh(s);
    return s->state!=SLVM_SECURITY_DENIED;
}
int slvm_security_observe_io(slvm_security_t*s,slvm_io_kind_t kind,size_t bytes){
    if(!s)return 0;
    if(s->io_requests!=UINT64_MAX)s->io_requests++;
    if(s->window_requests!=UINT64_MAX)s->window_requests++;
    if(s->io_bytes<=UINT64_MAX-(uint64_t)bytes)s->io_bytes+=(uint64_t)bytes;
    if(s->window_bytes<=UINT64_MAX-(uint64_t)bytes)s->window_bytes+=(uint64_t)bytes;
    if(kind==SLVM_IO_FILE)s->file_loads++;
    else if(kind==SLVM_IO_NETWORK)s->network_requests++;
    else if(kind==SLVM_IO_DYNAMIC)s->dynamic_instantiations++;
    if(bytes>64ULL*1024ULL*1024ULL)s->anomaly_score+=3;
    refresh(s);
    return s->state!=SLVM_SECURITY_DENIED;
}
slvm_security_state_t slvm_security_state(const slvm_security_t*s){return s?s->state:SLVM_SECURITY_DENIED;}
void slvm_security_reset_window(slvm_security_t*s){
    if(!s)return;
    s->window_requests=0;s->window_bytes=0;s->object_requests=0;s->io_requests=0;s->io_bytes=0;
    s->anomaly_score/=2; refresh(s);
}
