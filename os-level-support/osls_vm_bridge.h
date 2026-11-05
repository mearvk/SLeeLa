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

typedef struct {
    int operation;
    const char *source_name;
    const char *native_symbol;
    unsigned argument_count;
    const char *argument_contract;
    const char *result_contract;
} osls_vm_binding;

const osls_vm_binding *osls_vm_binding_table(size_t *out_count);
int osls_vm_call(int operation, const char *text_argument,
                 int64_t integer_argument, char *out_json, size_t out_capacity);
#endif
