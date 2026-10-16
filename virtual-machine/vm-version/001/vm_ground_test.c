/*
 * virtual-machine/vm-version/001/vm_ground_test.c
 * VM v001 ground-floor smoke test (config + report + status).
 * Idea System(TM) / Idea(TM). From Author; distributive.
 *
 * Build (standalone):  gcc -std=c11 -I. vm_ground_test.c vm_config.c vm_report.c vm_status.c -o t
 * Build (with memmgr): add -DVM_HAVE_MEMMGR sleela_memmgr.c sleela_memory.c -pthread
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_status.h"
#include <stdio.h>

int main(void) {
    VmConfig c = vm_config_defaults();
    vm_config_set_memory(&c, 512ull * 1024 * 1024);   /* 512 MiB */
    vm_config_set_threads(&c, 32);
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_disable(&c, VM_CAP_NETWORK);
    vm_config_apply(&c);

    vm_status_banner();
    vm_notice("vm", "configuration applied");
    vm_error("memory", "LIMIT", "allocation would exceed %llu MiB", 2048ull);
    vm_debug("vm", "suppressed unless verbosity=debug");
    return 0;
}
