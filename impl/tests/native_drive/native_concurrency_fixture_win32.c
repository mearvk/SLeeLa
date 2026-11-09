#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../../os-level-support/loader/native_loader.h"

#include <stdio.h>
#include <string.h>

SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void) {
    return SL_NATIVE_ABI_VERSION;
}
SL_NATIVE_EXPORT uint32_t sl_native_typed_abi_version(void) {
    return SL_NATIVE_TYPED_ABI_VERSION;
}
SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]) {
    (void)argc; (void)argv;
    return 0;
}

static int utf8_event_name(const SLNativeValue *value, char *out, size_t capacity) {
    size_t n;
    if (!value || value->tag != SL_NATIVE_VALUE_UTF8 ||
        (!value->as.utf8.data && value->as.utf8.length)) return 0;
    n = value->as.utf8.length;
    if (!n || n >= capacity || memchr(value->as.utf8.data, '\0', n)) return 0;
    memcpy(out, value->as.utf8.data, n);
    out[n] = '\0';
    return 1;
}

SL_NATIVE_EXPORT int sl_native_call(const char *method, uint32_t argc,
                                    const SLNativeValue *argv,
                                    SLNativeValue *out_result) {
    char entered_name[128], release_name[128];
    HANDLE entered = NULL, release = NULL;
    DWORD wait_result;
    if (!method || !out_result) return SL_NATIVE_CALL_INVALID_ARGUMENT;
    if (strcmp(method, "hold_until_released") != 0)
        return SL_NATIVE_CALL_NOT_HANDLED;
    if (argc != 2 || !argv ||
        !utf8_event_name(&argv[0], entered_name, sizeof(entered_name)) ||
        !utf8_event_name(&argv[1], release_name, sizeof(release_name)))
        return SL_NATIVE_CALL_INVALID_ARGUMENT;

    entered = OpenEventA(EVENT_MODIFY_STATE, FALSE, entered_name);
    release = OpenEventA(SYNCHRONIZE, FALSE, release_name);
    if (!entered || !release) {
        if (entered) CloseHandle(entered);
        if (release) CloseHandle(release);
        return SL_NATIVE_CALL_ERROR;
    }
    if (!SetEvent(entered)) {
        CloseHandle(entered);
        CloseHandle(release);
        return SL_NATIVE_CALL_ERROR;
    }
    wait_result = WaitForSingleObject(release, INFINITE);
    CloseHandle(entered);
    CloseHandle(release);
    if (wait_result != WAIT_OBJECT_0) return SL_NATIVE_CALL_ERROR;

    memset(out_result, 0, sizeof(*out_result));
    out_result->tag = SL_NATIVE_VALUE_I64;
    out_result->as.i64 = 73;
    return SL_NATIVE_CALL_OK;
}
