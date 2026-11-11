#include "../../core/sleela_core.h"
#include <stdio.h>
#include <string.h>

static int authorize_exact_path(const char *path, void *context) {
    const char *approved = (const char *)context;
    return path && approved && strcmp(path, approved) == 0;
}

typedef struct {
    SLVM *vm;
    SLNativeHandle handle;
    const char *plugin_path;
    int open_rejected;
    int run_rejected;
    int call_rejected;
    int close_rejected;
} NativeReentryPolicyContext;

static int authorize_callback_reentry(const char *function, void *context) {
    NativeReentryPolicyContext *state = (NativeReentryPolicyContext *)context;
    SLNativeHandle opened = SL_NATIVE_HANDLE_INVALID;
    SLNativeValue arg, result;
    int run_result = 0;
    SLNativeStatus open_status, run_status, call_status, close_status;
    if (!state || !state->vm || !function ||
        strcmp(function, "Callback::receive") != 0) return 0;

    open_status = slvm_native_module_open(state->vm, state->plugin_path, &opened);
    run_status = slvm_native_module_run(state->vm, state->handle, 0, NULL, &run_result);
    memset(&arg, 0, sizeof(arg));
    memset(&result, 0, sizeof(result));
    arg.tag = SL_NATIVE_VALUE_I64;
    arg.as.i64 = 1;
    call_status = slvm_native_module_call(
        state->vm, state->handle, "twice", 1, &arg, &result);
    close_status = slvm_native_module_close(state->vm, state->handle);

    state->open_rejected = open_status == SL_NATIVE_REENTRANT_CALL &&
                           opened == SL_NATIVE_HANDLE_INVALID;
    state->run_rejected = run_status == SL_NATIVE_REENTRANT_CALL;
    state->call_rejected = call_status == SL_NATIVE_REENTRANT_CALL;
    state->close_rejected = close_status == SL_NATIVE_REENTRANT_CALL;
    return 1;
}

static int authorize_callback_name(const char *function, void *context) {
    const char *approved = (const char *)context;
    return function && approved && strcmp(function, approved) == 0;
}


static int check_host_run_callback_rejected(const char *plugin_path) {
    SLVM *vm = slvm_new();
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    int plugin_result = 0;
    SLNativeStatus status;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin_path) != 0 ||
        slvm_set_native_callback_authorizer(
            vm, authorize_callback_name, (void *)"Callback::receive") != 0 ||
        slvm_native_module_open(vm, plugin_path, &handle) != SL_NATIVE_OK) {
        slvm_free(vm);
        return 0;
    }
    status = slvm_native_module_run(vm, handle, 0, NULL, &plugin_result);
    ok = status == SL_NATIVE_OK && plugin_result == -1 &&
         slvm_error(vm) &&
         strcmp(slvm_error(vm),
                "native callback requires an active source-level nativeCall context") == 0;
    (void)slvm_native_module_close(vm, handle);
    slvm_free(vm);
    return ok;
}

static int check_host_call_callback_rejected(const char *plugin_path) {
    SLVM *vm = slvm_new();
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    SLNativeValue arg, result;
    SLNativeStatus status;
    int ok = 0;
    if (!vm) return 0;
    memset(&arg, 0, sizeof(arg));
    memset(&result, 0, sizeof(result));
    arg.tag = SL_NATIVE_VALUE_I64;
    arg.as.i64 = 41;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin_path) != 0 ||
        slvm_set_native_callback_authorizer(
            vm, authorize_callback_name, (void *)"Callback::receive") != 0 ||
        slvm_native_module_open(vm, plugin_path, &handle) != SL_NATIVE_OK) {
        slvm_free(vm);
        return 0;
    }
    status = slvm_native_module_call(vm, handle, "call_sleela", 1, &arg, &result);
    ok = status == SL_NATIVE_PLUGIN_ERROR &&
         slvm_error(vm) &&
         strcmp(slvm_error(vm),
                "native callback requires an active source-level nativeCall context") == 0;
    (void)slvm_native_module_close(vm, handle);
    slvm_free(vm);
    return ok;
}

static int check_host_api_reentry_from_callback_policy(const char *plugin_path) {
    SLVM *vm = slvm_new();
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    NativeReentryPolicyContext policy;
    int method_name, value, module_value;
    int callback_function, main_function;
    SLValue result;
    int ok = 0;
    if (!vm) return 0;
    memset(&policy, 0, sizeof(policy));
    policy.vm = vm;
    policy.plugin_path = plugin_path;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin_path) != 0 ||
        slvm_set_native_callback_authorizer(
            vm, authorize_callback_reentry, &policy) != 0 ||
        slvm_native_module_open(vm, plugin_path, &handle) != SL_NATIVE_OK)
        goto cleanup;
    policy.handle = handle;

    method_name = slvm_add_const_str(vm, "call_sleela");
    value = slvm_add_const_int(vm, 41);
    module_value = slvm_add_const_int(vm, (int64_t)handle);
    if (method_name < 0 || value < 0 || module_value < 0) goto cleanup;

    callback_function = slvm_begin_func(vm, "Callback::receive", 1, 1);
    if (callback_function < 0) goto cleanup;
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);

    main_function = slvm_begin_func(vm, "NativeCallbackPolicyReentry::main", 0, 0);
    if (main_function < 0) goto cleanup;
    slvm_emit(vm, OP_CONST, module_value);
    slvm_emit(vm, OP_CONST, method_name);
    slvm_emit(vm, OP_CONST, value);
    slvm_emit(vm, OP_NATIVE_CALL, 1);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);
    slvm_set_entry(vm, main_function);

    if (slvm_run(vm) != SLR_OK) goto cleanup;
    result = slvm_result(vm);
    ok = result.type == SL_INT && result.as.i == 41 &&
         policy.open_rejected && policy.run_rejected &&
         policy.call_rejected && policy.close_rejected;
cleanup:
    (void)slvm_native_module_close(vm, handle);
    slvm_free(vm);
    return ok;
}

static int check_callback_lifecycle_opcode_rejected(const char *plugin_path,
                                                         int close_opcode) {
    SLVM *vm = slvm_new();
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    const char *callback_name = close_opcode
        ? "Callback::receive_close" : "Callback::receive_open";
    const char *plugin_method = close_opcode
        ? "call_sleela_close" : "call_sleela_open";
    int method_name, value, module_value, path_value;
    int callback_function, main_function;
    SLValue result;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin_path) != 0 ||
        slvm_set_native_callback_authorizer(
            vm, authorize_callback_name, (void *)callback_name) != 0 ||
        slvm_native_module_open(vm, plugin_path, &handle) != SL_NATIVE_OK)
        goto cleanup;

    method_name = slvm_add_const_str(vm, plugin_method);
    value = slvm_add_const_int(vm, (int64_t)handle);
    module_value = slvm_add_const_int(vm, (int64_t)handle);
    path_value = slvm_add_const_str(vm, "callback-open-must-be-rejected");
    if (method_name < 0 || value < 0 || module_value < 0 || path_value < 0)
        goto cleanup;

    callback_function = slvm_begin_func(vm, callback_name, 1, 1);
    if (callback_function < 0) goto cleanup;
    if (close_opcode) {
        slvm_emit(vm, OP_LOADL, 0);
        slvm_emit0(vm, OP_NATIVE_CLOSE);
    } else {
        slvm_emit(vm, OP_CONST, path_value);
        slvm_emit0(vm, OP_NATIVE_OPEN);
    }
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);

    main_function = slvm_begin_func(vm, "NativeCallbackLifecycle::main", 0, 0);
    if (main_function < 0) goto cleanup;
    slvm_emit(vm, OP_CONST, module_value);
    slvm_emit(vm, OP_CONST, method_name);
    slvm_emit(vm, OP_CONST, value);
    slvm_emit(vm, OP_NATIVE_CALL, 1);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);
    slvm_set_entry(vm, main_function);

    if (slvm_run(vm) != SLR_OK) goto cleanup;
    result = slvm_result(vm);
    ok = result.type == SL_INT && result.as.i == -1 &&
         slvm_error(vm) &&
         strcmp(slvm_error(vm),
                "native callback cannot recursively invoke native-module operations") == 0;
cleanup:
    (void)slvm_native_module_close(vm, handle);
    slvm_free(vm);
    return ok;
}

static int check_callback_nested_native_call_rejected(const char *plugin_path) {
    SLVM *vm = slvm_new();
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    int twice_name, nested_name, value, module_value;
    int callback_function, main_function;
    SLValue result;
    SLNativeValue post_callback_arg, post_callback_result;
    SLNativeStatus post_callback_status;
    int ok = 0;
    if (!vm) return 0;
    if (slvm_set_native_authorizer(vm, authorize_exact_path, (void *)plugin_path) != 0 ||
        slvm_set_native_callback_authorizer(
            vm, authorize_callback_name, (void *)"Callback::receive_nested") != 0 ||
        slvm_native_module_open(vm, plugin_path, &handle) != SL_NATIVE_OK) {
        slvm_free(vm);
        return 0;
    }

    twice_name = slvm_add_const_str(vm, "twice");
    nested_name = slvm_add_const_str(vm, "call_sleela_nested");
    value = slvm_add_const_int(vm, 41);
    module_value = slvm_add_const_int(vm, (int64_t)handle);
    if (twice_name < 0 || nested_name < 0 || value < 0 || module_value < 0)
        goto cleanup;

    /* Build callback bytecode directly: receive_nested(value, module) tries
     * nativeCall(module, "twice", value) while the outer plugin call is active. */
    callback_function = slvm_begin_func(vm, "Callback::receive_nested", 2, 2);
    if (callback_function < 0) goto cleanup;
    slvm_emit(vm, OP_LOADL, 1);
    slvm_emit(vm, OP_CONST, twice_name);
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_NATIVE_CALL, 1);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);

    main_function = slvm_begin_func(vm, "NativeCallbackReentry::main", 0, 0);
    if (main_function < 0) goto cleanup;
    slvm_emit(vm, OP_CONST, module_value);
    slvm_emit(vm, OP_CONST, nested_name);
    slvm_emit(vm, OP_CONST, value);
    slvm_emit(vm, OP_CONST, module_value);
    slvm_emit(vm, OP_NATIVE_CALL, 2);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);
    slvm_set_entry(vm, main_function);

    if (slvm_run(vm) != SLR_OK) goto cleanup;
    result = slvm_result(vm);
    ok = result.type == SL_INT && result.as.i == -1 &&
         slvm_error(vm) &&
         strcmp(slvm_error(vm),
                "native callback cannot recursively invoke native-module operations") == 0;
    if (!ok) goto cleanup;

    /* The rejected nested call must leave the module mutex usable and the
     * original handle live after the outer callback has unwound. */
    memset(&post_callback_arg, 0, sizeof(post_callback_arg));
    memset(&post_callback_result, 0, sizeof(post_callback_result));
    post_callback_arg.tag = SL_NATIVE_VALUE_I64;
    post_callback_arg.as.i64 = 41;
    post_callback_status = slvm_native_module_call(
        vm, handle, "twice", 1, &post_callback_arg, &post_callback_result);
    ok = post_callback_status == SL_NATIVE_OK &&
         post_callback_result.tag == SL_NATIVE_VALUE_I64 &&
         post_callback_result.as.i64 == 82 &&
         slvm_error(vm) &&
         strcmp(slvm_error(vm),
                "native callback cannot recursively invoke native-module operations") == 0;

cleanup:
    (void)slvm_native_module_close(vm, handle);
    slvm_free(vm);
    return ok;
}

int main(int argc, char **argv) {
    SLVM *vm;
    SLValue result;
    if (argc != 3) {
        fprintf(stderr, "usage: native_call_smoke <compiled-artifact> <trusted-plugin-path>\\n");
        return 2;
    }

    /* This test intentionally loads compiler-produced bytecode rather than
     * assembling opcodes in C. It exercises source -> artifact -> VM -> plugin. */
    vm = slvm_load_file_with_entry(argv[1], "NativeDriveCompile::check");
    if (!vm) {
        fprintf(stderr, "could not load compiled Native Drive artifact: %s\\n", argv[1]);
        return 2;
    }
    if (slvm_set_native_authorizer(vm, authorize_exact_path, argv[2]) != 0) {
        fprintf(stderr, "could not install native authorization callback\\n");
        slvm_free(vm);
        return 2;
    }
    if (slvm_set_native_callback_authorizer(
            vm, authorize_callback_name, (void *)"Callback::receive") != 0) {
        fprintf(stderr, "could not install native callback-name authorization\\n");
        slvm_free(vm);
        return 2;
    }

    if (slvm_run(vm) != SLR_OK) {
        fprintf(stderr, "compiled Native Drive execution failed: %s\\n",
                slvm_error(vm) ? slvm_error(vm) : "unknown VM error");
        slvm_free(vm);
        return 1;
    }
    result = slvm_result(vm);
    slvm_free(vm);
    if (result.type != SL_INT || result.as.i != 105) {
        fprintf(stderr, "expected 105 from native dispatch, SLeeLa callback, and bytecode fallback\\n");
        return 1;
    }
    /* Re-load the same compiler-produced artifact with a deliberately
     * mismatched allowlist. Source-level nativeOpen must fail closed before
     * any plugin code is entered. */
    vm = slvm_load_file_with_entry(argv[1], "NativeDriveCompile::check");
    if (!vm) {
        fprintf(stderr, "could not reload artifact for authorization-denial check\\n");
        return 2;
    }
    if (slvm_set_native_authorizer(vm, authorize_exact_path,
                                  (void *)"not-the-approved-plugin") != 0) {
        fprintf(stderr, "could not install denial-test authorization callback\\n");
        slvm_free(vm);
        return 2;
    }
    if (slvm_run(vm) != SLR_ERROR ||
        !slvm_error(vm) || !strstr(slvm_error(vm), "denied")) {
        fprintf(stderr, "source-level nativeOpen did not fail closed for an unapproved path\\n");
        slvm_free(vm);
        return 1;
    }
    slvm_free(vm);

    /* A trusted module path is not enough to authorize callback execution.
     * The host must separately allow the exact SLeeLa callback name. */
    vm = slvm_load_file_with_entry(argv[1], "NativeDriveCompile::check");
    if (!vm) {
        fprintf(stderr, "could not reload artifact for callback-policy denial check\\n");
        return 2;
    }
    if (slvm_set_native_authorizer(vm, authorize_exact_path, argv[2]) != 0) {
        fprintf(stderr, "could not install callback-policy test module authorization\\n");
        slvm_free(vm);
        return 2;
    }
    if (slvm_run(vm) != SLR_ERROR ||
        !slvm_error(vm) || !strstr(slvm_error(vm), "failed")) {
        fprintf(stderr, "native callback executed without a host callback-name allowlist\\n");
        slvm_free(vm);
        return 1;
    }
    slvm_free(vm);
    if (!check_host_run_callback_rejected(argv[2])) {
        fprintf(stderr, "host-side slvm_native_module_run did not reject its SLeeLa callback\n");
        return 1;
    }
    if (!check_host_call_callback_rejected(argv[2])) {
        fprintf(stderr, "host-side slvm_native_module_call did not reject its SLeeLa callback\n");
        return 1;
    }
    if (!check_host_api_reentry_from_callback_policy(argv[2])) {
        fprintf(stderr, "host native-module APIs did not reject reentry from callback policy\\n");
        return 1;
    }
    if (!check_callback_lifecycle_opcode_rejected(argv[2], 0)) {
        fprintf(stderr, "callback bytecode did not reject recursive nativeOpen\\n");
        return 1;
    }
    if (!check_callback_lifecycle_opcode_rejected(argv[2], 1)) {
        fprintf(stderr, "callback bytecode did not reject recursive nativeClose\\n");
        return 1;
    }
    if (!check_callback_nested_native_call_rejected(argv[2])) {
        fprintf(stderr, "callback bytecode did not reject recursive nativeCall\\n");
        return 1;
    }
    puts("compiled source callback and native dispatch; host module_run/module_call callback rejection; recursive callback nativeOpen/nativeCall/nativeClose rejection; host API reentry rejection; module and callback authorization denial: PASS");
    return 0;
}
