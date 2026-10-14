/*
 * virtual-machine/vm-version/007/vm_capability.c
 * VM v007 -- capability enforcement (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_capability.h"
#include "vm_report.h"
#include "vm_telemetry.h"

static unsigned long g_denials = 0;

int vm_cap_allowed(VmCapability cap) {
    const VmConfig *c = vm_config_current();
    if (!c) return 1;                 /* unconfigured -> permissive */
    return vm_config_has(c, cap);
}

int vm_cap_check(VmCapability cap, const char *subsystem, const char *operation,
                 VmResult *out) {
    if (vm_cap_allowed(cap)) return 1;

    g_denials++;
    vm_telemetry_add(VM_COUNTER_ERRORS, 1);
    VmResult r = vm_fail(VM_E_DENIED, subsystem ? subsystem : "vm",
                         "capability '%s' denied for operation '%s'",
                         vm_cap_name(cap), operation ? operation : "?");
    vm_result_report(&r);
    if (out) *out = r;
    return 0;
}

unsigned long vm_cap_denials(void) { return g_denials; }
void          vm_cap_reset(void)   { g_denials = 0; }

const char *vm_cap_name(VmCapability cap) {
    switch (cap) {
        case VM_CAP_MEMORY:   return "memory";
        case VM_CAP_FILE_IO:  return "file-io";
        case VM_CAP_NETWORK:  return "network";
        case VM_CAP_THREADS:  return "threads";
        case VM_CAP_DYNLIB:   return "dynlib";
        case VM_CAP_TERMINAL: return "terminal";
        case VM_CAP_PATHS:    return "paths";
        case VM_CAP_PROCESS:  return "process";
        case VM_CAP_IPC:      return "ipc";
        case VM_CAP_TIME:     return "time";
        case VM_CAP_ALL:      return "all";
        default:              return "capability";
    }
}
