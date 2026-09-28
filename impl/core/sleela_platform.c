#include "sleela_platform.h"
#include <stdio.h>
#include <string.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/utsname.h>
#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#endif

static SLPlatform compiled_platform(void) {
#if defined(_WIN32)
    return SL_PLATFORM_WINDOWS;
#elif defined(__APPLE__)
    return SL_PLATFORM_MACOS;
#elif defined(__linux__)
    return SL_PLATFORM_LINUX;
#else
    return SL_PLATFORM_UNKNOWN;
#endif
}
SLPlatform slplatform_current(void) { return compiled_platform(); }
const char *slplatform_name(SLPlatform p) {
    switch (p) {
        case SL_PLATFORM_LINUX: return "Linux";
        case SL_PLATFORM_WINDOWS: return "Windows";
        case SL_PLATFORM_MACOS: return "macOS";
        default: return "Unknown";
    }
}
int slplatform_detect(SLPlatformInfo *out) {
    if (!out) return -1;
    memset(out, 0, sizeof(*out));
    out->platform = compiled_platform();
    snprintf(out->name, sizeof(out->name), "%s", slplatform_name(out->platform));
#if defined(_WIN32)
    SYSTEM_INFO si;
    OSVERSIONINFOEXA vi;
    memset(&si, 0, sizeof(si)); GetNativeSystemInfo(&si);
    snprintf(out->architecture, sizeof(out->architecture), "%s",
        si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? "x86_64" :
        si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64 ? "arm64" : "other");
    memset(&vi, 0, sizeof(vi)); vi.dwOSVersionInfoSize = sizeof(vi);
    if (GetVersionExA((OSVERSIONINFOA *)&vi))
        snprintf(out->version, sizeof(out->version), "%lu.%lu build %lu",
                 (unsigned long)vi.dwMajorVersion, (unsigned long)vi.dwMinorVersion,
                 (unsigned long)vi.dwBuildNumber);
    else snprintf(out->version, sizeof(out->version), "unknown");
    snprintf(out->runtime, sizeof(out->runtime), "Win32");
#elif defined(__APPLE__)
    struct utsname u;
    if (uname(&u) == 0) {
        snprintf(out->version, sizeof(out->version), "Darwin %s", u.release);
        snprintf(out->architecture, sizeof(out->architecture), "%s", u.machine);
    } else { snprintf(out->version, sizeof(out->version), "unknown"); snprintf(out->architecture, sizeof(out->architecture), "unknown"); }
    {
        char product[64]; size_t n=sizeof(product);
        if (sysctlbyname("kern.osproductversion", product, &n, NULL, 0) == 0 && n > 1)
            snprintf(out->version, sizeof(out->version), "macOS %s", product);
    }
    snprintf(out->runtime, sizeof(out->runtime), "Apple/libSystem");
#elif defined(__linux__)
    struct utsname u;
    if (uname(&u) == 0) {
        snprintf(out->version, sizeof(out->version), "Linux %s", u.release);
        snprintf(out->architecture, sizeof(out->architecture), "%s", u.machine);
    } else { snprintf(out->version, sizeof(out->version), "unknown"); snprintf(out->architecture, sizeof(out->architecture), "unknown"); }
    snprintf(out->runtime, sizeof(out->runtime), "POSIX/libc");
#else
    snprintf(out->version, sizeof(out->version), "unknown");
    snprintf(out->architecture, sizeof(out->architecture), "unknown");
    snprintf(out->runtime, sizeof(out->runtime), "unknown");
#endif
    return 0;
}
int slplatform_capability_for(SLPlatform p, SLPlatformCapability c) {
    if (p == SL_PLATFORM_UNKNOWN || c < 0 || c >= SL_CAP_MAX) return 0;
    switch (c) {
        case SL_CAP_MEMORY: case SL_CAP_FILE_IO: case SL_CAP_THREADS:
        case SL_CAP_PATHS: case SL_CAP_TIME: case SL_CAP_TERMINAL: return 1;
        case SL_CAP_NETWORK: case SL_CAP_DYNAMIC_LIBRARY: case SL_CAP_PROCESS:
        case SL_CAP_IPC: return p == SL_PLATFORM_LINUX || p == SL_PLATFORM_WINDOWS || p == SL_PLATFORM_MACOS;
        default: return 0;
    }
}
int slplatform_capability_available(SLPlatformCapability c) { return slplatform_capability_for(compiled_platform(), c); }
int slplatform_is_supported_version(const SLPlatformInfo *info) {
    return info && (info->platform == SL_PLATFORM_LINUX || info->platform == SL_PLATFORM_WINDOWS || info->platform == SL_PLATFORM_MACOS);
}
int slplatform_format_report(char *buf, size_t cap) {
    SLPlatformInfo i; if (slplatform_detect(&i) != 0) return -1;
    return snprintf(buf, cap,
        "[platform] OS=%s version=%s arch=%s runtime=%s\n"
        "[platform] memory=%d file=%d network=%d threads=%d library=%d terminal=%d paths=%d process=%d ipc=%d time=%d\n",
        i.name,i.version,i.architecture,i.runtime,
        slplatform_capability_available(SL_CAP_MEMORY), slplatform_capability_available(SL_CAP_FILE_IO),
        slplatform_capability_available(SL_CAP_NETWORK), slplatform_capability_available(SL_CAP_THREADS),
        slplatform_capability_available(SL_CAP_DYNAMIC_LIBRARY), slplatform_capability_available(SL_CAP_TERMINAL),
        slplatform_capability_available(SL_CAP_PATHS), slplatform_capability_available(SL_CAP_PROCESS),
        slplatform_capability_available(SL_CAP_IPC), slplatform_capability_available(SL_CAP_TIME));
}
