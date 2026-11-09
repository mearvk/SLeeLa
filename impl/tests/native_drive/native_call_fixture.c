#include "../../../os-level-support/loader/native_loader.h"
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
    if (strcmp(method, "twice") != 0)
        return SL_NATIVE_CALL_NOT_HANDLED;
    if (argc != 1 || !argv || argv[0].tag != SL_NATIVE_VALUE_I64)
        return SL_NATIVE_CALL_ERROR;
    memset(out_result, 0, sizeof(*out_result));
    out_result->tag = SL_NATIVE_VALUE_I64;
    out_result->as.i64 = argv[0].as.i64 * 2;
    return SL_NATIVE_CALL_OK;
}
