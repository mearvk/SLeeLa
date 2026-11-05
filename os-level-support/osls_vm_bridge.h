#ifndef OSLS_VM_BRIDGE_H
#define OSLS_VM_BRIDGE_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSLS_VM_SYSTEM_INFO = 0,
    OSLS_VM_PROCESS_INFO = 1,
    OSLS_VM_ENUMERATE_PROCESSES = 2,
    OSLS_VM_GET_ENVIRONMENT_VALUE = 3,
    OSLS_VM_ENUMERATE_ENVIRONMENT = 4,
    OSLS_VM_GET_FILE_INFO = 5,
    OSLS_VM_LIST_DIRECTORY = 6,
    OSLS_VM_GET_CPU_INFO = 7,
    OSLS_VM_GET_TIMEZONE_INFO = 8,
    OSLS_VM_GET_NETWORK_INTERFACES = 9,
    OSLS_VM_CRON_VALIDATE = 10,
    OSLS_VM_CRON_NEXT_AFTER = 11,
    OSLS_VM_GET_MEMORY_INFO = 12,
    OSLS_VM_GET_AVAILABLE_MEMORY = 13,
    OSLS_VM_GET_DISK_SPACE = 14,
    OSLS_VM_GET_VOLUME_INFO = 15,
    OSLS_VM_GET_SYSTEM_LOAD = 16,
    OSLS_VM_GET_CAPABILITIES = 17,
    OSLS_VM_GET_RUNTIME_INFO = 18,
    OSLS_VM_GET_NATIVE_ERROR = 19,
    OSLS_VM_PATH_NORMALIZE = 20,
    OSLS_VM_PATH_JOIN = 21,
    OSLS_VM_PATH_RESOLVE = 22,
    OSLS_VM_GET_PATH_INFO = 23,
    OSLS_VM_NETWORK_RESOLVE = 24,
    OSLS_VM_OPERATION_COUNT = 25
} osls_vm_operation;

typedef enum {
    OSLS_VM_ARG_UTF8 = 1,
    OSLS_VM_ARG_I64 = 2
} osls_vm_arg_type;

typedef enum {
    OSLS_VM_RESULT_JSON_UTF8 = 1
} osls_vm_result_type;

#define OSLS_VM_CAP_HOST_READ UINT64_C(0x0000000000000001)

typedef struct {
    osls_vm_arg_type type;
    union {
        const char *utf8;
        int64_t i64;
    } value;
} osls_vm_value;

typedef struct {
    osls_vm_result_type type;
    const char *utf8;
    size_t length;
} osls_vm_result;

typedef int (*osls_vm_invoke_fn)(const osls_vm_value *args, size_t arg_count,
                                 char *out_json, size_t out_capacity);

typedef struct {
    int operation;
    const char *source_name;
    const char *native_symbol;
    unsigned argument_count;
    const osls_vm_arg_type *argument_types;
    osls_vm_result_type result_type;
    uint64_t required_capabilities;
    const char *argument_contract;
    const char *result_contract;
    osls_vm_invoke_fn invoke;
} osls_vm_binding;

/* This is an executable registry: every row contains the callable native
 * adapter, the native ABI symbol identity, and the complete marshalling
 * signature. Returned records/collections use a bounded UTF-8 JSON result. */
const osls_vm_binding *osls_vm_binding_table(size_t *out_count);
const osls_vm_binding *osls_vm_binding_by_id(int operation);
const osls_vm_binding *osls_vm_binding_by_symbol(const char *native_symbol);

int osls_vm_call_authorized(int operation, const osls_vm_value *args, size_t arg_count,
                            uint64_t granted_capabilities, char *out_json,
                            size_t out_capacity, osls_vm_result *out_result);
int osls_vm_call_typed(int operation, const osls_vm_value *args, size_t arg_count,
                       char *out_json, size_t out_capacity, osls_vm_result *out_result);

/* Compatibility adapter for the first OSLS bridge revision. New VM code must
 * use osls_vm_call_typed so argument and result tags are checked explicitly. */
int osls_vm_call(int operation, const char *text_argument,
                 int64_t integer_argument, char *out_json, size_t out_capacity);
#ifdef __cplusplus
}
#endif
#endif
