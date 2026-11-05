#include "osls_vm_bridge.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_registry(void) {
    size_t count = 0, i;
    const osls_vm_binding *table = osls_vm_binding_table(&count);
    assert(table != NULL && count == OSLS_VM_OPERATION_COUNT);
    for (i = 0; i < count; ++i) {
        assert(table[i].operation == (int)i);
        assert(table[i].source_name && table[i].source_name[0]);
        assert(table[i].native_symbol && table[i].native_symbol[0]);
        assert(table[i].invoke != NULL);
        assert(table[i].result_type == OSLS_VM_RESULT_JSON_UTF8);
        assert(osls_vm_binding_by_id((int)i) == &table[i]);
        assert(osls_vm_binding_by_symbol(table[i].native_symbol) == &table[i]);
    }
    assert(osls_vm_binding_by_id(-1) == NULL);
    assert(osls_vm_binding_by_id(OSLS_VM_OPERATION_COUNT) == NULL);
    assert(osls_vm_binding_by_symbol("osls_missing_native_symbol") == NULL);
    assert(osls_vm_binding_by_symbol(NULL) == NULL);
}

static void test_typed_validation(void) {
    char json[65536] = "stale";
    osls_vm_result result;
    osls_vm_value wrong[1];
    wrong[0].type = OSLS_VM_ARG_UTF8;
    wrong[0].value.utf8 = "0";
    assert(osls_vm_call_typed(OSLS_VM_PROCESS_INFO, wrong, 1,
                              json, sizeof(json), &result) == -1);
    assert(json[0] == '\0');

    assert(osls_vm_call_typed(OSLS_VM_SYSTEM_INFO, wrong, 1,
                              json, sizeof(json), &result) == -1);
    assert(json[0] == '\0');

    assert(osls_vm_call_authorized(OSLS_VM_SYSTEM_INFO, NULL, 0, 0,
                                   json, sizeof(json), &result) == -1);
    assert(json[0] == '\0');

    assert(osls_vm_call_authorized(OSLS_VM_SYSTEM_INFO, NULL, 0,
                                   OSLS_VM_CAP_HOST_READ,
                                   json, 8, &result) == -1);
    assert(result.utf8 == NULL);

    assert(osls_vm_call_typed(-1, NULL, 0, json, sizeof(json), &result) == -1);
    assert(osls_vm_call_typed(OSLS_VM_SYSTEM_INFO, NULL, 1,
                              json, sizeof(json), &result) == -1);
}

static void test_native_invocations(void) {
    char json[65536];
    osls_vm_result result;
    osls_vm_value arg[2];
    int n;

    n = osls_vm_call_typed(OSLS_VM_SYSTEM_INFO, NULL, 0,
                           json, sizeof(json), &result);
    assert(n > 0 && result.type == OSLS_VM_RESULT_JSON_UTF8);
    assert(result.utf8 == json && result.length == (size_t)n);
    assert(strstr(json, "\"status\":0") != NULL);

    arg[0].type = OSLS_VM_ARG_I64; arg[0].value.i64 = 0;
    n = osls_vm_call_typed(OSLS_VM_PROCESS_INFO, arg, 1, json, sizeof(json), &result);
    assert(n > 0 && result.type == OSLS_VM_RESULT_JSON_UTF8);

    n = osls_vm_call_typed(OSLS_VM_ENUMERATE_PROCESSES, NULL, 0, json, sizeof(json), &result); assert(n > 0);
    arg[0].type = OSLS_VM_ARG_UTF8; arg[0].value.utf8 = "PATH";
    n = osls_vm_call_typed(OSLS_VM_GET_ENVIRONMENT_VALUE, arg, 1, json, sizeof(json), &result); assert(n > 0);
    n = osls_vm_call_typed(OSLS_VM_ENUMERATE_ENVIRONMENT, NULL, 0, json, sizeof(json), &result); assert(n > 0);
    arg[0].value.utf8 = ".";
    n = osls_vm_call_typed(OSLS_VM_GET_FILE_INFO, arg, 1, json, sizeof(json), &result); assert(n > 0);
    n = osls_vm_call_typed(OSLS_VM_LIST_DIRECTORY, arg, 1, json, sizeof(json), &result); assert(n > 0);
    n = osls_vm_call_typed(OSLS_VM_GET_CPU_INFO, NULL, 0, json, sizeof(json), &result); assert(n > 0);
    n = osls_vm_call_typed(OSLS_VM_GET_TIMEZONE_INFO, NULL, 0, json, sizeof(json), &result); assert(n > 0);
    n = osls_vm_call_typed(OSLS_VM_GET_NETWORK_INTERFACES, NULL, 0, json, sizeof(json), &result); assert(n > 0);

    arg[0].value.utf8 = "* * * * *";
    n = osls_vm_call_typed(OSLS_VM_CRON_VALIDATE, arg, 1, json, sizeof(json), &result);
    assert(n > 0 && strstr(json, "\"status\":0") != NULL);
    arg[1].type = OSLS_VM_ARG_I64; arg[1].value.i64 = 0;
    n = osls_vm_call_typed(OSLS_VM_CRON_NEXT_AFTER, arg, 2, json, sizeof(json), &result); assert(n > 0);

    /* Compatibility entry point still resolves through the executable registry. */
    n = osls_vm_call(OSLS_VM_GET_FILE_INFO, ".", 0, json, sizeof(json));
    assert(n > 0 && strstr(json, "\"status\":0") != NULL);
}

int main(void) {
    test_registry();
    test_typed_validation();
    test_native_invocations();
    puts("OSLS VM binding registry and typed marshalling: PASS (12 bindings)");
    return 0;
}
