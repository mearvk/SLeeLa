#include "../core/sleela_core.h"
#include <stdio.h>
#include <string.h>

static int authorize_exact_path(const char *path, void *context) {
    const char *approved = (const char *)context;
    return path && approved && strcmp(path, approved) == 0;
}

static int emit(SLVM *vm, SLOp op, int operand) {
    return slvm_emit(vm, op, operand) >= 0;
}

int main(int argc, char **argv) {
    SLVM *vm;
    int path_id, twice_id, fallback_id, twenty_one_id, seven_id, three_id;
    int main_function;
    SLValue result;
    if (argc != 2) {
        fprintf(stderr, "usage: native_call_smoke <trusted-plugin-path>\\n");
        return 2;
    }
    vm = slvm_new();
    if (!vm) return 2;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, argv[1]) != 0) {
        fprintf(stderr, "could not install native authorization callback\\n");
        slvm_free(vm);
        return 2;
    }

    path_id = slvm_add_const_str(vm, argv[1]);
    twice_id = slvm_add_const_str(vm, "twice");
    fallback_id = slvm_add_const_str(vm, "fallback");
    twenty_one_id = slvm_add_const_int(vm, 21);
    seven_id = slvm_add_const_int(vm, 7);
    three_id = slvm_add_const_int(vm, 3);

    main_function = slvm_begin_func(vm, "main", 0, 3);
    if (main_function < 0 ||
        !emit(vm, OP_CONST, path_id) || !emit(vm, OP_NATIVE_OPEN, 0) ||
        !emit(vm, OP_STOREL, 0) ||
        !emit(vm, OP_LOADL, 0) || !emit(vm, OP_CONST, twice_id) ||
        !emit(vm, OP_CONST, twenty_one_id) || !emit(vm, OP_NATIVE_CALL, 1) ||
        !emit(vm, OP_STOREL, 1) ||
        !emit(vm, OP_LOADL, 0) || !emit(vm, OP_CONST, fallback_id) ||
        !emit(vm, OP_CONST, seven_id) || !emit(vm, OP_NATIVE_CALL, 1) ||
        !emit(vm, OP_STOREL, 2) ||
        !emit(vm, OP_LOADL, 0) || !emit(vm, OP_NATIVE_CLOSE, 0) ||
        !emit(vm, OP_POP, 0) ||
        !emit(vm, OP_LOADL, 1) || !emit(vm, OP_LOADL, 2) ||
        !emit(vm, OP_ADD, 0) || !emit(vm, OP_RET, 0)) {
        fprintf(stderr, "could not assemble native-call main function\\n");
        slvm_free(vm);
        return 2;
    }
    slvm_end_func(vm);

    if (slvm_begin_func(vm, "fallback", 1, 1) < 0 ||
        !emit(vm, OP_LOADL, 0) || !emit(vm, OP_CONST, three_id) ||
        !emit(vm, OP_MUL, 0) || !emit(vm, OP_RET, 0)) {
        fprintf(stderr, "could not assemble bytecode fallback\\n");
        slvm_free(vm);
        return 2;
    }
    slvm_end_func(vm);
    slvm_set_entry(vm, main_function);

    if (slvm_run(vm) != SLR_OK) {
        fprintf(stderr, "native-call execution failed: %s\\n",
                slvm_error(vm) ? slvm_error(vm) : "unknown VM error");
        slvm_free(vm);
        return 1;
    }
    result = slvm_result(vm);
    slvm_free(vm);
    if (result.type != SL_INT || result.as.i != 63) {
        fprintf(stderr, "expected 63 from native dispatch + bytecode fallback\\n");
        return 1;
    }
    puts("native-call dispatch and interpreter fallback: PASS");
    return 0;
}
