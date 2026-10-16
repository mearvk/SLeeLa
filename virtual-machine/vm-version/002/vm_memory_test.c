/*
 * virtual-machine/vm-version/002/vm_memory_test.c
 * VM v002 memory-hardening smoke test: drive usage across the watermarks and
 * confirm edge-triggered pressure reporting through vm_report.
 * Build: gcc -std=c11 -DVM_HAVE_MEMMGR -I. vm_memory_test.c vm_memory.c
 *        vm_config.c vm_report.c vm_status.c sleela_memmgr.c sleela_memory.c
 *        sleela_thread.c -pthread
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_memory.h"
#include "sleela_memmgr.h"

#include <stdio.h>

int main(void) {
    /* Small limit so we can cross watermarks cheaply: 256 MiB (the floor). */
    VmConfig c = vm_config_defaults();
    vm_config_set_memory(&c, 256ull * 1024 * 1024);
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_apply(&c);
    vm_memory_set_watermarks(0.50, 0.80);   /* warn at 50%, critical at 80% */

    char line[160];
    void *blocks[64]; int n = 0;
    const size_t chunk = 16ull * 1024 * 1024;   /* 16 MiB each */

    printf("step  usage  condition\n");
    for (int i = 0; i < 20; ++i) {
        void *p = slmm_alloc(chunk);
        if (p) blocks[n++] = p;
        VmMemCondition cond = vm_memory_poll();   /* reports on crossings */
        vm_memory_report(line, sizeof line);
        printf("%4d  %5.1f%%  %s\n", i,
               vm_memory_usage_fraction() * 100.0,
               vm_memory_condition_name(cond));
        if (cond == VM_MEM_LIMIT) break;
    }

    /* free everything -> should report a single recovery notice */
    for (int i = 0; i < n; ++i) slmm_free(blocks[i]);
    vm_memory_poll();
    vm_memory_report(line, sizeof line);
    printf("after free: %s\n", line);
    return 0;
}
