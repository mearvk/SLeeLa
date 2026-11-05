#ifndef OSLS_VM_BRIDGE_H
#define OSLS_VM_BRIDGE_H
#include <stddef.h>
#include <stdint.h>

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
    OSLS_VM_OPERATION_COUNT = 12
} osls_vm_operation;

typedef enum {
    OSLS_VM_ARG_UTF8 = 1,
    OSLS_VM_ARG_I64 = 2
} osls_vm_arg_type;

typedef enum {
    OSLS_VM_RESULT_JSON_UTF8 = 1
} osls_vm_result_type;

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

int osls_vm_call_typed(int operation, const osls_vm_value *args, size_t arg_count,
                       char *out_json, size_t out_capacity, osls_vm_result *out_result);

/* Compatibility adapter for the first OSLS bridge revision. New VM code must
 * use osls_vm_call_typed so argument and result tags are checked explicitly. */
int osls_vm_call(int operation, const char *text_argument,
                 int64_t integer_argument, char *out_json, size_t out_capacity);
#endif
