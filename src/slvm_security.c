#include "slvm_security.h"
#include <string.h>
static void assess(slvm_security_t*s){if(!s)return;s->state=SLVM_SECURITY_NORMAL;
if(s->window_requests>s->request_rate_limit||s->window_objects>s->object_rate_limit||s->window_io>s->io_rate_limit)s->anomaly_score+=10;
if(s->window_io>s->io_rate_limit/2U)s->anomaly_score+=2;
if(s->window_objects>s->object_rate_limit/2U)s->anomaly_score+=2;
if(s->anomaly_score>=s->anomaly_threshold)s->state=SLVM_SECURITY_RESTRICTED;
if(s->anomaly_score>=s->anomaly_threshold+25U)s->state=SLVM_SECURITY_DENIED;
if(s->anomaly_score>=s->anomaly_threshold/2U&&s->state==SLVM_SECURITY_NORMAL)s->state=SLVM_SECURITY_ELEVATED;}
void slvm_security_init(slvm_security_t*s){if(!s)return;memset(s,0,sizeof(*s));s->request_rate_limit=10000;s->object_rate_limit=1000;s->io_rate_limit=1000;s->anomaly_threshold=50;s->state=SLVM_SECURITY_NORMAL;}
void slvm_security_reset_window(slvm_security_t*s){if(!s)return;s->window_requests=s->window_objects=s->window_io=s->window_instructions=0;if(s->anomaly_score>5)s->anomaly_score-=5;else s->anomaly_score=0;assess(s);}
void slvm_security_set_limits(slvm_security_t*s,uint32_t r,uint32_t o,uint32_t i,uint32_t a){if(!s)return;if(r)s->request_rate_limit=r;if(o)s->object_rate_limit=o;if(i)s->io_rate_limit=i;if(a)s->anomaly_threshold=a;assess(s);}
int slvm_security_observe_instruction(slvm_security_t*s){if(!s)return-1;++s->instructions;++s->window_instructions;assess(s);return s->state==SLVM_SECURITY_DENIED?0:1;}
int slvm_security_observe_resource(slvm_security_t*s,uint32_t objects){if(!s)return-1;++s->resource_requests;++s->window_requests;s->object_requests+=objects;s->window_objects+=objects;assess(s);if(s->state>=SLVM_SECURITY_RESTRICTED)++s->security_events;return s->state==SLVM_SECURITY_DENIED?0:1;}
int slvm_security_observe_io(slvm_security_t*s,slvm_io_kind_t k,size_t b){if(!s)return-1;++s->io_requests;++s->window_requests;++s->window_io;s->io_bytes+=b;if(k==SLVM_IO_FILE_LOAD)++s->file_loads;if(k==SLVM_IO_DYNAMIC_INSTANTIATION)++s->dynamic_instantiations;if(k==SLVM_IO_NETWORK_READ||k==SLVM_IO_NETWORK_WRITE)++s->network_requests;if(k==SLVM_IO_RESOURCE_CONTROL)++s->resource_requests;assess(s);if(s->state>=SLVM_SECURITY_RESTRICTED)++s->security_events;return s->state==SLVM_SECURITY_DENIED?0:1;}
slvm_security_state_t slvm_security_state(const slvm_security_t*s){return s?s->state:SLVM_SECURITY_DENIED;}
uint32_t slvm_security_score(const slvm_security_t*s){return s?s->anomaly_score:UINT32_MAX;}
