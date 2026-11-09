#ifndef SLEELA_NATIVE_MODULE_TABLE_H
#define SLEELA_NATIVE_MODULE_TABLE_H

#include "native_loader.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SL_NATIVE_TABLE_CAPACITY 9u
#define SL_NATIVE_HANDLE_INVALID 0u

typedef uint32_t SLNativeHandle;
typedef int (*SLNativeAuthorizePath)(const char *path, void *context);

typedef struct SLNativeModuleSlot {
    SLNativeModule module;
    uint32_t generation;
    int occupied;
} SLNativeModuleSlot;

/*
 * Runtime-owned bounded module table. The host supplies an authorization
 * callback; a path identifies a candidate and never grants authority by itself.
 * Callers must externally synchronize concurrent access to one table.
 */
typedef struct SLNativeModuleTable {
    SLNativeModuleSlot slots[SL_NATIVE_TABLE_CAPACITY];
    SLNativeAuthorizePath authorize;
    void *authorize_context;
    int initialized;
} SLNativeModuleTable;

SLNativeStatus sl_native_table_init(SLNativeModuleTable *table,
                                    SLNativeAuthorizePath authorize,
                                    void *authorize_context);
SLNativeStatus sl_native_table_open(SLNativeModuleTable *table,
                                    const char *path,
                                    SLNativeHandle *out_handle);
SLNativeStatus sl_native_table_run(SLNativeModuleTable *table,
                                   SLNativeHandle handle,
                                   int argc,
                                   const char *const argv[],
                                   int *out_result);
SLNativeStatus sl_native_table_close(SLNativeModuleTable *table,
                                     SLNativeHandle handle);
SLNativeStatus sl_native_table_close_all(SLNativeModuleTable *table);

#ifdef __cplusplus
}
#endif
#endif
