#include "slvm_io_heuristic.h"
#include <string.h>
void slvm_io_heuristic_init(slvm_io_heuristic_t*h){if(h)memset(h,0,sizeof(*h));}
slvm_io_decision_t slvm_io_heuristic_observe(slvm_io_heuristic_t*h,slvm_io_kind_t kind,size_t bytes,uint32_t requests,slvm_security_state_t state){
    (void)kind; if(!h)return SLVM_IO_HEURISTIC_BLOCK;
    h->requests++; h->bytes+=(uint64_t)bytes;
    if(bytes>16ULL*1024ULL*1024ULL || requests>50000u) h->bursts++;
    if(state==SLVM_SECURITY_DENIED)return SLVM_IO_HEURISTIC_BLOCK;
    if(state==SLVM_SECURITY_RESTRICTED || bytes>64ULL*1024ULL*1024ULL)return SLVM_IO_HEURISTIC_THROTTLE;
    if(state==SLVM_SECURITY_ELEVATED || bytes>16ULL*1024ULL*1024ULL)return SLVM_IO_HEURISTIC_MONITOR;
    return SLVM_IO_HEURISTIC_ALLOW;
}
