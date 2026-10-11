/*
 * os/base_drivers.c
 * SLeeLa OS base-driver runtime — minimum-boot bring-up + driver source policy.
 *
 * Implements the contract in base_drivers.h against the generated series table
 * (base_drivers_table.c). The bring-up walks the series in class order and
 * binds each driver through the kernel's register/probe hooks (declared here as
 * extern; the native kernel layer provides them). The source policy decides
 * whether an in-tree / vendor / unknown / foreign driver may load, and the
 * bridge names the adapter for a foreign driver (NDISwrapper for Windows NDIS
 * network drivers on Linux, a FUSE shim for a foreign filesystem, else none).
 */
#include "base_drivers.h"
#include <string.h>

/* Provided by the native kernel layer (one per phase of a driver's life). */
extern int  kernel_register_driver(const char *name, int cls);
extern int  kernel_probe_driver(const char *name);
extern void kernel_taint(const char *reason);

/* Current policy (set by sleela_driver_policy; safe defaults). */
static int g_allow_unknown = 1;   /* permissive: load unsigned with a taint */
static int g_allow_foreign = 1;   /* bridge-only, where a bridge exists      */

void sleela_driver_policy(int allow_unknown, int allow_foreign) {
    g_allow_unknown = allow_unknown ? 1 : 0;
    g_allow_foreign = allow_foreign ? 1 : 0;
}

const char *sleela_driver_bridge(const char *foreign_family,
                                 const char *device_class) {
    const char *host = sleela_os_host_family();
    if (foreign_family && host && strcmp(foreign_family, host) == 0) return "native";
    if (host && strcmp(host, "Linux") == 0 &&
        foreign_family && strcmp(foreign_family, "Windows") == 0) {
        if (device_class && strcmp(device_class, "network") == 0)    return "ndiswrapper";
        if (device_class && strcmp(device_class, "filesystem") == 0) return "fuse-shim";
        return "none";                       /* gpu/storage/audio: no in-kernel bridge */
    }
    if (device_class && strcmp(device_class, "passthrough") == 0) return "vm-passthrough";
    return "none";
}

sleela_driver_admit sleela_driver_admit_check(sleela_driver_origin origin,
                                              const char *foreign_family,
                                              const char *device_class) {
    switch (origin) {
        case SL_ORIGIN_INTREE:
        case SL_ORIGIN_VENDOR:
            return SL_ADMIT_LOAD;
        case SL_ORIGIN_UNKNOWN:
            return g_allow_unknown ? SL_ADMIT_LOAD_TAINT : SL_ADMIT_REFUSED;
        case SL_ORIGIN_FOREIGN:
        default:
            if (!g_allow_foreign) return SL_ADMIT_REFUSED;
            if (strcmp(sleela_driver_bridge(foreign_family, device_class), "none") == 0)
                return SL_ADMIT_UNSUPPORTED;
            return SL_ADMIT_LOAD_BRIDGE;
    }
}

int sleela_base_drivers_bringup(void) {
    const sleela_driver *d = sleela_base_drivers();
    size_t n = sleela_base_driver_count();
    for (size_t i = 0; i < n; ++i) {
        sleela_driver_admit a =
            sleela_driver_admit_check(d[i].origin, sleela_os_host_family(), "any");
        if (a == SL_ADMIT_REFUSED || a == SL_ADMIT_UNSUPPORTED) {
            if (d[i].phase == SL_PHASE_BOOT) return 1;   /* a required driver cannot load */
            continue;                                     /* skip a non-boot driver */
        }
        if (a == SL_ADMIT_LOAD_TAINT) kernel_taint("unsigned driver");
        if (kernel_register_driver(d[i].name, (int)d[i].cls) != 0) {
            if (d[i].phase == SL_PHASE_BOOT) return 2;
            continue;
        }
        if (kernel_probe_driver(d[i].name) != 0) {
            if (d[i].phase == SL_PHASE_BOOT) return 3;    /* boot driver failed to bind */
        }
    }
    return 0;
}
