#include "native_loader.h"
#include <string.h>

SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void) {
    return SL_NATIVE_ABI_VERSION;
}

SL_NATIVE_EXPORT uint32_t sl_native_typed_abi_version(void) {
    return SL_NATIVE_TYPED_ABI_VERSION;
}

SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]) {
    (void)argv;
    return argc;
}

SL_NATIVE_EXPORT int sl_native_call(const char *method, uint32_t argc,
                                    const SLNativeValue *argv,
                                    SLNativeValue *out_result) {
    if (!method || !out_result) return SL_NATIVE_CALL_ERROR;
    if (strcmp(method, "unhandled") == 0)
        return SL_NATIVE_CALL_NOT_HANDLED;
    if (strcmp(method, "sum") != 0 || argc != 2 || !argv)
        return SL_NATIVE_CALL_ERROR;
    if (argv[0].tag != SL_NATIVE_VALUE_I64 ||
        argv[1].tag != SL_NATIVE_VALUE_I64) return SL_NATIVE_CALL_ERROR;
    memset(out_result, 0, sizeof(*out_result));
    out_result->tag = SL_NATIVE_VALUE_I64;
    out_result->as.i64 = argv[0].as.i64 + argv[1].as.i64;
    return 0;
}
