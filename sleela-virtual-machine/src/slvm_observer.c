#include "slvm_observer.h"
#include <string.h>
void slvm_observer_init(slvm_observer_t *o){if(o)memset(o,0,sizeof(*o));}
int slvm_observer_attach(slvm_observer_t *o,slvm_observer_hook_fn f,void *c,int cert){if(!o||!f)return 0;o->hook=f;o->context=c;o->enabled=1;o->certified_only=cert?1:0;o->redact_values=1;return 1;}
void slvm_observer_detach(slvm_observer_t *o){if(o)memset(o,0,sizeof(*o));}
int slvm_observer_emit(slvm_observer_t *o,const slvm_observer_record_t *r){if(!o||!r||!o->enabled||!o->hook)return 1;slvm_observer_record_t c=*r;if(o->redact_values&&r->value_is_secret){c.value=NULL;c.value_size=0;}return o->hook(&c,o->context);}