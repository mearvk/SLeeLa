/*
 * virtual-machine/vm-version/005/vm_gc_test.c  — VM v005 GC-quality smoke test.
 * Build: gcc -std=c11 -I. vm_gc_test.c vm_gc.c vm_telemetry.c vm_result.c
 *        vm_config.c vm_report.c -pthread
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_telemetry.h"
#include "vm_gc.h"
#include <stdio.h>

int main(void) {
    VmConfig c = vm_config_defaults();
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_apply(&c);
    vm_telemetry_init();
    vm_gc_init();
    vm_gc_set_thresholds(0.25, 50.0);

    char line[200];
    /* healthy: lots reclaimed per alloc */
    VmGcSnapshot s = {0,0,0,0,0,0};
    s.allocated=1000; s.reclaimed=900; s.collections=2; vm_gc_observe(&s);
    vm_gc_report(line,sizeof line); printf("%s\n", line);

    /* pressure: allocation outpaces reclamation */
    s.allocated=4000; s.reclaimed=1200; s.collections=6; vm_gc_observe(&s);
    vm_gc_report(line,sizeof line); printf("%s  collect_young=%d\n", line, vm_gc_should_collect_young());

    /* thrash: many collections, little reclaimed */
    s.allocated=4500; s.reclaimed=1210; s.collections=200; vm_gc_observe(&s);
    vm_gc_report(line,sizeof line); printf("%s\n", line);

    /* recovery */
    s.allocated=5500; s.reclaimed=2210; s.collections=201; vm_gc_observe(&s);
    vm_gc_report(line,sizeof line); printf("%s\n", line);

    char t[256]; vm_telemetry_report(t,sizeof t); printf("%s", t);
    return 0;
}
