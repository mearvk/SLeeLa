#include "osls_vm_bridge.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    size_t count = 0;
    const osls_vm_binding *table = osls_vm_binding_table(&count);
    char json[65536];
    int n;
    assert(table != NULL && count == OSLS_VM_OPERATION_COUNT);
    assert(strcmp(table[0].native_symbol, "osls_get_system_info") == 0);
    n = osls_vm_call(OSLS_VM_SYSTEM_INFO, NULL, 0, json, sizeof(json));
    assert(n > 0 && strstr(json, "\"status\":0") != NULL);
    n = osls_vm_call(OSLS_VM_CRON_VALIDATE, "* * * * *", 0, json, sizeof(json));
    assert(n > 0 && strstr(json, "\"status\":0") != NULL);
    n = osls_vm_call(OSLS_VM_GET_FILE_INFO, ".", 0, json, sizeof(json));
    assert(n > 0 && strstr(json, "\"status\":0") != NULL);
    puts("OSLS VM binding registry: PASS (12 bindings)");
    return 0;
}
