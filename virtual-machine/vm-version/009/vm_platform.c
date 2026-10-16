/*
 * virtual-machine/vm-version/008/vm_platform.c
 * VM v008 -- platform-parity readout (implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 */
#include "vm_platform.h"
#include "vm_capability.h"
#include "vm_result.h"
#include "vm_report.h"

#include <stdio.h>
#include <string.h>

#if defined(VM_HAVE_PLATFORM)
#include "sleela_platform.h"
#endif

static VmPlatform g_platform = VM_PLATFORM_UNKNOWN;
static char       g_version[96] = "";
static char       g_arch[32]    = "";
static int        g_warned[16];   /* edge-trigger per VmCapability bit index */

/* Map our VmCapability bit to the platform enum ordinal. */
static int cap_ordinal(VmCapability cap) {
    switch (cap) {
        case VM_CAP_MEMORY:   return 0;
        case VM_CAP_FILE_IO:  return 1;
        case VM_CAP_NETWORK:  return 2;
        case VM_CAP_THREADS:  return 3;
        case VM_CAP_DYNLIB:   return 4;
        case VM_CAP_TERMINAL: return 5;
        case VM_CAP_PATHS:    return 6;
        case VM_CAP_PROCESS:  return 7;
        case VM_CAP_IPC:      return 8;
        case VM_CAP_TIME:     return 9;
        default:              return -1;
    }
}

void vm_platform_init(void) {
    memset(g_warned, 0, sizeof g_warned);
#if defined(VM_HAVE_PLATFORM)
    SLPlatformInfo info;
    if (slplatform_detect(&info) == 0) {
        g_platform = (VmPlatform)info.platform;
        snprintf(g_version, sizeof g_version, "%s", info.version);
        snprintf(g_arch, sizeof g_arch, "%s", info.architecture);
        return;
    }
#endif
#if defined(_WIN32)
    g_platform = VM_PLATFORM_WINDOWS;
#elif defined(__APPLE__)
    g_platform = VM_PLATFORM_MACOS;
#elif defined(__linux__) || defined(__unix__)
    g_platform = VM_PLATFORM_LINUX;
#else
    g_platform = VM_PLATFORM_UNKNOWN;
#endif
}

VmPlatform vm_platform_current(void) { return g_platform; }

const char *vm_platform_name(VmPlatform p) {
    switch (p) {
        case VM_PLATFORM_LINUX:   return "linux";
        case VM_PLATFORM_WINDOWS: return "windows";
        case VM_PLATFORM_MACOS:   return "macos";
        default:                  return "unknown";
    }
}

int vm_platform_supports(VmCapability cap) {
#if defined(VM_HAVE_PLATFORM)
    int ord = cap_ordinal(cap);
    if (ord < 0) return 0;
    return slplatform_capability_available((SLPlatformCapability)ord);
#else
    /* Without the platform layer linked, assume the common POSIX/Win set is
     * available; dynlib/ipc are the usual cross-OS variance points. */
    (void)cap;
    return 1;
#endif
}

int vm_platform_parity(VmCapability cap) {
    int os = vm_platform_supports(cap);
    int permitted = vm_cap_allowed(cap);
    if (permitted && !os) {
        int ord = cap_ordinal(cap);
        if (ord >= 0 && ord < 16 && !g_warned[ord]) {
            g_warned[ord] = 1;
            VmResult r = vm_fail(VM_E_UNAVAILABLE, "platform",
                                 "capability '%s' permitted but not supported on %s",
                                 vm_cap_name(cap), vm_platform_name(g_platform));
            vm_result_report(&r);
        }
        return 0;
    }
    return (permitted && os) ? 1 : 0;
}

int vm_platform_audit(void) {
    const VmCapability all[] = {
        VM_CAP_MEMORY, VM_CAP_FILE_IO, VM_CAP_NETWORK, VM_CAP_THREADS,
        VM_CAP_DYNLIB, VM_CAP_TERMINAL, VM_CAP_PATHS, VM_CAP_PROCESS,
        VM_CAP_IPC, VM_CAP_TIME
    };
    int mismatches = 0;
    for (size_t i = 0; i < sizeof all / sizeof all[0]; ++i) {
        if (vm_cap_allowed(all[i]) && !vm_platform_supports(all[i])) {
            vm_platform_parity(all[i]);  /* emit the warning once */
            mismatches++;
        }
    }
    return mismatches;
}

int vm_platform_report(char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    int n = snprintf(buf, cap, "platform: %s %s %s\n",
                     vm_platform_name(g_platform),
                     g_version[0] ? g_version : "(ver n/a)",
                     g_arch[0] ? g_arch : "");
    const struct { VmCapability c; const char *name; } rows[] = {
        {VM_CAP_MEMORY,"memory"},{VM_CAP_FILE_IO,"file-io"},{VM_CAP_NETWORK,"network"},
        {VM_CAP_THREADS,"threads"},{VM_CAP_DYNLIB,"dynlib"},{VM_CAP_TERMINAL,"terminal"},
        {VM_CAP_PATHS,"paths"},{VM_CAP_PROCESS,"process"},{VM_CAP_IPC,"ipc"},{VM_CAP_TIME,"time"}
    };
    for (size_t i = 0; i < sizeof rows / sizeof rows[0] && n > 0 && (size_t)n < cap; ++i) {
        int os = vm_platform_supports(rows[i].c);
        int cfg = vm_cap_allowed(rows[i].c);
        n += snprintf(buf + n, cap - (size_t)n,
                      "  %-9s os=%s config=%s parity=%s\n", rows[i].name,
                      os ? "yes" : "no", cfg ? "on" : "off",
                      (os && cfg) ? "ok" : (cfg && !os ? "UNAVAILABLE" : "-"));
    }
    return n;
}
