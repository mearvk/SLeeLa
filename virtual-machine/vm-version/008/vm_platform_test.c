/*
 * virtual-machine/vm-version/008/vm_platform_test.c — VM v008 parity test.
 * Build: gcc -std=c11 -I. vm_platform_test.c vm_platform.c vm_capability.c
 *        vm_result.c vm_telemetry.c vm_config.c vm_report.c -pthread
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_platform.h"
#include <stdio.h>

int main(void) {
    VmConfig c = vm_config_defaults();
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_apply(&c);
    vm_platform_init();

    printf("detected platform: %s\n", vm_platform_name(vm_platform_current()));

    int mism = vm_platform_audit();
    printf("permitted-but-unsupported capabilities: %d\n", mism);

    char buf[512]; vm_platform_report(buf, sizeof buf);
    printf("%s", buf);
    return 0;
}
