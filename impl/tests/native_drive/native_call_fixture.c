#include "../../../os-level-support/loader/native_loader.h"
#include <string.h>

#if defined(_MSC_VER)
#  define FIXTURE_THREAD_LOCAL __declspec(thread)
#else
#  define FIXTURE_THREAD_LOCAL _Thread_local
#endif
static FIXTURE_THREAD_LOCAL SLNativeHostAPI host_api;
static FIXTURE_THREAD_LOCAL int host_api_bound;

SL_NATIVE_EXPORT int sl_native_bind_host(const SLNativeHostAPI *api) {
    if (!api || api->abi_version != SL_NATIVE_HOST_CALLBACK_ABI_VERSION ||
        api->struct_size < sizeof(SLNativeHostAPI) || !api->invoke)
        return -1;
    host_api = *api;
    host_api_bound = 1;
    return 0;
}

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
    if (strcmp(method, "call_sleela") == 0) {
        SLNativeValue callback_arg, callback_result;
        if (!host_api_bound || !host_api.invoke || argc != 1 || !argv ||
            argv[0].tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_ERROR;
        callback_arg = argv[0];
        memset(&callback_result, 0, sizeof(callback_result));
        if (!host_api.invoke(host_api.context, "Callback::receive", 1,
                             &callback_arg, &callback_result))
            return SL_NATIVE_CALL_ERROR;
        if (callback_result.tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_ERROR;
        *out_result = callback_result;
        return SL_NATIVE_CALL_OK;
    }
    if (strcmp(method, "twice") != 0)
        return SL_NATIVE_CALL_NOT_HANDLED;
    if (argc != 1 || !argv || argv[0].tag != SL_NATIVE_VALUE_I64)
        return SL_NATIVE_CALL_ERROR;
    memset(out_result, 0, sizeof(*out_result));
    out_result->tag = SL_NATIVE_VALUE_I64;
    out_result->as.i64 = argv[0].as.i64 * 2;
    return SL_NATIVE_CALL_OK;
}
