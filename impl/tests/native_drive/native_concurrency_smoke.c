#define _POSIX_C_SOURCE 200809L
#include "../../core/sleela_core.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static void release_plugin(int fd) {
    if (fd >= 0) {
        (void)lseek(fd, 0, SEEK_SET);
        (void)write(fd, "x", 1);
    }
}

typedef struct {
    SLVM *vm;
    SLNativeHandle handle;
    const char *entered_path;
    const char *release_path;
    atomic_int call_done;
    atomic_int close_started;
    atomic_int close_done;
    SLNativeStatus call_status;
    SLNativeStatus close_status;
    SLNativeValue result;
} TestState;

static int authorize_path(const char *path, void *context) {
    const char *expected = (const char *)context;
    return path && expected && strcmp(path, expected) == 0;
}

static void *call_worker(void *opaque) {
    TestState *state = (TestState *)opaque;
    SLNativeValue args[2];
    memset(args, 0, sizeof(args));
    args[0].tag = SL_NATIVE_VALUE_UTF8;
    args[0].as.utf8.data = (const uint8_t *)state->entered_path;
    args[0].as.utf8.length = (uint32_t)strlen(state->entered_path);
    args[1].tag = SL_NATIVE_VALUE_UTF8;
    args[1].as.utf8.data = (const uint8_t *)state->release_path;
    args[1].as.utf8.length = (uint32_t)strlen(state->release_path);
    state->call_status = slvm_native_module_call(
        state->vm, state->handle, "hold_until_released", 2, args, &state->result);
    atomic_store(&state->call_done, 1);
    return NULL;
}

static void *close_worker(void *opaque) {
    TestState *state = (TestState *)opaque;
    atomic_store(&state->close_started, 1);
    state->close_status = slvm_native_module_close(state->vm, state->handle);
    atomic_store(&state->close_done, 1);
    return NULL;
}

static int wait_for_file(const char *path) {
    struct timespec pause = {0, 1000000L};
    int i;
    for (i = 0; i < 5000; ++i) {
        if (access(path, F_OK) == 0) return 1;
        (void)nanosleep(&pause, NULL);
    }
    return 0;
}

int main(int argc, char **argv) {
    char entered_path[] = "/tmp/sleela-native-entered-XXXXXX";
    char release_path[] = "/tmp/sleela-native-release-XXXXXX";
    int entered_fd = -1, release_fd = -1, ok = 0;
    SLVM *vm = NULL;
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    TestState state;
    pthread_t caller, closer;
    struct timespec observation = {0, 50000000L};
    SLNativeValue stale_result;
    SLNativeStatus stale_status;

    if (argc != 2) {
        fprintf(stderr, "usage: native_concurrency_smoke <plugin-path>\n");
        return 2;
    }
    memset(&state, 0, sizeof(state));
    entered_fd = mkstemp(entered_path);
    release_fd = mkstemp(release_path);
    if (entered_fd < 0 || release_fd < 0) goto cleanup;
    close(entered_fd); entered_fd = -1;
    unlink(entered_path);

    vm = slvm_new();
    if (!vm || slvm_set_native_authorizer(vm, authorize_path, argv[1]) != 0 ||
        slvm_native_module_open(vm, argv[1], &handle) != SL_NATIVE_OK)
        goto cleanup;

    state.vm = vm;
    state.handle = handle;
    state.entered_path = entered_path;
    state.release_path = release_path;
    atomic_init(&state.call_done, 0);
    atomic_init(&state.close_started, 0);
    atomic_init(&state.close_done, 0);

    if (pthread_create(&caller, NULL, call_worker, &state) != 0) goto cleanup;
    if (!wait_for_file(entered_path)) {
        release_plugin(release_fd);
        (void)pthread_join(caller, NULL);
        goto cleanup;
    }
    if (pthread_create(&closer, NULL, close_worker, &state) != 0) {
        release_plugin(release_fd);
        (void)pthread_join(caller, NULL);
        goto cleanup;
    }

    while (!atomic_load(&state.close_started)) (void)nanosleep(&observation, NULL);
    (void)nanosleep(&observation, NULL);
    if (atomic_load(&state.close_done) || atomic_load(&state.call_done)) {
        release_plugin(release_fd);
        (void)pthread_join(caller, NULL);
        (void)pthread_join(closer, NULL);
        goto cleanup;
    }

    release_plugin(release_fd);
    (void)pthread_join(caller, NULL);
    (void)pthread_join(closer, NULL);

    memset(&stale_result, 0, sizeof(stale_result));
    stale_status = slvm_native_module_call(
        vm, handle, "hold_until_released", 0, NULL, &stale_result);
    ok = state.call_status == SL_NATIVE_OK &&
         state.result.tag == SL_NATIVE_VALUE_I64 &&
         state.result.as.i64 == 73 &&
         state.close_status == SL_NATIVE_OK &&
         stale_status == SL_NATIVE_STALE_HANDLE &&
         atomic_load(&state.call_done) && atomic_load(&state.close_done);
cleanup:
    if (entered_fd >= 0) close(entered_fd);
    if (release_fd >= 0) close(release_fd);
    unlink(entered_path);
    unlink(release_path);
    if (vm) {
        if (handle != SL_NATIVE_HANDLE_INVALID && slvm_native_module_close(vm, handle) == SL_NATIVE_OK) {
            /* The normal path already closed it; this is only cleanup. */
        }
        slvm_free(vm);
    }
    if (!ok) {
        fprintf(stderr, "native call/close serialization regression: FAIL\n");
        return 1;
    }
    puts("native call remains loaded while close waits; close invalidates handle afterward: PASS");
    return 0;
}
