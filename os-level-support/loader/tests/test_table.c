#include "../native_module_table.h"
#include <assert.h>
#include <string.h>

static int authorize_only(const char *path, void *context) {
    const char *allowed = (const char *)context;
    return path && allowed && strcmp(path, allowed) == 0;
}

int main(int argc, char **argv) {
    SLNativeModuleTable table;
    SLNativeHandle handles[SL_NATIVE_TABLE_CAPACITY];
    SLNativeHandle replacement = SL_NATIVE_HANDLE_INVALID;
    const char *args[] = {"one", "two"};
    int result = -1;
    size_t i;
    assert(argc == 3);
    assert(sl_native_table_init(&table, authorize_only, argv[1]) == SL_NATIVE_OK);
    assert(sl_native_table_open(&table, argv[2], &replacement) == SL_NATIVE_AUTHORIZATION_DENIED);
    assert(replacement == SL_NATIVE_HANDLE_INVALID);

    for (i = 0; i < SL_NATIVE_TABLE_CAPACITY; ++i)
        assert(sl_native_table_open(&table, argv[1], &handles[i]) == SL_NATIVE_OK);
    assert(sl_native_table_open(&table, argv[1], &replacement) == SL_NATIVE_TABLE_FULL);
    assert(replacement == SL_NATIVE_HANDLE_INVALID);

    assert(sl_native_table_run(&table, handles[0], 2, args, &result) == SL_NATIVE_OK);
    assert(result == 2);
    assert(sl_native_table_close(&table, handles[0]) == SL_NATIVE_OK);
    assert(sl_native_table_run(&table, handles[0], 2, args, &result) == SL_NATIVE_STALE_HANDLE);
    assert(sl_native_table_open(&table, argv[1], &replacement) == SL_NATIVE_OK);
    assert(replacement != handles[0]);
    assert(sl_native_table_run(&table, handles[0], 2, args, &result) == SL_NATIVE_STALE_HANDLE);
    assert(sl_native_table_close_all(&table) == SL_NATIVE_OK);
    assert(sl_native_table_run(&table, replacement, 2, args, &result) == SL_NATIVE_STALE_HANDLE);
    return 0;
}
