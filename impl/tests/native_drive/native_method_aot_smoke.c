#include "../../core/sleela_core.h"
#include <stdio.h>
#include <string.h>

static int authorize_exact_path(const char *path, void *context) {
    const char *approved = (const char *)context;
    return path && approved && strcmp(path, approved) == 0;
}

int main(int argc, char **argv) {
    SLVM *vm = NULL;
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    SLValue result;
    uint32_t flags = 0, abi = 0;
    uint64_t identity = 0;
    const char *signature = NULL;
    int bound[4] = {0, 0, 0, 0};
    int ok = 0;
    if (argc != 3) {
        fprintf(stderr, "usage: native_method_aot_smoke <artifact> <plugin>\n");
        return 2;
    }
    vm = slvm_load_file(argv[1]);
    if (!vm) goto done;
    if (slvm_get_func_native_metadata(vm, 0, &flags, &abi,
                                      &identity, &signature) != 1 ||
        flags != (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) ||
        abi != SL_NATIVE_METHOD_ABI_VERSION || !identity ||
        !signature ||
        strcmp(signature, "NativeAotFixture::sum|static|argc=2|3:int3:int|return=3:int|abi=1.0") != 0)
        goto done;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, argv[2]) != 0 ||
        slvm_native_module_open(vm, argv[2], &handle) != SL_NATIVE_OK)
        goto done;
    for (int i = 0; i < 4; ++i) {
        if (slvm_bind_native_method(vm, i, handle) != 0) goto done;
        bound[i] = 1;
    }
    if (slvm_native_module_close(vm, handle) == SL_NATIVE_OK) goto done;
    if (slvm_run(vm) != SLR_OK) goto done;
    result = slvm_result(vm);
    if (result.type != SL_INT || result.as.i != 42) goto done;
    for (int i = 0; i < 4; ++i) {
        if (slvm_unbind_native_method(vm, i) != 0) goto done;
        bound[i] = 0;
    }
    if (slvm_native_module_close(vm, handle) != SL_NATIVE_OK) goto done;
    handle = SL_NATIVE_HANDLE_INVALID;
    ok = 1;
done:
    if (vm && handle != SL_NATIVE_HANDLE_INVALID) {
        for (int i = 0; i < 4; ++i) if (bound[i]) (void)slvm_unbind_native_method(vm, i);
        (void)slvm_native_module_close(vm, handle);
    }
    if (vm) slvm_free(vm);
    if (!ok) {
        fprintf(stderr, "generated @native plugin load/bind/ordinary-call dispatch failed\n");
        return 1;
    }
    puts("generated @native plugin: PASS (int64, double, bool, UTF-8, ordinary OP_CALL, authorized binding)");
    return 0;
}
