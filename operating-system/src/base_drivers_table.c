/*
 * operating-system/src/base_drivers_table.c
 * The default minimum-boot driver series table + host identity + the kernel
 * bring-up hooks, so /os builds and tests as a self-contained static library.
 *
 * This is the DEFAULT, reference table. In a real OS build the OS Creator
 * (lib/operating-system/os-creator/SLBaseDriverEmitter.sleela, mirrored by
 * lib/operating-system/os-generate.sleela baseDriversTableC) emits a shape-specific
 * base_drivers_table.c that REPLACES this one. The three accessors declared in
 * base_drivers.h are satisfied here; the extern kernel_* hooks are provided as
 * safe no-op stubs so sleela_base_drivers_bringup() links and runs in a host
 * tool/test (a real kernel layer supplies the genuine hooks).
 */
#include "base_drivers.h"
#include <stddef.h>

/* The ordered minimum-boot series: bus -> storage -> display -> input -> usb,
 * with audio as a non-boot (early) driver. Names are generic Linux-shape
 * defaults; the emitted table uses the chosen shape's real module names. */
static const sleela_driver k_default_series[] = {
    { SL_DRV_BUS,      "pci-acpi", SL_PHASE_BOOT,  SL_ORIGIN_INTREE },
    { SL_DRV_STORAGE,  "ahci",     SL_PHASE_BOOT,  SL_ORIGIN_INTREE },
    { SL_DRV_DISPLAY,  "fbcon",    SL_PHASE_BOOT,  SL_ORIGIN_INTREE },
    { SL_DRV_KEYBOARD, "atkbd",    SL_PHASE_BOOT,  SL_ORIGIN_INTREE },
    { SL_DRV_MOUSE,    "psmouse",  SL_PHASE_EARLY, SL_ORIGIN_INTREE },
    { SL_DRV_USB,      "xhci-hcd", SL_PHASE_BOOT,  SL_ORIGIN_INTREE },
    { SL_DRV_AUDIO,    "snd-hda",  SL_PHASE_EARLY, SL_ORIGIN_VENDOR }
};

const sleela_driver *sleela_base_drivers(void) {
    return k_default_series;
}

size_t sleela_base_driver_count(void) {
    return sizeof(k_default_series) / sizeof(k_default_series[0]);
}

/* Host family the admission policy/bridge reasons about. The real table is
 * emitted for the target shape; the default reports Linux. */
const char *sleela_os_host_family(void) {
    return "Linux";
}

/* ---- Default kernel bring-up hooks (no-op stubs) ------------------------
 * base_drivers.c declares these extern; a real kernel layer provides them. The
 * stubs let the series bring-up link and succeed in a host tool/test without a
 * kernel, registering/probing nothing and tainting nothing.
 * They are weak so a linked-in real kernel layer overrides them cleanly. */
#if defined(__GNUC__)
__attribute__((weak)) int  kernel_register_driver(const char *name, int cls) { (void)name; (void)cls; return 0; }
__attribute__((weak)) int  kernel_probe_driver(const char *name)             { (void)name; return 0; }
__attribute__((weak)) void kernel_taint(const char *reason)                  { (void)reason; }
#else
int  kernel_register_driver(const char *name, int cls) { (void)name; (void)cls; return 0; }
int  kernel_probe_driver(const char *name)             { (void)name; return 0; }
void kernel_taint(const char *reason)                  { (void)reason; }
#endif
