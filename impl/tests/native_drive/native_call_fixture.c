#include "../../../os-level-support/loader/native_loader.h"
#include "../../core/sleela_utf16.h"
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
        api->struct_size < sizeof(SLNativeHostAPI) || !api->invoke) return -1;
    host_api = *api; host_api_bound = 1; return 0;
}
SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void) { return SL_NATIVE_ABI_VERSION; }
SL_NATIVE_EXPORT uint32_t sl_native_typed_abi_version(void) { return SL_NATIVE_TYPED_ABI_VERSION; }
SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]) {
    SLNativeValue callback_arg, callback_result;
    (void)argc; (void)argv;
    if (!host_api_bound || !host_api.invoke) return -1;
    memset(&callback_arg, 0, sizeof(callback_arg));
    memset(&callback_result, 0, sizeof(callback_result));
    callback_arg.tag = SL_NATIVE_VALUE_I64;
    callback_arg.as.i64 = 41;
    /* Host-side module_run has no suspended SLeeLa call frame. The callback
     * must be refused by the VM guard, and the plugin must observe that refusal. */
    return host_api.invoke(host_api.context, "Callback::receive", 1,
                           &callback_arg, &callback_result) ? 0 : -1;
}
static int utf8_argument(uint32_t argc, const SLNativeValue *argv,
                         const char **data, size_t *length) {
    if (argc < 1 || !argv || argv[0].tag != SL_NATIVE_VALUE_UTF8 ||
        (!argv[0].as.utf8.data && argv[0].as.utf8.length != 0)) return 0;
    *data = (const char *)argv[0].as.utf8.data;
    *length = (size_t)argv[0].as.utf8.length; return 1;
}
SL_NATIVE_EXPORT int sl_native_call(const char *method, uint32_t argc,
                                    const SLNativeValue *argv, SLNativeValue *out_result) {
    if (strcmp(method, "utf16_length") == 0) {
        const char *data; size_t length; int64_t units = 0;
        int rc;
        if (argc != 1 || !utf8_argument(argc, argv, &data, &length))
            return SL_NATIVE_CALL_INVALID_ARGUMENT;
        rc = sleela_utf8_utf16_length(data, length, &units);
        if (rc == -2) return SL_NATIVE_CALL_MALFORMED_TEXT;
        if (rc != 0) return SL_NATIVE_CALL_INVALID_ARGUMENT;
        memset(out_result, 0, sizeof(*out_result));
        out_result->tag = SL_NATIVE_VALUE_I64; out_result->as.i64 = units;
        return SL_NATIVE_CALL_OK;
    }
    if (strcmp(method, "utf16_char_at") == 0) {
        const char *data; size_t length; uint16_t unit = 0;
        int rc;
        if (argc != 2 || !utf8_argument(argc, argv, &data, &length) ||
            argv[1].tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_INVALID_ARGUMENT;
        rc = sleela_utf8_utf16_char_at(data, length, argv[1].as.i64, &unit);
        if (rc == -2) return SL_NATIVE_CALL_MALFORMED_TEXT;
        if (rc == -1 || rc == -3) return SL_NATIVE_CALL_INDEX_OUT_OF_BOUNDS;
        if (rc != 0) return SL_NATIVE_CALL_INVALID_ARGUMENT;
        memset(out_result, 0, sizeof(*out_result));
        out_result->tag = SL_NATIVE_VALUE_I64; out_result->as.i64 = (int64_t)unit;
        return SL_NATIVE_CALL_OK;
    }
    if (strcmp(method, "call_sleela") == 0) {
        SLNativeValue callback_arg, callback_result;
        if (!host_api_bound || !host_api.invoke || argc != 1 || !argv ||
            argv[0].tag != SL_NATIVE_VALUE_I64) return SL_NATIVE_CALL_ERROR;
        callback_arg = argv[0]; memset(&callback_result, 0, sizeof(callback_result));
        if (!host_api.invoke(host_api.context, "Callback::receive", 1,
                             &callback_arg, &callback_result) ||
            callback_result.tag != SL_NATIVE_VALUE_I64) return SL_NATIVE_CALL_ERROR;
        *out_result = callback_result; return SL_NATIVE_CALL_OK;
    }
    if (strcmp(method, "call_sleela_open") == 0 ||
        strcmp(method, "call_sleela_close") == 0) {
        const char *callback_name = strcmp(method, "call_sleela_open") == 0
            ? "Callback::receive_open" : "Callback::receive_close";
        SLNativeValue callback_arg, callback_result;
        if (!host_api_bound || !host_api.invoke || argc != 1 || !argv ||
            argv[0].tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_ERROR;
        callback_arg = argv[0];
        memset(&callback_result, 0, sizeof(callback_result));
        if (!host_api.invoke(host_api.context, callback_name, 1,
                             &callback_arg, &callback_result)) {
            memset(out_result, 0, sizeof(*out_result));
            out_result->tag = SL_NATIVE_VALUE_I64;
            out_result->as.i64 = -1;
            return SL_NATIVE_CALL_OK;
        }
        if (callback_result.tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_ERROR;
        *out_result = callback_result;
        return SL_NATIVE_CALL_OK;
    }
    if (strcmp(method, "call_sleela_nested") == 0) {
        SLNativeValue callback_args[2], callback_result;
        if (!host_api_bound || !host_api.invoke || argc != 2 || !argv ||
            argv[0].tag != SL_NATIVE_VALUE_I64 ||
            argv[1].tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_ERROR;
        callback_args[0] = argv[0];
        callback_args[1] = argv[1];
        memset(&callback_result, 0, sizeof(callback_result));
        if (!host_api.invoke(host_api.context, "Callback::receive_nested", 2,
                             callback_args, &callback_result)) {
            /* The nested nativeCall is expected to be rejected by the VM.
             * Return a sentinel successfully so the harness can inspect the
             * precise VM diagnostic rather than an outer plugin error. */
            memset(out_result, 0, sizeof(*out_result));
            out_result->tag = SL_NATIVE_VALUE_I64;
            out_result->as.i64 = -1;
            return SL_NATIVE_CALL_OK;
        }
        if (callback_result.tag != SL_NATIVE_VALUE_I64)
            return SL_NATIVE_CALL_ERROR;
        *out_result = callback_result;
        return SL_NATIVE_CALL_OK;
    }
    if (strcmp(method, "twice") != 0) return SL_NATIVE_CALL_NOT_HANDLED;
    if (argc != 1 || !argv || argv[0].tag != SL_NATIVE_VALUE_I64) return SL_NATIVE_CALL_ERROR;
    memset(out_result, 0, sizeof(*out_result));
    out_result->tag = SL_NATIVE_VALUE_I64; out_result->as.i64 = argv[0].as.i64 * 2;
    return SL_NATIVE_CALL_OK;
}
