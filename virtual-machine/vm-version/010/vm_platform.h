/*
 * virtual-machine/vm-version/008/vm_platform.h
 * VM v008 -- portability / platform-parity readout.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Builds on v001-007. The core knows the host (sleela_platform: Win32/POSIX/
 * Apple) and which capabilities the OS actually provides, but a user has no
 * single view tying that to the VM's own capability model (v001/v007). v008
 * adds a parity readout:
 *
 *   - the active platform + version + architecture;
 *   - per-capability parity: is it supported by the OS AND permitted by config?
 *   - a typed warning (via v003) for any capability the user permitted but the
 *     platform does not actually support -- so cross-OS surprises surface early.
 *
 * Pure C over sleela_platform when VM_HAVE_PLATFORM is defined; otherwise it
 * reports the compile-time OS so the layer builds standalone.
 */
#ifndef VM_PLATFORM_H
#define VM_PLATFORM_H

#include "vm_config.h"   /* VmCapability */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VM_PLATFORM_UNKNOWN = 0,
    VM_PLATFORM_LINUX   = 1,
    VM_PLATFORM_WINDOWS = 2,
    VM_PLATFORM_MACOS   = 3
} VmPlatform;

/* Detect and cache the host platform. Call once at startup. */
void vm_platform_init(void);

/* The active platform and its human-readable name. */
VmPlatform  vm_platform_current(void);
const char *vm_platform_name(VmPlatform p);

/* Does the OS actually provide `cap`? (1/0) */
int vm_platform_supports(VmCapability cap);

/* Parity check for one capability: 1 if (OS supports) AND (config permits).
 * If the config permits a capability the OS does NOT support, emits ONE typed
 * UNAVAILABLE warning (via v003) the first time it is observed. */
int vm_platform_parity(VmCapability cap);

/* Scan every capability, emitting parity warnings for permitted-but-
 * unsupported ones. Returns the count of such mismatches. */
int vm_platform_audit(void);

/* Render a parity table (platform header + per-capability os/config/parity). */
int vm_platform_report(char *buf, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* VM_PLATFORM_H */
