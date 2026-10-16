/*
 * virtual-machine/vm-version/004/vm_telemetry_test.c
 * VM v004 telemetry smoke test: counters + concurrent gauge + saturation watch.
 * Build: gcc -std=c11 -I. vm_telemetry_test.c vm_telemetry.c vm_result.c
 *        vm_config.c vm_report.c -pthread
 */
#include "vm_config.h"
#include "vm_report.h"
#include "vm_telemetry.h"
#include "sleela_bestof.h"   /* for the real SL_BESTOF_MAX */

#include <pthread.h>
#include <stdio.h>

static void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < 100000; ++i) vm_telemetry_add(VM_COUNTER_STEPS, 1);
    return NULL;
}

int main(void) {
    VmConfig c = vm_config_defaults();
    vm_config_set_verbosity(&c, VM_VERBOSITY_VERBOSE);
    vm_config_apply(&c);

    vm_telemetry_init();
    vm_telemetry_set_capacity(VM_TABLE_BESTOF, SL_BESTOF_MAX);  /* 32 */
    vm_telemetry_set_watermark(0.80);

    /* lock-free counter correctness under concurrency: 8 threads x 100000 */
    pthread_t th[8];
    for (int i = 0; i < 8; ++i) pthread_create(&th[i], NULL, worker, NULL);
    for (int i = 0; i < 8; ++i) pthread_join(th[i], NULL);
    printf("steps counter = %llu (expect 800000)\n",
           (unsigned long long)vm_telemetry_get(VM_COUNTER_STEPS));

    /* drive the best-of gauge up to capacity; expect ONE saturation warning */
    printf("filling best-of table (cap %d, watermark 80%%)...\n", SL_BESTOF_MAX);
    for (int live = 1; live <= SL_BESTOF_MAX; ++live)
        vm_telemetry_gauge(VM_TABLE_BESTOF, live);

    char buf[256];
    vm_telemetry_report(buf, sizeof buf);
    printf("%s", buf);
    return 0;
}
