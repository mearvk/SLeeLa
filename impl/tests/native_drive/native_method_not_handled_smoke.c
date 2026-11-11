#include "sleela_core.h"
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
    int bound = 0, ok = 0;
    if (argc != 3) {
        fprintf(stderr, "usage: native_method_not_handled_smoke <artifact> <plugin>\n");
        return 2;
    }
    vm = slvm_load_file_with_entry(argv[1], "NativeDeclineFixture::check");
    if (!vm) goto done;
    if (slvm_get_func_native_metadata(vm, 0, &flags, &abi,
                                      &identity, &signature) != 1 ||
        flags != (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) ||
        abi != SL_NATIVE_METHOD_ABI_VERSION || !identity || !signature ||
        strcmp(signature, "NativeDeclineFixture::fallback|static|argc=1|3:int|return=3:int|abi=1.0") != 0)
        goto done;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, argv[2]) != 0 ||
        slvm_native_module_open(vm, argv[2], &handle) != SL_NATIVE_OK ||
        slvm_bind_native_method(vm, 0, handle) != 0)
        goto done;
    bound = 1;
    if (slvm_native_module_close(vm, handle) == SL_NATIVE_OK) goto done;
    if (slvm_run(vm) != SLR_OK) goto done;
    result = slvm_result(vm);
    /* fallbackCalls must advance 0->1->2. A duplicated bytecode fallback
       would instead return 24 (2 then 4), not 12. */
    if (result.type != SL_INT || result.as.i != 12) goto done;
    if (slvm_unbind_native_method(vm, 0) != 0) goto done;
    bound = 0;
    if (slvm_native_module_close(vm, handle) != SL_NATIVE_OK) goto done;
    handle = SL_NATIVE_HANDLE_INVALID;
    ok = 1;
done:
    if (vm && handle != SL_NATIVE_HANDLE_INVALID) {
        if (bound) (void)slvm_unbind_native_method(vm, 0);
        (void)slvm_native_module_close(vm, handle);
    }
    if (vm) slvm_free(vm);
    if (!ok) {
        fprintf(stderr, "NOT_HANDLED did not execute bytecode fallback exactly once per call\n");
        return 1;
    }
    puts("native @native NOT_HANDLED fallback: PASS (two calls, exactly one bytecode execution each)");
    return 0;
}
