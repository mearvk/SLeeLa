#include "../../../os-level-support/loader/native_loader.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

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

static int utf8_path(const SLNativeValue *value, char *out, size_t capacity) {
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
    char entered_path[1024], release_path[1024];
    struct timespec pause = {0, 1000000L};
    FILE *marker;
    if (!method || !out_result) return SL_NATIVE_CALL_INVALID_ARGUMENT;
    if (strcmp(method, "hold_until_released") != 0)
        return SL_NATIVE_CALL_NOT_HANDLED;
    if (argc != 2 || !argv ||
        !utf8_path(&argv[0], entered_path, sizeof(entered_path)) ||
        !utf8_path(&argv[1], release_path, sizeof(release_path)))
        return SL_NATIVE_CALL_INVALID_ARGUMENT;

    marker = fopen(entered_path, "wb");
    if (!marker) return SL_NATIVE_CALL_ERROR;
    fputs("entered", marker);
    fclose(marker);

    for (;;) {
        FILE *release = fopen(release_path, "rb");
        if (release) {
            fclose(release);
            break;
        }
        (void)nanosleep(&pause, NULL);
    }
    memset(out_result, 0, sizeof(*out_result));
    out_result->tag = SL_NATIVE_VALUE_I64;
    out_result->as.i64 = 73;
    return SL_NATIVE_CALL_OK;
}
