#include "native_module_table.h"

#include <assert.h>
#include <string.h>

static int authorize_exact_path(const char *path, void *context) {
    const char *expected = (const char *)context;
    return path && expected && strcmp(path, expected) == 0;
}

int main(int argc, char **argv) {
    SLNativeModule module = {0};
    SLNativeModuleTable table;
    SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
    SLNativeValue args[2];
    SLNativeValue result;
    int run_result = -1;
    assert(argc == 3);
    assert(sl_native_open(&module, argv[1]) == SL_NATIVE_OK);

    memset(args, 0, sizeof(args));
    args[0].tag = SL_NATIVE_VALUE_I64;
    args[0].as.i64 = 19;
    args[1].tag = SL_NATIVE_VALUE_I64;
    args[1].as.i64 = 23;
    assert(sl_native_call_module(&module, "sum", 2, args, &result) == SL_NATIVE_OK);
    assert(result.tag == SL_NATIVE_VALUE_I64 && result.as.i64 == 42);

    args[1].reserved = 1;
    assert(sl_native_call_module(&module, "sum", 2, args, &result) == SL_NATIVE_INVALID_ARGUMENT);
    args[1].reserved = 0;
    assert(sl_native_call_module(&module, "missing", 2, args, &result) == SL_NATIVE_PLUGIN_ERROR);
    assert(sl_native_close(&module) == SL_NATIVE_OK);

    /* Existing ABI-v1 modules without the optional typed symbol remain valid. */
    assert(sl_native_open(&module, argv[2]) == SL_NATIVE_OK);
    assert(sl_native_call_module(&module, "sum", 2, args, &result) == SL_NATIVE_TYPED_ENTRY_NOT_FOUND);
    assert(sl_native_run_module(&module, 0, NULL, &run_result) == SL_NATIVE_OK);
    assert(run_result == 0);
    assert(sl_native_close(&module) == SL_NATIVE_OK);

    /* The bounded handle table exposes typed calls without leaking module pointers. */
    assert(sl_native_table_init(&table, authorize_exact_path, argv[1]) == SL_NATIVE_OK);
    assert(sl_native_table_open(&table, argv[1], &handle) == SL_NATIVE_OK);
    assert(handle != SL_NATIVE_HANDLE_INVALID);
    assert(sl_native_table_is_valid(&table, handle));
    assert(sl_native_table_call(&table, handle, "sum", 2, args, &result) == SL_NATIVE_OK);
    assert(result.tag == SL_NATIVE_VALUE_I64 && result.as.i64 == 42);
    assert(sl_native_table_close(&table, handle) == SL_NATIVE_OK);
    assert(!sl_native_table_is_valid(&table, handle));
    assert(sl_native_table_call(&table, handle, "sum", 2, args, &result) == SL_NATIVE_STALE_HANDLE);
    assert(sl_native_table_close_all(&table) == SL_NATIVE_OK);
    return 0;
}
