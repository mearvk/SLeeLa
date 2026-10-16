/*
 * virtual-machine/vm-version/010/vm_runtime_demo.c
 * VM v010 -- the consolidated runtime, end to end (the eventual program).
 * Build (full): gcc -std=c11 -DVM_HAVE_MEMMGR -DVM_HAVE_PLATFORM -I.
 *   vm_runtime_demo.c vm_runtime.c vm_config.c vm_report.c vm_status.c
 *   vm_memory.c vm_result.c vm_telemetry.c vm_gc.c vm_profile.c
 *   vm_capability.c vm_platform.c vm_audit.c
 *   sleela_memmgr.c sleela_memory.c sleela_thread.c sleela_platform.c -pthread
 */
#include "vm_runtime.h"
#include "vm_profile.h"
#include "vm_telemetry.h"
#include "sleela_bestof.h"

#include <stdio.h>

int main(void) {
    /* 1. configure + init the whole hardened VM */
    VmConfig c = vm_config_defaults();
    vm_config_set_memory(&c, 256ull * 1024 * 1024);  /* 256 MiB */
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_disable(&c, VM_CAP_PROCESS);            /* deny spawning processes */
    vm_runtime_init(&c);
    vm_runtime_register_table(VM_TABLE_BESTOF, SL_BESTOF_MAX);

    /* 2. simulate running a program: some dispatch + a guarded op */
    for (int i = 0; i < 2000; ++i) { uint64_t t=vm_profile_begin(VM_OP_ARITH); vm_profile_end(VM_OP_ARITH,t); }
    VmResult r;
    if (!vm_runtime_permit(VM_CAP_PROCESS, "process", "spawn", &r))
        printf("(process spawn correctly denied: %s)\n", vm_result_code_name(r.code));

    /* 3. drive best-of table toward saturation (watched) */
    for (int live = 1; live <= SL_BESTOF_MAX; ++live) vm_telemetry_gauge(VM_TABLE_BESTOF, live);

    /* 4. a GC snapshot showing pressure */
    VmGcSnapshot gc0 = {0,0,0,0,0,0}; vm_runtime_poll(&gc0);
    VmGcSnapshot gc1 = {6,6,1200,0,4000,0}; vm_runtime_poll(&gc1);

    /* 5. one consolidated report */
    char buf[1600];
    vm_runtime_report(buf, sizeof buf);
    printf("\n%s\n", buf);

    vm_runtime_shutdown();
    return 0;
}
