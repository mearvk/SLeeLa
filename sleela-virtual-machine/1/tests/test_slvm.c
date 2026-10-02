

static void test_memory_security(void) {
    slvm_memory_security_t m;
    slvm_memory_security_init(&m, 1024);
    assert(slvm_memory_security_check(&m, 128, 1) == SLVM_MEMORY_ALLOW);
    assert(slvm_memory_security_check(&m, 512, 1) != SLVM_MEMORY_DENY);
    assert(slvm_memory_security_check(&m, 2048, 1) == SLVM_MEMORY_DENY);
    slvm_memory_security_record_free(&m, 512);
    assert(slvm_memory_security_state(&m) != SLVM_MEMORY_DENIED);
}
#include "slvm.h"
#include "slvm_observer.h"
#include "slvm_broker_security.h"
#include <assert.h>

static int observer_seen=0;
static int observer_hook(const slvm_observer_record_t *r, void *ctx){(void)ctx;if(r->event==SLVM_OBSERVER_PARAMETER && r->value==NULL && r->value_is_secret)observer_seen=1;return 1;}
static void test_observer_and_broker(void){
    slvm_observer_t o; slvm_observer_init(&o); assert(slvm_observer_attach(&o,observer_hook,NULL,1));
    const char secret[]="secret"; assert(slvm_observer_function_enter(&o,1,2));
    assert(slvm_observer_parameter(&o,1,2,0,secret,sizeof(secret),1)); assert(observer_seen);
    assert(slvm_observer_function_return(&o,1,2,0)); slvm_observer_detach(&o);
    slvm_broker_security_t bs; slvm_broker_security_init(&bs); assert(!slvm_broker_security_validate(&bs));
}

int main(void){const uint8_t p[]={SLVM_OP_CONST,2,0,0,0,0,0,0,0,SLVM_OP_CONST,3,0,0,0,0,0,0,0,SLVM_OP_ADD,SLVM_OP_HALT};slvm_word_t s[16];slvm_t v;slvm_init(&v,p,sizeof(p),s,16);assert(slvm_run(&v)==SLVM_HALTED);assert(v.stack_size==1&&v.stack[0]==5);test_memory_security();test_observer_and_broker();
    return 0;
}
