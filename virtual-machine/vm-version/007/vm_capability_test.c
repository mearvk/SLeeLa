/*
 * virtual-machine/vm-version/007/vm_capability_test.c — VM v007 enforcement test.
 * Build: gcc -std=c11 -I. vm_capability_test.c vm_capability.c vm_result.c
 *        vm_telemetry.c vm_config.c vm_report.c -pthread
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_telemetry.h"
#include "vm_capability.h"
#include <stdio.h>

/* A simulated net bridge op guarded by the capability gate. */
static VmResult net_listen(void) {
    VmResult r;
    if (!vm_cap_check(VM_CAP_NETWORK, "net", "listen", &r)) return r;  /* denied */
    return vm_ok();   /* permitted -> would really bind */
}

int main(void) {
    vm_telemetry_init();

    VmConfig c = vm_config_defaults();           /* all caps on */
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_disable(&c, VM_CAP_NETWORK);        /* user denies networking */
    vm_config_apply(&c);

    VmResult a = net_listen();
    printf("net with NETWORK denied: ok=%d code=%s denials=%lu\n",
           vm_is_ok(&a), vm_result_code_name(a.code), vm_cap_denials());

    /* now permit it and retry */
    vm_config_enable(&c, VM_CAP_NETWORK);
    vm_config_apply(&c);
    VmResult b = net_listen();
    printf("net with NETWORK allowed: ok=%d code=%s denials=%lu\n",
           vm_is_ok(&b), vm_result_code_name(b.code), vm_cap_denials());

    char t[256]; vm_telemetry_report(t,sizeof t); printf("%s", t);
    return 0;
}
