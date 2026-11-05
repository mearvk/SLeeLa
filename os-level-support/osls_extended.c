#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
/* Expose the BSD interfaces used by the macOS network headers even when the
 * build also defines _POSIX_C_SOURCE. This must precede every system header. */
#  define _DARWIN_C_SOURCE 1
#endif

#include "osls.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
#  include <iphlpapi.h>
#  include <process.h>
#  include <direct.h>
#  include <io.h>
#  include <intrin.h>
#  include <sys/stat.h>
#  include <Lmcons.h>
#  pragma comment(lib, "iphlpapi.lib")
#  pragma comment(lib, "ws2_32.lib")
#else
#  include <dirent.h>
#  include <unistd.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <fcntl.h>
#  include <limits.h>
#  include <pwd.h>
#  include <ifaddrs.h>
#  include <arpa/inet.h>
#  include <net/if.h>
#  if defined(__linux__)
#    include <netpacket/packet.h>
#    include <sys/sysinfo.h>
#    include <sys/utsname.h>
#  elif defined(__APPLE__)
#    include <mach-o/dyld.h>
#    include <net/if_dl.h>
#    include <mach/mach.h>
#    include <sys/sysctl.h>
#  endif
#endif

static int osls_copy(char *dst, size_t cap, const char *src) {
    size_t n;
    if (!dst || !cap || !src) return 0;
    n = strlen(src);
    if (n >= cap) return 0;
    memcpy(dst, src, n + 1);
    return 1;
}

static uint64_t osls_self_pid(void) {
#if defined(_WIN32)
    return (uint64_t)_getpid();
#else
    return (uint64_t)getpid();
#endif
}

static osls_status osls_fill_current_process(osls_process_info *out) {
    char cwd[OSLS_PATH_MAX];
    if (!out) return OSLS_INVALID_ARGUMENT;
    memset(out, 0, sizeof(*out));
    out->pid = osls_self_pid();
#if defined(_WIN32)
    if (!_getcwd(cwd, (int)sizeof(cwd)) || !osls_copy(out->current_directory, sizeof(out->current_directory), cwd)) return OSLS_SYSTEM_ERROR;
    if (!GetUserNameA(out->user_name, &(DWORD){(DWORD)sizeof(out->user_name)})) out->user_name[0] = '\0';
    if (!GetModuleFileNameA(NULL, out->executable_path, (DWORD)sizeof(out->executable_path))) return OSLS_SYSTEM_ERROR;
    osls_copy(out->command_line, sizeof(out->command_line), GetCommandLineA());
    osls_copy(out->state, sizeof(out->state), "running");
#else
    if (!getcwd(cwd, sizeof(cwd)) || !osls_copy(out->current_directory, sizeof(out->current_directory), cwd)) return OSLS_SYSTEM_ERROR;
    {
        struct passwd *pw = getpwuid(geteuid());
        if (pw && pw->pw_name) osls_copy(out->user_name, sizeof(out->user_name), pw->pw_name);
    }
    osls_copy(out->state, sizeof(out->state), "running");
#  if defined(__linux__)
    {
        char procpath[64];
        ssize_t n;
        snprintf(procpath, sizeof(procpath), "/proc/%llu/exe", (unsigned long long)out->pid);
        n = readlink(procpath, out->executable_path, sizeof(out->executable_path) - 1);
        if (n >= 0) out->executable_path[n] = '\0';
        snprintf(procpath, sizeof(procpath), "/proc/%llu/cmdline", (unsigned long long)out->pid);
        {
            int fd = open(procpath, O_RDONLY);
            if (fd >= 0) {
                n = read(fd, out->command_line, sizeof(out->command_line) - 1);
                close(fd);
                if (n > 0) {
                    ssize_t i;
                    for (i = 0; i < n; ++i) if (!out->command_line[i]) out->command_line[i] = ' ';
                    while (n > 0 && out->command_line[n - 1] == ' ') --n;
                    out->command_line[n] = '\0';
                }
            }
        }
        {
            FILE *f;
            char line[256];
            snprintf(procpath, sizeof(procpath), "/proc/%llu/status", (unsigned long long)out->pid);
            f = fopen(procpath, "r");
            if (f) {
                while (fgets(line, sizeof(line), f)) {
                    unsigned long long value;
                    if (sscanf(line, "Threads: %llu", &value) == 1) out->thread_count = (uint64_t)value;
                    if (sscanf(line, "PPid: %llu", &value) == 1) out->parent_pid = (uint64_t)value;
                }
                fclose(f);
            }
        }
    }
#  elif defined(__APPLE__)
    {
        uint32_t size = (uint32_t)sizeof(out->executable_path);
        if (_NSGetExecutablePath(out->executable_path, &size) != 0) out->executable_path[0] = '\0';
    }
#  endif
#endif
    out->start_epoch_seconds = (int64_t)time(NULL);
    return OSLS_OK;
}

osls_status osls_get_process_info(uint64_t pid, osls_process_info *out_info) {
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    if (pid != 0 && pid != osls_self_pid()) return OSLS_UNSUPPORTED;
    return osls_fill_current_process(out_info);
}

osls_status osls_enumerate_processes(osls_process_info *buffer, size_t buffer_count, size_t *out_count) {
    if (!out_count) return OSLS_INVALID_ARGUMENT;
    *out_count = 1;
    if (!buffer || buffer_count < 1) return OSLS_BUFFER_TOO_SMALL;
    return osls_fill_current_process(&buffer[0]);
}

osls_status osls_get_environment_value(const char *name, char *value_buffer, size_t value_capacity) {
    const char *value;
    if (!name || !*name || !value_buffer || !value_capacity) return OSLS_INVALID_ARGUMENT;
    value = getenv(name);
    if (!value) return OSLS_NOT_FOUND;
    if (!osls_copy(value_buffer, value_capacity, value)) return OSLS_BUFFER_TOO_SMALL;
    return OSLS_OK;
}

osls_status osls_enumerate_environment(osls_environment_entry *buffer, size_t buffer_count, size_t *out_count) {
#if defined(_WIN32)
    LPCH block, item;
    size_t count = 0;
    if (!out_count) return OSLS_INVALID_ARGUMENT;
    block = GetEnvironmentStringsA();
    if (!block) return OSLS_SYSTEM_ERROR;
    for (item = block; *item; item += strlen(item) + 1) ++count;
    *out_count = count;
    if (buffer_count < count || (count && !buffer)) { FreeEnvironmentStringsA(block); return OSLS_BUFFER_TOO_SMALL; }
    count = 0;
    for (item = block; *item; item += strlen(item) + 1) {
        char *eq = strchr(item, '=');
        size_t key_len;
        if (!eq || item[0] == '=') continue;
        key_len = (size_t)(eq - item);
        if (key_len >= sizeof(buffer[count].name) || strlen(eq + 1) >= sizeof(buffer[count].value)) { FreeEnvironmentStringsA(block); return OSLS_BUFFER_TOO_SMALL; }
        memcpy(buffer[count].name, item, key_len); buffer[count].name[key_len] = 0;
        osls_copy(buffer[count].value, sizeof(buffer[count].value), eq + 1);
        ++count;
    }
    *out_count = count;
    FreeEnvironmentStringsA(block);
    return OSLS_OK;
#else
    extern char **environ;
    size_t count = 0, i;
    if (!out_count) return OSLS_INVALID_ARGUMENT;
    while (environ[count]) ++count;
    *out_count = count;
    if (buffer_count < count || (count && !buffer)) return OSLS_BUFFER_TOO_SMALL;
    for (i = 0; i < count; ++i) {
        char *eq = strchr(environ[i], '=');
        size_t key_len;
        if (!eq) continue;
        key_len = (size_t)(eq - environ[i]);
        if (key_len >= sizeof(buffer[i].name) || strlen(eq + 1) >= sizeof(buffer[i].value)) return OSLS_BUFFER_TOO_SMALL;
        memcpy(buffer[i].name, environ[i], key_len); buffer[i].name[key_len] = 0;
        osls_copy(buffer[i].value, sizeof(buffer[i].value), eq + 1);
    }
    return OSLS_OK;
#endif
}

osls_status osls_get_file_info(const char *path, osls_file_info *out_info) {
#if defined(_WIN32)
    struct _stat64 st;
#else
    struct stat st;
#endif
    if (!path || !*path || !out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
    if (!osls_copy(out_info->path, sizeof(out_info->path), path)) return OSLS_BUFFER_TOO_SMALL;
#if defined(_WIN32)
    if (_stat64(path, &st) != 0) return errno == ENOENT ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
    out_info->is_directory = (st.st_mode & _S_IFDIR) != 0;
    out_info->is_regular_file = (st.st_mode & _S_IFREG) != 0;
    out_info->size_bytes = (uint64_t)st.st_size;
    out_info->modified_epoch_seconds = (int64_t)st.st_mtime;
    out_info->created_epoch_seconds = (int64_t)st.st_ctime;
    out_info->is_readable = _access(path, 4) == 0;
    out_info->is_writable = _access(path, 2) == 0;
    out_info->is_executable = 0;
#else
    if (stat(path, &st) != 0) return errno == ENOENT ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
    out_info->is_directory = S_ISDIR(st.st_mode) != 0;
    out_info->is_regular_file = S_ISREG(st.st_mode) != 0;
    out_info->size_bytes = (uint64_t)st.st_size;
    out_info->modified_epoch_seconds = (int64_t)st.st_mtime;
    out_info->created_epoch_seconds = (int64_t)st.st_ctime;
    out_info->is_readable = access(path, R_OK) == 0;
    out_info->is_writable = access(path, W_OK) == 0;
    out_info->is_executable = access(path, X_OK) == 0;
#endif
    return OSLS_OK;
}

osls_status osls_list_directory(const char *path, char *entries, size_t entries_capacity, size_t *out_written) {
    size_t used = 0;
    if (!path || !*path || !out_written || (!entries && entries_capacity)) return OSLS_INVALID_ARGUMENT;
#if defined(_WIN32)
    {
        WIN32_FIND_DATAA data;
        char pattern[OSLS_PATH_MAX];
        HANDLE find;
        size_t n = strlen(path);
        if (n + 3 >= sizeof(pattern)) return OSLS_BUFFER_TOO_SMALL;
        memcpy(pattern, path, n);
        if (n && path[n - 1] != '\\' && path[n - 1] != '/') pattern[n++] = '\\';
        memcpy(pattern + n, "*", 2);
        find = FindFirstFileA(pattern, &data);
        if (find == INVALID_HANDLE_VALUE) return OSLS_NOT_FOUND;
        do {
            size_t len = strlen(data.cFileName) + 1;
            if (used + len > entries_capacity) { FindClose(find); *out_written = used; return OSLS_BUFFER_TOO_SMALL; }
            memcpy(entries + used, data.cFileName, len); used += len;
        } while (FindNextFileA(find, &data));
        FindClose(find);
    }
#else
    {
        DIR *dir = opendir(path);
        struct dirent *entry;
        if (!dir) return errno == ENOENT ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
        while ((entry = readdir(dir)) != NULL) {
            size_t len = strlen(entry->d_name) + 1;
            if (used + len > entries_capacity) { closedir(dir); *out_written = used; return OSLS_BUFFER_TOO_SMALL; }
            memcpy(entries + used, entry->d_name, len); used += len;
        }
        closedir(dir);
    }
#endif
    *out_written = used;
    return OSLS_OK;
}

osls_status osls_get_cpu_info(osls_cpu_info *out_info) {
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
#if defined(_WIN32)
    {
        SYSTEM_INFO si;
        MEMORYSTATUSEX mem;
        int regs[4] = {0};
        char vendor[13] = {0};
        char brand[49] = {0};
        GetSystemInfo(&si);
        out_info->logical_cores = si.dwNumberOfProcessors;
        out_info->physical_cores = out_info->logical_cores;
        out_info->cache_line_size = 64; /* x86/x64 baseline; refined below if exposed */
        mem.dwLength = sizeof(mem);
        if (GlobalMemoryStatusEx(&mem)) out_info->total_memory_bytes = mem.ullTotalPhys;
        /* CPUID vendor string (leaf 0) and brand string (leaves 0x80000002..4). */
        __cpuid(regs, 0);
        memcpy(vendor + 0, &regs[1], 4);
        memcpy(vendor + 4, &regs[3], 4);
        memcpy(vendor + 8, &regs[2], 4);
        osls_copy(out_info->vendor, sizeof(out_info->vendor), vendor[0] ? vendor : "unknown");
        __cpuid(regs, (int)0x80000000);
        if ((unsigned)regs[0] >= 0x80000004u) {
            __cpuid((int *)(brand + 0), (int)0x80000002);
            __cpuid((int *)(brand + 16), (int)0x80000003);
            __cpuid((int *)(brand + 32), (int)0x80000004);
            /* trim leading spaces */
            {
                const char *b = brand;
                while (*b == ' ') ++b;
                osls_copy(out_info->model, sizeof(out_info->model), b[0] ? b : "unknown");
            }
        } else {
            osls_copy(out_info->model, sizeof(out_info->model), "unknown");
        }
    }
#elif defined(__linux__)
    {
        long n = sysconf(_SC_NPROCESSORS_ONLN);
        long line = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
        struct sysinfo info;
        struct utsname name;
        FILE *cpuinfo;
        out_info->logical_cores = n > 0 ? (uint32_t)n : 0;
        out_info->physical_cores = out_info->logical_cores;
        out_info->cache_line_size = line > 0 ? (uint32_t)line : 0;
        if (sysinfo(&info) == 0) out_info->total_memory_bytes = (uint64_t)info.totalram * (uint64_t)info.mem_unit;
        osls_copy(out_info->vendor, sizeof(out_info->vendor), "unknown");
        if (uname(&name) == 0) osls_copy(out_info->model, sizeof(out_info->model), name.machine);
        /* Prefer the real vendor_id / model name from /proc/cpuinfo when present. */
        cpuinfo = fopen("/proc/cpuinfo", "r");
        if (cpuinfo) {
            char line_buf[512];
            int have_vendor = 0, have_model = 0;
            while ((!have_vendor || !have_model) && fgets(line_buf, sizeof(line_buf), cpuinfo)) {
                char *colon = strchr(line_buf, ':');
                char *value;
                size_t len;
                if (!colon) continue;
                value = colon + 1;
                while (*value == ' ' || *value == '\t') ++value;
                len = strlen(value);
                while (len > 0 && (value[len - 1] == '\n' || value[len - 1] == '\r' || value[len - 1] == ' ')) value[--len] = '\0';
                if (!have_vendor && strncmp(line_buf, "vendor_id", 9) == 0) {
                    osls_copy(out_info->vendor, sizeof(out_info->vendor), value);
                    have_vendor = 1;
                } else if (!have_model && strncmp(line_buf, "model name", 10) == 0) {
                    osls_copy(out_info->model, sizeof(out_info->model), value);
                    have_model = 1;
                } else if (!have_vendor && strncmp(line_buf, "CPU implementer", 15) == 0) {
                    osls_copy(out_info->vendor, sizeof(out_info->vendor), value);
                    have_vendor = 1;
                } else if (!have_model && strncmp(line_buf, "Model", 5) == 0) {
                    osls_copy(out_info->model, sizeof(out_info->model), value);
                    have_model = 1;
                }
            }
            fclose(cpuinfo);
        }
    }
#elif defined(__APPLE__)
    {
        int n = 0; size_t len = sizeof(n);
        uint64_t mem = 0;
        int64_t line = 0;
        len = sizeof(n);
        if (sysctlbyname("hw.logicalcpu", &n, &len, NULL, 0) == 0 && n > 0) out_info->logical_cores = (uint32_t)n;
        n = 0; len = sizeof(n);
        if (sysctlbyname("hw.physicalcpu", &n, &len, NULL, 0) == 0 && n > 0) out_info->physical_cores = (uint32_t)n;
        len = sizeof(mem);
        if (sysctlbyname("hw.memsize", &mem, &len, NULL, 0) == 0) out_info->total_memory_bytes = mem;
        len = sizeof(line);
        if (sysctlbyname("hw.cachelinesize", &line, &len, NULL, 0) == 0 && line > 0) out_info->cache_line_size = (uint32_t)line;
        len = sizeof(out_info->model);
        if (sysctlbyname("machdep.cpu.brand_string", out_info->model, &len, NULL, 0) != 0) osls_copy(out_info->model, sizeof(out_info->model), "unknown");
        len = sizeof(out_info->vendor);
        if (sysctlbyname("machdep.cpu.vendor", out_info->vendor, &len, NULL, 0) != 0) osls_copy(out_info->vendor, sizeof(out_info->vendor), "unknown");
    }
#else
    return OSLS_UNSUPPORTED;
#endif
    return OSLS_OK;
}

osls_status osls_get_timezone_info(osls_timezone_info *out_info) {
    time_t now;
    struct tm local_tm, utc_tm;
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
    now = time(NULL);
#if defined(_WIN32)
    if (localtime_s(&local_tm, &now) != 0 || gmtime_s(&utc_tm, &now) != 0) return OSLS_SYSTEM_ERROR;
    {
        TIME_ZONE_INFORMATION zone;
        DYNAMIC_TIME_ZONE_INFORMATION dzone;
        DWORD zone_state = GetTimeZoneInformation(&zone);
        LONG bias;
        if (zone_state == TIME_ZONE_ID_INVALID) return OSLS_SYSTEM_ERROR;
        bias = zone.Bias;
        if (zone_state == TIME_ZONE_ID_DAYLIGHT) bias += zone.DaylightBias;
        else bias += zone.StandardBias;
        out_info->offset_minutes_from_utc = (int)(-bias);
        /* Real IANA-style key name where available, else the localized std name. */
        if (GetDynamicTimeZoneInformation(&dzone) != TIME_ZONE_ID_INVALID && dzone.TimeZoneKeyName[0]) {
            WideCharToMultiByte(CP_UTF8, 0, dzone.TimeZoneKeyName, -1, out_info->id, (int)sizeof(out_info->id), NULL, NULL);
        } else if (zone.StandardName[0]) {
            WideCharToMultiByte(CP_UTF8, 0, zone.StandardName, -1, out_info->id, (int)sizeof(out_info->id), NULL, NULL);
        } else {
            osls_copy(out_info->id, sizeof(out_info->id), "local");
        }
    }
#else
    if (!localtime_r(&now, &local_tm) || !gmtime_r(&now, &utc_tm)) return OSLS_SYSTEM_ERROR;
#  if defined(__APPLE__) || defined(__linux__)
    out_info->offset_minutes_from_utc = (int)(local_tm.tm_gmtoff / 60);
#  else
    return OSLS_UNSUPPORTED;
#  endif
    {
        const char *tz = getenv("TZ");
        osls_copy(out_info->id, sizeof(out_info->id), tz && *tz ? tz : "local");
    }
#endif
    {
        char abbreviation[sizeof(out_info->abbreviation)];
        if (strftime(abbreviation, sizeof(abbreviation), "%Z", &local_tm) > 0)
            osls_copy(out_info->abbreviation, sizeof(out_info->abbreviation), abbreviation);
        else
            osls_copy(out_info->abbreviation, sizeof(out_info->abbreviation), "local");
    }
    out_info->is_dst = local_tm.tm_isdst > 0;
    return OSLS_OK;
}

osls_status osls_get_network_interfaces(osls_network_interface *buffer, size_t buffer_count, size_t *out_count) {
#if defined(_WIN32)
    {
        ULONG family = AF_UNSPEC;
        ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;
        ULONG size = 15000;
        IP_ADAPTER_ADDRESSES *addresses = NULL, *it;
        DWORD rc;
        size_t count = 0;
        int attempts = 0;
        if (!out_count) return OSLS_INVALID_ARGUMENT;
        *out_count = 0;
        do {
            addresses = (IP_ADAPTER_ADDRESSES *)malloc(size);
            if (!addresses) return OSLS_SYSTEM_ERROR;
            rc = GetAdaptersAddresses(family, flags, NULL, addresses, &size);
            if (rc == ERROR_BUFFER_OVERFLOW) { free(addresses); addresses = NULL; }
        } while (rc == ERROR_BUFFER_OVERFLOW && ++attempts < 4);
        if (rc != NO_ERROR) { free(addresses); return rc == ERROR_NO_DATA ? OSLS_OK : OSLS_SYSTEM_ERROR; }
        for (it = addresses; it; it = it->Next) {
            IP_ADAPTER_UNICAST_ADDRESS *ua;
            for (ua = it->FirstUnicastAddress; ua; ua = ua->Next) {
                int fam = ua->Address.lpSockaddr->sa_family;
                if (fam == AF_INET || fam == AF_INET6) ++count;
            }
        }
        *out_count = count;
        if (count > buffer_count || (count && !buffer)) { free(addresses); return OSLS_BUFFER_TOO_SMALL; }
        memset(buffer, 0, count * sizeof(*buffer));
        count = 0;
        for (it = addresses; it; it = it->Next) {
            IP_ADAPTER_UNICAST_ADDRESS *ua;
            for (ua = it->FirstUnicastAddress; ua; ua = ua->Next) {
                int fam = ua->Address.lpSockaddr->sa_family;
                osls_network_interface *entry;
                char addr[64] = {0};
                DWORD addr_len = (DWORD)sizeof(addr);
                if (fam != AF_INET && fam != AF_INET6) continue;
                entry = &buffer[count++];
                WideCharToMultiByte(CP_UTF8, 0, it->FriendlyName, -1, entry->name, (int)sizeof(entry->name), NULL, NULL);
                entry->is_up = (it->OperStatus == IfOperStatusUp);
                entry->is_loopback = (it->IfType == IF_TYPE_SOFTWARE_LOOPBACK);
                if (it->PhysicalAddressLength && it->PhysicalAddressLength <= 8) {
                    char *p = entry->mac_address;
                    DWORD b;
                    for (b = 0; b < it->PhysicalAddressLength && (p - entry->mac_address) + 3 < (int)sizeof(entry->mac_address); ++b)
                        p += snprintf(p, sizeof(entry->mac_address) - (size_t)(p - entry->mac_address), b ? "-%02X" : "%02X", it->PhysicalAddress[b]);
                }
                if (WSAAddressToStringA(ua->Address.lpSockaddr, (DWORD)ua->Address.iSockaddrLength, NULL, addr, &addr_len) == 0) {
                    char *pct = strchr(addr, '%'); if (pct) *pct = '\0';
                    if (fam == AF_INET) osls_copy(entry->ipv4, sizeof(entry->ipv4), addr);
                    else osls_copy(entry->ipv6, sizeof(entry->ipv6), addr);
                }
            }
        }
        free(addresses);
        return OSLS_OK;
    }
#else
    struct ifaddrs *list, *it;
    size_t count = 0;
    if (!out_count) return OSLS_INVALID_ARGUMENT;
    if (getifaddrs(&list) != 0) return OSLS_SYSTEM_ERROR;
    for (it = list; it; it = it->ifa_next) if (it->ifa_name && it->ifa_addr && (it->ifa_addr->sa_family == AF_INET || it->ifa_addr->sa_family == AF_INET6)) ++count;
    *out_count = count;
    if (count > buffer_count || (count && !buffer)) { freeifaddrs(list); return OSLS_BUFFER_TOO_SMALL; }
    memset(buffer, 0, count * sizeof(*buffer));
    count = 0;
    for (it = list; it; it = it->ifa_next) {
        char address[INET6_ADDRSTRLEN];
        osls_network_interface *entry;
        if (!it->ifa_name || !it->ifa_addr || (it->ifa_addr->sa_family != AF_INET && it->ifa_addr->sa_family != AF_INET6)) continue;
        entry = &buffer[count++];
        osls_copy(entry->name, sizeof(entry->name), it->ifa_name);
        entry->is_up = (it->ifa_flags & IFF_UP) != 0;
        entry->is_loopback = (it->ifa_flags & IFF_LOOPBACK) != 0;
        if (inet_ntop(it->ifa_addr->sa_family,
                      it->ifa_addr->sa_family == AF_INET ? (void *)&((struct sockaddr_in *)it->ifa_addr)->sin_addr : (void *)&((struct sockaddr_in6 *)it->ifa_addr)->sin6_addr,
                      address, sizeof(address))) {
            if (it->ifa_addr->sa_family == AF_INET) osls_copy(entry->ipv4, sizeof(entry->ipv4), address);
            else osls_copy(entry->ipv6, sizeof(entry->ipv6), address);
        }
    }
    freeifaddrs(list);
    return OSLS_OK;
#endif
}
