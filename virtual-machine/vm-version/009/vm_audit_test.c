/*
 * virtual-machine/vm-version/009/vm_audit_test.c — VM v009 audit-trail test.
 * Build: gcc -std=c11 -I. vm_audit_test.c vm_audit.c vm_result.c vm_config.c vm_report.c
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_result.h"
#include "vm_audit.h"
#include <stdio.h>

int main(void) {
    VmConfig c = vm_config_defaults();
    vm_config_set_verbosity(&c, VM_VERBOSITY_DEBUG);
    vm_config_apply(&c);
    vm_audit_init();

    vm_audit_record(VM_AUDIT_CONFIG, "vm", "configuration applied: 2048 MiB, 128 threads");
    VmResult d = vm_fail(VM_E_DENIED, "net", "capability 'network' denied for 'listen'");
    vm_audit_result(&d);
    vm_audit_record(VM_AUDIT_PRESSURE, "memory", "usage 81%% -- above critical");
    vm_audit_record(VM_AUDIT_SATURATE, "telemetry", "best-of table 26/32");

    printf("total=%llu retained=%d chain=%016llx\n",
           (unsigned long long)vm_audit_total(), vm_audit_count(),
           (unsigned long long)vm_audit_chain());

    /* tamper-evidence: recompute independently by replaying the same events
     * would reproduce the chain; mutating one event's order/content would not.
     * Here we just show the report + verify count. */
    char buf[800]; vm_audit_report(buf, sizeof buf, 10); printf("%s", buf);

    /* ring-overwrite sanity: push past capacity, retained caps at capacity */
    for (int i = 0; i < VM_AUDIT_CAPACITY + 50; ++i)
        vm_audit_record(VM_AUDIT_INFO, "loop", "event %d", i);
    printf("after flood: total=%llu retained=%d (cap %d)\n",
           (unsigned long long)vm_audit_total(), vm_audit_count(), VM_AUDIT_CAPACITY);
    return 0;
}
