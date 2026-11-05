#include "../../core/sleela_core.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void emit_file_info_program(SLVM *vm) {
    int path = slvm_add_const_str(vm, ".");
    assert(path >= 0);
    assert(slvm_begin_func(vm, "main", 0, 0) >= 0);
    assert(slvm_emit(vm, OP_CONST, path) >= 0);
    assert(slvm_emit(vm, OP_OSLS_CALL, OSLS_VM_GET_FILE_INFO) >= 0);
    assert(slvm_emit0(vm, OP_POP) >= 0);
    assert(slvm_emit0(vm, OP_HALT) >= 0);
    slvm_end_func(vm);
    slvm_set_entry(vm, 0);
}

int main(void) {
    SLVM *vm = slvm_new();
    assert(vm != NULL);
    emit_file_info_program(vm);
    assert(slvm_set_osls_capabilities(vm, UINT64_C(2)) == -1);
    assert(slvm_set_osls_capabilities(vm, 0) == 0);
    assert(slvm_run(vm) == SLR_ERROR);
    assert(strstr(slvm_error(vm), "capability") != NULL);
    slvm_free(vm);

    /* A fresh VM receives the documented read-only default grant and can
     * execute the same real native function through OP_OSLS_CALL. */
    vm = slvm_new();
    assert(vm != NULL);
    emit_file_info_program(vm);
    assert(slvm_run(vm) != SLR_ERROR);
    assert(slvm_result(vm).type == SL_NULL);
    slvm_free(vm);

    puts("OSLS VM capability enforcement and teardown: PASS");
    return 0;
}
