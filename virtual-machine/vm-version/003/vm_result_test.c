/*
 * virtual-machine/vm-version/003/vm_result_test.c
 * VM v003 typed-result smoke test.
 * Build: gcc -std=c11 -I. vm_result_test.c vm_result.c vm_config.c vm_report.c
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_result.h"

#include <stdio.h>

/* A bridge that used to return a raw int now returns a typed result. */
static VmResult open_handle(int bad) {
    if (bad) return vm_fail(VM_E_INVALID, "net", "bad socket handle %d", bad);
    return vm_ok();
}

int main(void) {
    VmConfig c = vm_config_defaults();
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_apply(&c);

    VmResult a = open_handle(0);
    printf("ok result: is_ok=%d code=%s\n", vm_is_ok(&a), vm_result_code_name(a.code));

    VmResult b = open_handle(-5);
    printf("fail result: is_ok=%d code=%s subsystem=%s\n",
           vm_is_ok(&b), vm_result_code_name(b.code), b.subsystem);
    vm_result_report(&b);   /* -> [idea-vm] WARN (net/INVALID): bad socket handle -5 */

    VmResult lim = vm_fail(VM_E_LIMIT, "memory", "would exceed %llu MiB", 2048ull);
    vm_result_report(&lim); /* -> ERROR severity */

    /* legacy status mapping */
    printf("map(0)=%s map(-2)=%s map(-3)=%s map(-99)=%s\n",
           vm_result_code_name(vm_result_from_status(0)),
           vm_result_code_name(vm_result_from_status(-2)),
           vm_result_code_name(vm_result_from_status(-3)),
           vm_result_code_name(vm_result_from_status(-99)));
    return 0;
}
