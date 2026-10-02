#include "slvm_observer.h"
#include <string.h>
void slvm_observer_init(slvm_observer_t *o){if(o)memset(o,0,sizeof(*o));}
int slvm_observer_attach(slvm_observer_t *o,slvm_observer_hook_fn f,void *c,int cert){if(!o||!f)return 0;o->hook=f;o->context=c;o->enabled=1;o->certified_only=cert?1:0;o->redact_values=1;return 1;}
void slvm_observer_detach(slvm_observer_t *o){if(o)memset(o,0,sizeof(*o));}
int slvm_observer_emit(slvm_observer_t *o,const slvm_observer_record_t *r){if(!o||!r||!o->enabled||!o->hook)return 1;slvm_observer_record_t c=*r;if(o->redact_values&&r->value_is_secret){c.value=NULL;c.value_size=0;}return o->hook(&c,o->context);}
int slvm_observer_function_enter(slvm_observer_t *o,uint64_t x,uint64_t f){slvm_observer_record_t r={0};r.event=SLVM_OBSERVER_FUNCTION_ENTER;r.execution_id=x;r.function_id=f;return slvm_observer_emit(o,&r);}
int slvm_observer_parameter(slvm_observer_t *o,uint64_t x,uint64_t f,uint32_t i,const void *v,size_t n,uint8_t secret){slvm_observer_record_t r={0};r.event=SLVM_OBSERVER_PARAMETER;r.execution_id=x;r.function_id=f;r.parameter_index=i;r.value=v;r.value_size=n;r.value_is_secret=secret;return slvm_observer_emit(o,&r);}
int slvm_observer_function_return(slvm_observer_t *o,uint64_t x,uint64_t f,int64_t status){slvm_observer_record_t r={0};r.event=SLVM_OBSERVER_FUNCTION_RETURN;r.execution_id=x;r.function_id=f;r.status=status;return slvm_observer_emit(o,&r);}
