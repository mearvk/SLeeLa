#include "native_loader.h"

#include <string.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
static const char *loader_error = "no Windows loader error";
#else
#  include <dlfcn.h>
#endif

static void *open_library(const char *path) {
#ifdef _WIN32
    HMODULE handle = LoadLibraryA(path);
    if (!handle) loader_error = "LoadLibraryA failed";
    return (void *)handle;
#else
    dlerror();
    return dlopen(path, RTLD_NOW | RTLD_LOCAL);
#endif
}

static void close_library(void *handle) {
    if (!handle) return;
#ifdef _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}

static void *find_symbol(void *handle, const char *name) {
#ifdef _WIN32
    return (void *)(uintptr_t)GetProcAddress((HMODULE)handle, name);
#else
    dlerror();
    return dlsym(handle, name);
#endif
}

static int value_valid(const SLNativeValue *value) {
    if (!value || value->reserved != 0) return 0;
    switch ((SLNativeValueTag)value->tag) {
    case SL_NATIVE_VALUE_NULL: return 1;
    case SL_NATIVE_VALUE_BOOL: return value->as.boolean == 0 || value->as.boolean == 1;
    case SL_NATIVE_VALUE_I64: return 1;
    case SL_NATIVE_VALUE_F64: return 1;
    case SL_NATIVE_VALUE_UTF8:
        return value->as.utf8.length == 0 || value->as.utf8.data != NULL;
    default: return 0;
    }
}

const char *sl_native_loader_error(void) {
#ifdef _WIN32
    return loader_error;
#else
    const char *error = dlerror();
    return error ? error : "no dynamic-loader error";
#endif
}

const char *sl_native_status_string(SLNativeStatus status) {
    switch (status) {
    case SL_NATIVE_OK: return "ok";
    case SL_NATIVE_INVALID_ARGUMENT: return "invalid argument or module state";
    case SL_NATIVE_LOAD_ERROR: return "could not load shared library";
    case SL_NATIVE_ABI_MISMATCH: return "native module ABI version mismatch";
    case SL_NATIVE_ENTRY_NOT_FOUND: return "required module entry point not found";
    case SL_NATIVE_AUTHORIZATION_DENIED: return "native module path was not authorized by host policy";
    case SL_NATIVE_TABLE_FULL: return "native module handle table is full";
    case SL_NATIVE_STALE_HANDLE: return "native module handle is stale or invalid";
    case SL_NATIVE_TYPED_ENTRY_NOT_FOUND: return "native module does not export the typed-call entry";
    case SL_NATIVE_PLUGIN_ERROR: return "native plugin method returned an error or invalid value";
    default: return "unknown native-loader status";
    }
}

SLNativeStatus sl_native_open(SLNativeModule *module, const char *path) {
    SLNativeAbiVersion abi;
    SLNativeEntry entry;
    void *symbol;
    void *handle;

    if (!module || !path || !*path || module->handle || module->run)
        return SL_NATIVE_INVALID_ARGUMENT;

    handle = open_library(path);
    if (!handle) return SL_NATIVE_LOAD_ERROR;

    symbol = find_symbol(handle, "sl_native_abi_version");
    if (!symbol) {
        close_library(handle);
        return SL_NATIVE_ENTRY_NOT_FOUND;
    }
    memcpy(&abi, &symbol, sizeof(abi));

    symbol = find_symbol(handle, "sl_native_run");
    if (!symbol) {
        close_library(handle);
        return SL_NATIVE_ENTRY_NOT_FOUND;
    }
    memcpy(&entry, &symbol, sizeof(entry));

    if (abi() != SL_NATIVE_ABI_VERSION) {
        close_library(handle);
        return SL_NATIVE_ABI_MISMATCH;
    }

    module->handle = handle;
    module->run = entry;
    return SL_NATIVE_OK;
}

SLNativeStatus sl_native_run_module(SLNativeModule *module, int argc,
                                    const char *const argv[], int *out_result) {
    int i;
    if (!module || !module->handle || !module->run || argc < 0 ||
        (argc > 0 && !argv) || !out_result)
        return SL_NATIVE_INVALID_ARGUMENT;
    for (i = 0; i < argc; ++i) {
        if (!argv[i]) return SL_NATIVE_INVALID_ARGUMENT;
    }
    *out_result = module->run(argc, argv);
    return SL_NATIVE_OK;
}

SLNativeStatus sl_native_call_module(SLNativeModule *module, const char *method,
                                     uint32_t argc, const SLNativeValue *argv,
                                     SLNativeValue *out_result) {
    uint32_t i;
    int plugin_status;
    SLNativeValue result;
    SLNativeTypedCall typed_call;
    SLNativeTypedAbiVersion typed_abi;
    void *symbol;
    if (!module || !module->handle || !method || !*method ||
        (argc > 0 && !argv) || !out_result)
        return SL_NATIVE_INVALID_ARGUMENT;
    symbol = find_symbol(module->handle, "sl_native_call");
    if (!symbol) return SL_NATIVE_TYPED_ENTRY_NOT_FOUND;
    memcpy(&typed_call, &symbol, sizeof(typed_call));
    symbol = find_symbol(module->handle, "sl_native_typed_abi_version");
    if (!symbol) return SL_NATIVE_ABI_MISMATCH;
    memcpy(&typed_abi, &symbol, sizeof(typed_abi));
    if (typed_abi() != SL_NATIVE_TYPED_ABI_VERSION)
        return SL_NATIVE_ABI_MISMATCH;
    for (i = 0; i < argc; ++i)
        if (!value_valid(&argv[i])) return SL_NATIVE_INVALID_ARGUMENT;
    memset(&result, 0, sizeof(result));
    result.tag = SL_NATIVE_VALUE_NULL;
    plugin_status = typed_call(method, argc, argv, &result);
    if (plugin_status != 0) return SL_NATIVE_PLUGIN_ERROR;
    if (!value_valid(&result)) return SL_NATIVE_PLUGIN_ERROR;
    *out_result = result;
    return SL_NATIVE_OK;
}

SLNativeStatus sl_native_close(SLNativeModule *module) {
    if (!module) return SL_NATIVE_INVALID_ARGUMENT;
    if (module->handle) close_library(module->handle);
    memset(module, 0, sizeof(*module));
    return SL_NATIVE_OK;
}
