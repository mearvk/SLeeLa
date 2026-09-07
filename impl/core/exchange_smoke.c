/* ==========================================================================
 * exchange_smoke.c  --  Proves the C core is fully drivable through the single
 * slcore_exchange() dispatch symbol, with no direct builder-helper calls.
 *
 * It assembles and runs:   main() { print(6 * 7); }
 * expecting the core to print 42.
 * ========================================================================== */
#include "sleela_core.h"
#include <stdio.h>

static int emit(SLVM* vm, SLOp op, int a) {
    SLExchangeArg x = {0}; x.op = op; x.a = a;
    slcore_exchange(vm, SLX_EMIT, &x);
    return x.out;
}

int main(void) {
    SLVM* vm = slvm_new();
    SLExchangeArg x;

    /* constants 6 and 7 via exchange */
    x = (SLExchangeArg){0}; x.value = slval_int(6);
    slcore_exchange(vm, SLX_ADD_CONST, &x); int c6 = x.out;
    x = (SLExchangeArg){0}; x.value = slval_int(7);
    slcore_exchange(vm, SLX_ADD_CONST, &x); int c7 = x.out;

    /* begin main() with 0 args, 0 locals */
    x = (SLExchangeArg){0}; x.name = "main"; x.i0 = 0; x.i1 = 0;
    slcore_exchange(vm, SLX_BEGIN_FUNC, &x); int fmain = x.out;

    emit(vm, OP_CONST, c6);
    emit(vm, OP_CONST, c7);
    emit(vm, OP_MUL, 0);
    emit(vm, OP_PRINT, 0);
    emit(vm, OP_CONST, c6);   /* dummy return value */
    emit(vm, OP_RET, 0);

    x = (SLExchangeArg){0};
    slcore_exchange(vm, SLX_END_FUNC, &x);

    x = (SLExchangeArg){0}; x.a = fmain;
    slcore_exchange(vm, SLX_SET_ENTRY, &x);

    x = (SLExchangeArg){0};
    SLResult r = slcore_exchange(vm, SLX_RUN, &x);

    printf("exchange run result code = %d\n", (int)r);
    slvm_free(vm);
    return 0;
}
