#include "native_module_table.h"

#include <string.h>

#define HANDLE_INDEX_BITS 4u
#define HANDLE_INDEX_MASK ((1u << HANDLE_INDEX_BITS) - 1u)
#define HANDLE_GENERATION_MAX (UINT32_MAX >> HANDLE_INDEX_BITS)

static SLNativeStatus invalid_table(void) {
    return SL_NATIVE_INVALID_ARGUMENT;
}

static SLNativeModuleSlot *resolve(SLNativeModuleTable *table,
                                   SLNativeHandle handle) {
    uint32_t encoded_index = handle & HANDLE_INDEX_MASK;
    uint32_t generation = handle >> HANDLE_INDEX_BITS;
    SLNativeModuleSlot *slot;
    if (!table || !table->initialized || !handle || !encoded_index ||
        encoded_index > SL_NATIVE_TABLE_CAPACITY || !generation)
        return NULL;
    slot = &table->slots[encoded_index - 1u];
    if (!slot->occupied || slot->generation != generation)
        return NULL;
    return slot;
}

SLNativeStatus sl_native_table_init(SLNativeModuleTable *table,
                                    SLNativeAuthorizePath authorize,
                                    void *authorize_context) {
    size_t i;
    if (!table || !authorize) return SL_NATIVE_INVALID_ARGUMENT;
    memset(table, 0, sizeof(*table));
    table->authorize = authorize;
    table->authorize_context = authorize_context;
    for (i = 0; i < SL_NATIVE_TABLE_CAPACITY; ++i)
        table->slots[i].generation = 1u;
    table->initialized = 1;
    return SL_NATIVE_OK;
}

SLNativeStatus sl_native_table_open(SLNativeModuleTable *table,
                                    const char *path,
                                    SLNativeHandle *out_handle) {
    size_t i;
    SLNativeModule candidate = {0};
    SLNativeStatus status;
    if (!table || !table->initialized || !path || !*path || !out_handle)
        return invalid_table();
    *out_handle = SL_NATIVE_HANDLE_INVALID;
    if (!table->authorize(path, table->authorize_context))
        return SL_NATIVE_AUTHORIZATION_DENIED;
    for (i = 0; i < SL_NATIVE_TABLE_CAPACITY; ++i)
        if (!table->slots[i].occupied) break;
    if (i == SL_NATIVE_TABLE_CAPACITY) return SL_NATIVE_TABLE_FULL;
    status = sl_native_open(&candidate, path);
    if (status != SL_NATIVE_OK) return status;
    table->slots[i].module = candidate;
    table->slots[i].occupied = 1;
    *out_handle = (table->slots[i].generation << HANDLE_INDEX_BITS) |
                  (uint32_t)(i + 1u);
    return SL_NATIVE_OK;
}

SLNativeStatus sl_native_table_run(SLNativeModuleTable *table,
                                   SLNativeHandle handle,
                                   int argc,
                                   const char *const argv[],
                                   int *out_result) {
    SLNativeModuleSlot *slot = resolve(table, handle);
    if (!slot) return SL_NATIVE_STALE_HANDLE;
    return sl_native_run_module(&slot->module, argc, argv, out_result);
}

SLNativeStatus sl_native_table_call(SLNativeModuleTable *table,
                                    SLNativeHandle handle,
                                    const char *method,
                                    uint32_t argc,
                                    const SLNativeValue *argv,
                                    SLNativeValue *out_result) {
    SLNativeModuleSlot *slot = resolve(table, handle);
    if (!slot) return SL_NATIVE_STALE_HANDLE;
    return sl_native_call_module(&slot->module, method, argc, argv, out_result);
}

int sl_native_table_is_valid(SLNativeModuleTable *table, SLNativeHandle handle) {
    return resolve(table, handle) != NULL;
}

SLNativeStatus sl_native_table_close(SLNativeModuleTable *table,
                                     SLNativeHandle handle) {
    SLNativeModuleSlot *slot = resolve(table, handle);
    SLNativeStatus status;
    if (!slot) return SL_NATIVE_STALE_HANDLE;
    status = sl_native_close(&slot->module);
    if (status != SL_NATIVE_OK) return status;
    slot->occupied = 0;
    slot->generation = slot->generation >= HANDLE_GENERATION_MAX
        ? 1u : slot->generation + 1u;
    return SL_NATIVE_OK;
}

SLNativeStatus sl_native_table_close_all(SLNativeModuleTable *table) {
    size_t i;
    SLNativeStatus result = SL_NATIVE_OK;
    if (!table || !table->initialized) return SL_NATIVE_INVALID_ARGUMENT;
    for (i = 0; i < SL_NATIVE_TABLE_CAPACITY; ++i) {
        SLNativeModuleSlot *slot = &table->slots[i];
        if (!slot->occupied) continue;
        if (sl_native_close(&slot->module) == SL_NATIVE_OK) {
            slot->occupied = 0;
            slot->generation = slot->generation >= HANDLE_GENERATION_MAX
                ? 1u : slot->generation + 1u;
        } else {
            result = SL_NATIVE_LOAD_ERROR;
        }
    }
    return result;
}
