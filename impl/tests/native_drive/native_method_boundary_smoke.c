#include "../../core/sleela_core.h"
#include <stdio.h>
#include <string.h>

static int authorize_exact_path(const char *path, void *context) {
    const char *approved = (const char *)context;
    return path && approved && strcmp(path, approved) == 0;
}

static int deny_all_paths(const char *path, void *context) {
    (void)path;
    (void)context;
    return 0;
}

static int bind_plugin(SLVM *vm, const char *plugin, SLNativeHandle *handle) {
    return slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin) == 0 &&
           slvm_native_module_open(vm, plugin, handle) == SL_NATIVE_OK;
}

static int check_composite_fallback(const char *artifact, const char *plugin) {
    SLVM *vm = slvm_load_file(artifact);
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    SLValue result;
    uint32_t flags = 0, abi = 0;
    uint64_t identity = 0;
    const char *signature = NULL;
    int bound[2] = {0, 0}, ok = 0;
    if (!vm) return 0;
    if (slvm_get_func_native_metadata(vm, 0, &flags, &abi, &identity, &signature) != 1 ||
        flags != (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) ||
        abi != SL_NATIVE_METHOD_ABI_VERSION || !identity || !signature ||
        strcmp(signature, "NativeBoundaryFixture::arrayTotal|static|argc=1|5:int[]|return=3:int|abi=1.0") != 0)
        goto done;
    if (slvm_get_func_native_metadata(vm, 1, &flags, &abi, &identity, &signature) != 1 ||
        flags != (SL_NATIVE_METHOD_FLAG_ANNOTATED | SL_NATIVE_METHOD_FLAG_STATIC) ||
        abi != SL_NATIVE_METHOD_ABI_VERSION || !identity || !signature ||
        strcmp(signature, "NativeBoundaryFixture::pointTotal|static|argc=1|5:Point|return=3:int|abi=1.0") != 0)
        goto done;
    if (!bind_plugin(vm, plugin, &handle) ||
        slvm_bind_native_method(vm, 0, handle) != 0 ||
        slvm_bind_native_method(vm, 1, handle) != 0)
        goto done;
    bound[0] = bound[1] = 1;
    if (slvm_run(vm) != SLR_OK) goto done;
    result = slvm_result(vm);
    /* 10+20 + 5+7 = 42. The plugin returns 9001 if either composite
       argument is incorrectly marshalled across the native boundary. */
    if (result.type != SL_INT || result.as.i != 42) goto done;
    ok = 1;
done:
    if (handle != SL_NATIVE_HANDLE_INVALID) {
        if (bound[0]) (void)slvm_unbind_native_method(vm, 0);
        if (bound[1]) (void)slvm_unbind_native_method(vm, 1);
        (void)slvm_native_module_close(vm, handle);
    }
    slvm_free(vm);
    return ok;
}

static int check_plugin_error_is_not_fallback(const char *artifact, const char *plugin) {
    SLVM *vm = slvm_load_file(artifact);
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    int bound = 0, ok = 0;
    if (!vm) return 0;
    if (!bind_plugin(vm, plugin, &handle) ||
        slvm_bind_native_method(vm, 0, handle) != 0)
        goto done;
    bound = 1;
    /* The plugin returns CALL_ERROR for this handled method. If the VM
       incorrectly falls back, the bytecode method returns successfully. */
    if (slvm_run(vm) == SLR_OK) goto done;
    if (!slvm_error(vm) || !strstr(slvm_error(vm), "failed")) goto done;
    ok = 1;
done:
    if (handle != SL_NATIVE_HANDLE_INVALID) {
        if (bound) (void)slvm_unbind_native_method(vm, 0);
        (void)slvm_native_module_close(vm, handle);
    }
    slvm_free(vm);
    return ok;
}

static int check_denied_path_cannot_bind(const char *artifact, const char *plugin) {
    SLVM *vm = slvm_load_file(artifact);
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    SLValue result;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_set_native_authorizer(vm, deny_all_paths, NULL) != 0 ||
        slvm_native_module_open(vm, plugin, &handle) != SL_NATIVE_AUTHORIZATION_DENIED ||
        handle != SL_NATIVE_HANDLE_INVALID ||
        slvm_bind_native_method(vm, 0, handle) == 0)
        goto done;
    /* @native is descriptive metadata only: with no authorized binding,
       the exact same artifact must execute its original bytecode body. */
    if (slvm_run(vm) != SLR_OK) goto done;
    result = slvm_result(vm);
    if (result.type != SL_INT || result.as.i != 17) goto done;
    ok = 1;
done:
    if (handle != SL_NATIVE_HANDLE_INVALID) (void)slvm_native_module_close(vm, handle);
    slvm_free(vm);
    return ok;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: native_method_boundary_smoke <composite-artifact> <error-artifact> <plugin>\n");
        return 2;
    }
    if (!check_composite_fallback(argv[1], argv[3])) {
        fprintf(stderr, "VM-owned array/struct arguments did not use bytecode fallback\n");
        return 1;
    }
    if (!check_plugin_error_is_not_fallback(argv[2], argv[3])) {
        fprintf(stderr, "handled native plugin error was hidden by bytecode fallback\n");
        return 1;
    }
    if (!check_denied_path_cannot_bind(argv[2], argv[3])) {
        fprintf(stderr, "denied native module path became executable through @native metadata\n");
        return 1;
    }
    puts("native @native boundary: PASS (array/struct fallback, handled error propagation, denied path remains denied)");
    return 0;
}
