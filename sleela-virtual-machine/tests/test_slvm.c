#include "slvm.h"

#include <assert.h>
#include <stdint.h>

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
    return 0;
}
