#include "slvm.h"
#include <assert.h>

static void test_memory_security(void) {
    slvm_memory_security_t m;
    slvm_memory_security_init(&m, 1024);
    assert(slvm_memory_security_check(&m, 128, 1) == SLVM_MEMORY_ALLOW);
    assert(slvm_memory_security_check(&m, 512, 1) != SLVM_MEMORY_DENY);
    assert(slvm_memory_security_check(&m, 2048, 1) == SLVM_MEMORY_DENY);
    slvm_memory_security_record_free(&m, 512);
    assert(slvm_memory_security_state(&m) != SLVM_MEMORY_DENIED);
}

int main(void) {
    const uint8_t program[] = {
        SLVM_OP_CONST,
        2,0,0,0,0,0,0,0,
        SLVM_OP_CONST,
        3,0,0,0,0,0,0,0,
        SLVM_OP_ADD,
        SLVM_OP_HALT
    };

    slvm_word_t stack[16];
    slvm_t vm;
    slvm_init(&vm, program, sizeof(program), stack, 16);

    assert(slvm_run(&vm) == SLVM_HALTED);
    assert(vm.stack_size == 1);
    assert(vm.stack[0] == 5);
    test_memory_security();
    return 0;
}
