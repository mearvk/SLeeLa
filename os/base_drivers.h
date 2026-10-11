/*
 * os/base_drivers.h
 * SLeeLa OS base-driver interface — the minimum-boot driver series contract.
 *
 * This header is the C ABI the generated base-driver table (base_drivers_table.c,
 * emitted by lib/os/os-creator/SLBaseDriverEmitter from an SLBaseDriverSeries)
 * plugs into. It declares the driver-class enumeration, a driver record, the
 * registration/probe/boot order a kernel's early init walks, and the driver
 * SOURCE policy (in-tree / vendor-signed / unknown / foreign) plus the cross-OS
 * foreign-driver bridge. The implementation lives in base_drivers.c / .cpp.
 *
 * Boundary: these are the SLeeLa-side contracts the native kernel/initramfs
 * layer implements against; loading a real driver crosses the kernel boundary.
 */
#ifndef SLEELA_OS_BASE_DRIVERS_H
#define SLEELA_OS_BASE_DRIVERS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Device classes in the minimum-boot series, in bring-up order. */
typedef enum sleela_driver_class {
    SL_DRV_BUS = 0,       /* motherboard/chipset bus (PCI/ACPI) — first */
    SL_DRV_STORAGE,       /* storage controller — must reach the rootfs */
    SL_DRV_DISPLAY,       /* console/framebuffer */
    SL_DRV_KEYBOARD,      /* keyboard */
    SL_DRV_MOUSE,         /* pointer */
    SL_DRV_USB,           /* USB host controller */
    SL_DRV_AUDIO,         /* audio — never boot-required */
    SL_DRV_CLASS_COUNT
} sleela_driver_class;

/* When a driver must be available. */
typedef enum sleela_driver_phase {
    SL_PHASE_BOOT = 0,    /* built-in / initramfs — present before rootfs mount */
    SL_PHASE_EARLY        /* available early in userspace, not boot-required */
} sleela_driver_phase;

/* Where a driver comes from, and how far it is trusted. */
typedef enum sleela_driver_origin {
    SL_ORIGIN_INTREE = 0, /* shipped with the OS */
    SL_ORIGIN_VENDOR,     /* signed by a trusted vendor */
    SL_ORIGIN_UNKNOWN,    /* unsigned / out-of-tree */
    SL_ORIGIN_FOREIGN     /* written for another OS family */
} sleela_driver_origin;

/* Admission result for a driver load request. */
typedef enum sleela_driver_admit {
    SL_ADMIT_LOAD = 0,        /* load, clean */
    SL_ADMIT_LOAD_TAINT = 1,  /* load, but mark the kernel tainted (unsigned) */
    SL_ADMIT_LOAD_BRIDGE = 2, /* load a foreign driver through a bridge */
    SL_ADMIT_REFUSED = -1,    /* refused by policy */
    SL_ADMIT_UNSUPPORTED = -2 /* no bridge exists for this foreign driver */
} sleela_driver_admit;

/* One driver record in the base series. */
typedef struct sleela_driver {
    sleela_driver_class  cls;
    const char          *name;     /* driver module/name, e.g. "ahci" */
    sleela_driver_phase  phase;
    sleela_driver_origin origin;
} sleela_driver;

/* The generated table (base_drivers_table.c) exposes these. */
const sleela_driver *sleela_base_drivers(void);   /* the ordered series */
size_t sleela_base_driver_count(void);
const char *sleela_os_host_family(void);          /* "Linux"/"Windows"/"macOS" */

/* ---- Boot bring-up ---------------------------------------------------- */
/* Register and probe the series in class order; stops and returns non-zero on
 * the first boot-phase driver that fails to bind. Returns 0 when every
 * boot-required driver bound. */
int sleela_base_drivers_bringup(void);

/* ---- Driver source policy + cross-OS bridge --------------------------- */
/* Admit a driver of a given origin (and, when foreign, a source OS family +
 * device class) under the current policy. Mirrors lib/os/SLDriverSource.admit. */
sleela_driver_admit sleela_driver_admit_check(sleela_driver_origin origin,
                                              const char *foreign_family,
                                              const char *device_class);

/* The bridge that adapts a foreign driver to this host, or "none".
 * e.g. (Linux host, "Windows", "network") -> "ndiswrapper". */
const char *sleela_driver_bridge(const char *foreign_family,
                                 const char *device_class);

/* Set policy: allow unsigned/unknown-source drivers (taint), and allow foreign
 * drivers where a bridge exists. */
void sleela_driver_policy(int allow_unknown, int allow_foreign);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_BASE_DRIVERS_H */
