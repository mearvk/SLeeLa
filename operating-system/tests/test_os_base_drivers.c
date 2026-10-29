/* operating-system/tests/test_os_base_drivers.c -- default series + bring-up + policy. */
#include "base_drivers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    const sleela_driver *d = sleela_base_drivers();
    size_t n = sleela_base_driver_count();
    size_t i, boot = 0;

    assert(d != NULL);
    assert(n >= 1);
    assert(strcmp(sleela_os_host_family(), "Linux") == 0);

    /* first driver is the bus, in class order */
    assert(d[0].cls == SL_DRV_BUS);
    for (i = 0; i < n; ++i) if (d[i].phase == SL_PHASE_BOOT) ++boot;
    assert(boot >= 1); /* at least storage must be boot-required */

    /* bring-up succeeds against the no-op kernel stubs */
    assert(sleela_base_drivers_bringup() == 0);

    /* admission policy */
    sleela_driver_policy(1, 1);
    assert(sleela_driver_admit_check(SL_ORIGIN_INTREE, "Linux", "storage") == SL_ADMIT_LOAD);
    assert(sleela_driver_admit_check(SL_ORIGIN_UNKNOWN, "Linux", "any") == SL_ADMIT_LOAD_TAINT);

    /* cross-OS bridge: Windows network driver on a Linux host -> ndiswrapper */
    assert(strcmp(sleela_driver_bridge("Windows", "network"), "ndiswrapper") == 0);
    assert(strcmp(sleela_driver_bridge("Windows", "filesystem"), "fuse-shim") == 0);
    assert(strcmp(sleela_driver_bridge("Linux", "storage"), "native") == 0); /* same family */

    sleela_driver_policy(0, 0);
    assert(sleela_driver_admit_check(SL_ORIGIN_UNKNOWN, 0, 0) == SL_ADMIT_REFUSED);

    printf("test_os_base_drivers: ok\n");
    return 0;
}
