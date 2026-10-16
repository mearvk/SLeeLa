#include "slvm_memory_security.h"
#include <string.h>
static void state(slvm_memory_security_t*m){
    if(m->live_bytes>=m->ceiling || m->denied_requests>10){m->state=SLVM_MEMORY_DENIED;return;}
    if(m->live_bytes>=m->ceiling*9/10 || m->burst_count>m->burst_limit){m->state=SLVM_MEMORY_RESTRICTED;return;}
    if(m->live_bytes>=m->ceiling*3/4){m->state=SLVM_MEMORY_PRESSURE;return;}
    m->state=SLVM_MEMORY_NORMAL;
}
void slvm_memory_security_init(slvm_memory_security_t*m,size_t ceiling){
    if(!m)return; memset(m,0,sizeof(*m)); m->ceiling=ceiling?ceiling:512ULL*1024ULL*1024ULL;
    m->allocation_limit=m->ceiling/4; if(!m->allocation_limit)m->allocation_limit=1;
    m->burst_limit=1024; state(m);
}
void slvm_memory_security_set_ceiling(slvm_memory_security_t*m,size_t c){
    if(!m)return; m->ceiling=c?c:512ULL*1024ULL*1024ULL;
    m->allocation_limit=m->ceiling/4; if(!m->allocation_limit)m->allocation_limit=1; state(m);
}
slvm_memory_decision_t slvm_memory_security_check(slvm_memory_security_t*m,size_t bytes,size_t objects){
    if(!m)return SLVM_MEMORY_DENY;
    m->allocation_requests++; m->bytes_requested+=(uint64_t)bytes;
    if(bytes>m->allocation_limit)m->large_allocations++;
    if(bytes>m->ceiling || objects==0 || m->live_bytes>m->ceiling-bytes){m->denied_requests++;state(m);return SLVM_MEMORY_DENY;}
    if(m->state==SLVM_MEMORY_RESTRICTED || m->state==SLVM_MEMORY_DENIED){m->denied_requests++;state(m);return SLVM_MEMORY_DENY;}
    if(bytes>m->allocation_limit || objects>1024){m->burst_count++;state(m);return SLVM_MEMORY_THROTTLE;}
    m->live_bytes+=bytes; m->live_objects+=objects; state(m);
    return m->state==SLVM_MEMORY_PRESSURE?SLVM_MEMORY_COLLECT:SLVM_MEMORY_ALLOW;
}
void slvm_memory_security_record_free(slvm_memory_security_t*m,size_t bytes){
    if(!m)return; if(bytes>=m->live_bytes)m->live_bytes=0;else m->live_bytes-=bytes;
    if(m->live_objects)m->live_objects--; if(m->burst_count)m->burst_count--; state(m);
}
slvm_memory_security_state_t slvm_memory_security_state(const slvm_memory_security_t*m){return m?m->state:SLVM_MEMORY_DENIED;}
