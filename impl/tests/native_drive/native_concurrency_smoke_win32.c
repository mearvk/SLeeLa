#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../core/sleela_core.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    SLVM *vm;
    SLNativeHandle handle;
    const char *entered_name;
    const char *release_name;
    HANDLE call_done;
    HANDLE close_started;
    HANDLE close_done;
    SLNativeStatus call_status;
    SLNativeStatus close_status;
    SLNativeValue result;
} TestState;

static int authorize_path(const char *path, void *context) {
    const char *expected = (const char *)context;
    return path && expected && strcmp(path, expected) == 0;
}

static DWORD WINAPI call_worker(LPVOID opaque) {
    TestState *state = (TestState *)opaque;
    SLNativeValue args[2];
    memset(args, 0, sizeof(args));
    args[0].tag = SL_NATIVE_VALUE_UTF8;
    args[0].as.utf8.data = (const uint8_t *)state->entered_name;
    args[0].as.utf8.length = (uint32_t)strlen(state->entered_name);
    args[1].tag = SL_NATIVE_VALUE_UTF8;
    args[1].as.utf8.data = (const uint8_t *)state->release_name;
    args[1].as.utf8.length = (uint32_t)strlen(state->release_name);
    state->call_status = slvm_native_module_call(
        state->vm, state->handle, "hold_until_released", 2, args, &state->result);
    SetEvent(state->call_done);
    return 0;
}

static DWORD WINAPI close_worker(LPVOID opaque) {
    TestState *state = (TestState *)opaque;
    SetEvent(state->close_started);
    state->close_status = slvm_native_module_close(state->vm, state->handle);
    SetEvent(state->close_done);
    return 0;
}

int main(int argc, char **argv) {
    char entered_name[128], release_name[128];
    LARGE_INTEGER stamp;
    HANDLE entered = NULL, release = NULL;
    HANDLE call_done = NULL, close_started = NULL, close_done = NULL;
    HANDLE caller = NULL, closer = NULL;
    SLVM *vm = NULL;
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    TestState state;
    SLNativeValue stale_result;
    SLNativeStatus stale_status = SL_NATIVE_INVALID_ARGUMENT;
    int ok = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: native_concurrency_smoke <plugin-path>\n");
        return 2;
    }
    memset(&state, 0, sizeof(state));
    QueryPerformanceCounter(&stamp);
    if (snprintf(entered_name, sizeof(entered_name),
                 "Local\\SLeeLaNativeEntered_%lu_%llu",
                 (unsigned long)GetCurrentProcessId(),
                 (unsigned long long)stamp.QuadPart) < 0 ||
        snprintf(release_name, sizeof(release_name),
                 "Local\\SLeeLaNativeRelease_%lu_%llu",
                 (unsigned long)GetCurrentProcessId(),
                 (unsigned long long)stamp.QuadPart) < 0)
        goto cleanup;

    entered = CreateEventA(NULL, TRUE, FALSE, entered_name);
    release = CreateEventA(NULL, TRUE, FALSE, release_name);
    call_done = CreateEventA(NULL, TRUE, FALSE, NULL);
    close_started = CreateEventA(NULL, TRUE, FALSE, NULL);
    close_done = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (!entered || !release || !call_done || !close_started || !close_done)
        goto cleanup;

    vm = slvm_new();
    if (!vm || slvm_set_native_authorizer(vm, authorize_path, argv[1]) != 0 ||
        slvm_native_module_open(vm, argv[1], &handle) != SL_NATIVE_OK)
        goto cleanup;

    state.vm = vm;
    state.handle = handle;
    state.entered_name = entered_name;
    state.release_name = release_name;
    state.call_done = call_done;
    state.close_started = close_started;
    state.close_done = close_done;

    caller = CreateThread(NULL, 0, call_worker, &state, 0, NULL);
    if (!caller) goto cleanup;
    if (WaitForSingleObject(entered, 5000) != WAIT_OBJECT_0) goto cleanup;

    closer = CreateThread(NULL, 0, close_worker, &state, 0, NULL);
    if (!closer) goto cleanup;
    if (WaitForSingleObject(close_started, 5000) != WAIT_OBJECT_0)
        goto cleanup;

    if (WaitForSingleObject(close_done, 75) != WAIT_TIMEOUT ||
        WaitForSingleObject(call_done, 0) != WAIT_TIMEOUT)
        goto cleanup;

    if (!SetEvent(release)) goto cleanup;
    if (WaitForSingleObject(caller, 5000) != WAIT_OBJECT_0 ||
        WaitForSingleObject(closer, 5000) != WAIT_OBJECT_0)
        goto cleanup;

    memset(&stale_result, 0, sizeof(stale_result));
    stale_status = slvm_native_module_call(
        vm, handle, "hold_until_released", 0, NULL, &stale_result);
    ok = state.call_status == SL_NATIVE_OK &&
         state.result.tag == SL_NATIVE_VALUE_I64 &&
         state.result.as.i64 == 73 &&
         state.close_status == SL_NATIVE_OK &&
         stale_status == SL_NATIVE_STALE_HANDLE;
cleanup:
    /* Always release a blocked plugin before joining or destroying its VM. */
    if (release) SetEvent(release);
    if (caller) WaitForSingleObject(caller, INFINITE);
    if (closer) WaitForSingleObject(closer, INFINITE);
    if (vm) {
        if (handle != SL_NATIVE_HANDLE_INVALID)
            (void)slvm_native_module_close(vm, handle);
        slvm_free(vm);
    }
    if (caller) CloseHandle(caller);
    if (closer) CloseHandle(closer);
    if (entered) CloseHandle(entered);
    if (release) CloseHandle(release);
    if (call_done) CloseHandle(call_done);
    if (close_started) CloseHandle(close_started);
    if (close_done) CloseHandle(close_done);

    if (!ok) {
        fprintf(stderr, "Windows native call/close serialization regression: FAIL\n");
        return 1;
    }
    puts("Windows native call remains loaded while close waits; close invalidates handle afterward: PASS");
    return 0;
}
