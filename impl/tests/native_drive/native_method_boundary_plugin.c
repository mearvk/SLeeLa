#include "native_loader.h"
#include <string.h>

SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void) {
    return SL_NATIVE_ABI_VERSION;
}

SL_NATIVE_EXPORT uint32_t sl_native_typed_abi_version(void) {
    return SL_NATIVE_TYPED_ABI_VERSION;
}

SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]) {
    (void)argc;
    (void)argv;
    return 0;
}

SL_NATIVE_EXPORT int sl_native_call(const char *method, uint32_t argc,
                                    const SLNativeValue *argv,
                                    SLNativeValue *out_result) {
    (void)argv;
    if (!method || !out_result) return SL_NATIVE_CALL_INVALID_ARGUMENT;

    /* If a VM composite ever crosses the boundary, return a conspicuous
       sentinel so the host smoke test fails instead of masking the violation. */
    if (strcmp(method, "NativeBoundaryFixture::arrayTotal|static|argc=1|5:int[]|return=3:int|abi=1.0") == 0 ||
        strcmp(method, "NativeBoundaryFixture::pointTotal|static|argc=1|5:Point|return=3:int|abi=1.0") == 0) {
        if (argc != 1) return SL_NATIVE_CALL_INVALID_ARGUMENT;
        out_result->tag = SL_NATIVE_VALUE_I64;
        out_result->as.i64 = 9001;
        return SL_NATIVE_CALL_OK;
    }

    /* A handled error is not a request for bytecode fallback. */
    if (strcmp(method, "NativeErrorFixture::fail|static|argc=1|3:int|return=3:int|abi=1.0") == 0) {
        if (argc != 1 || !argv || argv[0].tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_INVALID_ARGUMENT;
        return SL_NATIVE_CALL_ERROR;
    }

    return SL_NATIVE_CALL_NOT_HANDLED;
}
