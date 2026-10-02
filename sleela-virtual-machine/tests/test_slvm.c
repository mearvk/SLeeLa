

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
#include <assert.h>
int main(void){const uint8_t p[]={SLVM_OP_CONST,2,0,0,0,0,0,0,0,SLVM_OP_CONST,3,0,0,0,0,0,0,0,SLVM_OP_ADD,SLVM_OP_HALT};slvm_word_t s[16];slvm_t v;slvm_init(&v,p,sizeof(p),s,16);assert(slvm_run(&v)==SLVM_HALTED);assert(v.stack_size==1&&v.stack[0]==5);test_memory_security();
    return 0;
}
