#include "sleela_core.h"
#include <stdio.h>
#include <string.h>

static int check_cross_frame_catch(void) {
    SLVM *vm = slvm_new();
    int value, thrower, main_function, handler_push, catch_ip;
    SLResult status;
    SLValue result;
    int ok = 0;
    if (!vm) return 0;

    value = slvm_add_const_int(vm, 73);
    if (value < 0) goto done;

    thrower = slvm_begin_func(vm, "thrower", 0, 0);
    slvm_emit(vm, OP_CONST, value);
    slvm_emit0(vm, OP_THROW);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);

    main_function = slvm_begin_func(vm, "main", 0, 1);
    handler_push = slvm_emit(vm, OP_TRY_PUSH, 0);
    slvm_emit(vm, OP_CALL, thrower);
    slvm_emit0(vm, OP_TRY_POP);
    slvm_emit(vm, OP_CONST, value);
    slvm_emit0(vm, OP_RET);
    catch_ip = slvm_here(vm);
    slvm_emit(vm, OP_STOREL, 0);
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit0(vm, OP_RET);
    slvm_patch(vm, handler_push, catch_ip);
    slvm_end_func(vm);
    slvm_set_entry(vm, main_function);

    status = slvm_run(vm);
    result = slvm_result(vm);
    ok = status == SLR_OK && result.type == SL_INT && result.as.i == 73;

done:
    slvm_free(vm);
    return ok;
}

static int check_uncaught_payload(void) {
    SLVM *vm = slvm_new();
    int value, main_function;
    SLResult status;
    SLValue exception_value;
    int ok = 0;
    if (!vm) return 0;

    value = slvm_add_const_bool(vm, 0);
    if (value < 0) goto done;
    main_function = slvm_begin_func(vm, "main", 0, 0);
    slvm_emit(vm, OP_CONST, value);
    slvm_emit0(vm, OP_THROW);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);
    slvm_set_entry(vm, main_function);

    status = slvm_run(vm);
    ok = status == SLR_ERROR &&
         slvm_uncaught_exception(vm, &exception_value) &&
         exception_value.type == SL_BOOL && exception_value.as.b == 0 &&
         slvm_error(vm) &&
         strcmp(slvm_error(vm), "uncaught language exception") == 0;

done:
    slvm_free(vm);
    return ok;
}

int main(void) {
    if (!check_cross_frame_catch()) {
        fprintf(stderr, "exception VM smoke: cross-frame catch failed\n");
        return 1;
    }
    if (!check_uncaught_payload()) {
        fprintf(stderr, "exception VM smoke: uncaught payload failed\n");
        return 1;
    }
    puts("exception VM smoke: PASS");
    return 0;
}
