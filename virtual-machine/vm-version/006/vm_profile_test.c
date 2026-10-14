/*
 * virtual-machine/vm-version/006/vm_profile_test.c — VM v006 profiler test.
 * Build: gcc -std=c11 -I. vm_profile_test.c vm_profile.c vm_telemetry.c
 *        vm_result.c vm_config.c vm_report.c -pthread
 */
#include "vm_config.h"
#include "vm_telemetry.h"
#include "vm_profile.h"
#include <stdio.h>

static volatile double sink = 0;
static void busy(int n){ for(int i=0;i<n;i++) sink += (double)i*1.000001; }

int main(void) {
    VmConfig c = vm_config_defaults(); vm_config_apply(&c);
    vm_telemetry_init();
    vm_profile_init();

    /* simulate a dispatch loop: arith hot, some calls, a few bridge ops */
    for (int i = 0; i < 5000; ++i) { uint64_t t=vm_profile_begin(VM_OP_ARITH); busy(50); vm_profile_end(VM_OP_ARITH,t); }
    for (int i = 0; i < 800;  ++i) { uint64_t t=vm_profile_begin(VM_OP_CALL);  busy(200); vm_profile_end(VM_OP_CALL,t); }
    for (int i = 0; i < 50;   ++i) { uint64_t t=vm_profile_begin(VM_OP_BRIDGE);busy(2000);vm_profile_end(VM_OP_BRIDGE,t); }
    for (int i = 0; i < 1200; ++i)   vm_profile_tick(VM_OP_LOADSTORE);

    printf("hot family: %s\n", vm_op_family_name(vm_profile_hot()));
    char buf[400]; vm_profile_report(buf,sizeof buf); printf("%s", buf);
    char t[256]; vm_telemetry_report(t,sizeof t); printf("%s", t);
    return 0;
}
