#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
/* Expose BSD interfaces used by the macOS headers even with _POSIX_C_SOURCE. */
#  define _DARWIN_C_SOURCE 1
#endif

#include "osls.h"
#include "osls_vm_bridge.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
#  include <shlwapi.h>
#  pragma comment(lib, "shlwapi.lib")
#  pragma comment(lib, "ws2_32.lib")
#else
#  include <unistd.h>
#  include <limits.h>
#  include <stdlib.h>
#  include <sys/types.h>
#  include <netdb.h>
#  include <arpa/inet.h>
#  include <sys/socket.h>
#  if defined(__linux__)
#    include <sys/statvfs.h>
#    include <sys/sysinfo.h>
#  elif defined(__APPLE__)
#    include <sys/statvfs.h>
#    include <sys/mount.h>
#    include <sys/sysctl.h>
#    include <mach/mach.h>
#    include <mach/mach_host.h>
#  endif
#endif

static int osls2_copy(char *dst, size_t cap, const char *src) {
    size_t n;
    if (!dst || !cap || !src) return 0;
    n = strlen(src);
    if (n >= cap) return 0;
    memcpy(dst, src, n + 1);
    return 1;
}

/* -------------------------------------------------------------------------
 * Memory
 * ---------------------------------------------------------------------- */
osls_status osls_get_memory_info(osls_memory_info *out_info) {
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
#if defined(_WIN32)
    {
        MEMORYSTATUSEX mem;
        SYSTEM_INFO si;
        mem.dwLength = sizeof(mem);
        if (!GlobalMemoryStatusEx(&mem)) return OSLS_SYSTEM_ERROR;
        GetSystemInfo(&si);
        out_info->total_bytes = mem.ullTotalPhys;
        out_info->available_bytes = mem.ullAvailPhys;
        out_info->used_bytes = mem.ullTotalPhys - mem.ullAvailPhys;
        out_info->page_size_bytes = si.dwPageSize;
    }
    return OSLS_OK;
#elif defined(__linux__)
    {
        struct sysinfo info;
        long page = sysconf(_SC_PAGESIZE);
        long avail_pages = sysconf(_SC_AVPHYS_PAGES);
        if (sysinfo(&info) != 0) return OSLS_SYSTEM_ERROR;
        out_info->total_bytes = (uint64_t)info.totalram * (uint64_t)info.mem_unit;
        if (avail_pages > 0 && page > 0) out_info->available_bytes = (uint64_t)avail_pages * (uint64_t)page;
        else out_info->available_bytes = (uint64_t)info.freeram * (uint64_t)info.mem_unit;
        out_info->used_bytes = out_info->total_bytes > out_info->available_bytes
            ? out_info->total_bytes - out_info->available_bytes : 0;
        out_info->page_size_bytes = page > 0 ? (uint64_t)page : 0;
    }
    return OSLS_OK;
#elif defined(__APPLE__)
    {
        uint64_t mem = 0; size_t len = sizeof(mem);
        vm_size_t page = 0;
        mach_port_t host = mach_host_self();
        vm_statistics64_data_t vmstat;
        mach_msg_type_number_t cnt = HOST_VM_INFO64_COUNT;
        if (sysctlbyname("hw.memsize", &mem, &len, NULL, 0) == 0) out_info->total_bytes = mem;
        if (host_page_size(host, &page) == KERN_SUCCESS) out_info->page_size_bytes = (uint64_t)page;
        if (host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&vmstat, &cnt) == KERN_SUCCESS && page) {
            out_info->available_bytes = (uint64_t)(vmstat.free_count + vmstat.inactive_count) * (uint64_t)page;
            out_info->used_bytes = out_info->total_bytes > out_info->available_bytes
                ? out_info->total_bytes - out_info->available_bytes : 0;
        }
    }
    return OSLS_OK;
#else
    return OSLS_UNSUPPORTED;
#endif
}

osls_status osls_get_available_memory(uint64_t *out_bytes) {
    osls_memory_info info;
    osls_status s;
    if (!out_bytes) return OSLS_INVALID_ARGUMENT;
    *out_bytes = 0;
    s = osls_get_memory_info(&info);
    if (s != OSLS_OK) return s;
    *out_bytes = info.available_bytes;
    return OSLS_OK;
}

/* -------------------------------------------------------------------------
 * Disk space and volume
 * ---------------------------------------------------------------------- */
osls_status osls_get_disk_space(const char *path, osls_disk_space *out_info) {
    if (!path || !*path || !out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
    if (!osls2_copy(out_info->path, sizeof(out_info->path), path)) return OSLS_BUFFER_TOO_SMALL;
#if defined(_WIN32)
    {
        ULARGE_INTEGER avail, total, free_bytes;
        wchar_t wpath[OSLS_PATH_MAX];
        if (MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, OSLS_PATH_MAX) == 0) return OSLS_INVALID_ARGUMENT;
        if (!GetDiskFreeSpaceExW(wpath, &avail, &total, &free_bytes))
            return GetLastError() == ERROR_PATH_NOT_FOUND ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
        out_info->total_bytes = total.QuadPart;
        out_info->free_bytes = free_bytes.QuadPart;
        out_info->available_bytes = avail.QuadPart;
    }
    return OSLS_OK;
#else
    {
        struct statvfs st;
        if (statvfs(path, &st) != 0) return errno == ENOENT ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
        out_info->total_bytes = (uint64_t)st.f_blocks * (uint64_t)st.f_frsize;
        out_info->free_bytes = (uint64_t)st.f_bfree * (uint64_t)st.f_frsize;
        out_info->available_bytes = (uint64_t)st.f_bavail * (uint64_t)st.f_frsize;
    }
    return OSLS_OK;
#endif
}

osls_status osls_get_volume_info(const char *path, osls_volume_info *out_info) {
    if (!path || !*path || !out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
    if (!osls2_copy(out_info->path, sizeof(out_info->path), path)) return OSLS_BUFFER_TOO_SMALL;
#if defined(_WIN32)
    {
        ULARGE_INTEGER avail, total, free_bytes;
        wchar_t wpath[OSLS_PATH_MAX];
        wchar_t fsname[64] = {0};
        char fsname8[64] = {0};
        DWORD serial = 0, maxlen = 0, flags = 0;
        if (MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, OSLS_PATH_MAX) == 0) return OSLS_INVALID_ARGUMENT;
        if (GetDiskFreeSpaceExW(wpath, &avail, &total, &free_bytes)) {
            out_info->total_bytes = total.QuadPart;
            out_info->available_bytes = avail.QuadPart;
        }
        if (GetVolumeInformationW(wpath, NULL, 0, &serial, &maxlen, &flags, fsname, 64)) {
            WideCharToMultiByte(CP_UTF8, 0, fsname, -1, fsname8, (int)sizeof(fsname8), NULL, NULL);
            osls2_copy(out_info->filesystem, sizeof(out_info->filesystem), fsname8);
            out_info->is_read_only = (flags & FILE_READ_ONLY_VOLUME) != 0;
        }
    }
    return OSLS_OK;
#elif defined(__APPLE__)
    {
        struct statfs st;
        if (statfs(path, &st) != 0) return errno == ENOENT ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
        out_info->total_bytes = (uint64_t)st.f_blocks * (uint64_t)st.f_bsize;
        out_info->available_bytes = (uint64_t)st.f_bavail * (uint64_t)st.f_bsize;
        out_info->block_size = (uint64_t)st.f_bsize;
        out_info->is_read_only = (st.f_flags & MNT_RDONLY) != 0;
        osls2_copy(out_info->filesystem, sizeof(out_info->filesystem), st.f_fstypename);
    }
    return OSLS_OK;
#else
    {
        struct statvfs st;
        if (statvfs(path, &st) != 0) return errno == ENOENT ? OSLS_NOT_FOUND : OSLS_SYSTEM_ERROR;
        out_info->total_bytes = (uint64_t)st.f_blocks * (uint64_t)st.f_frsize;
        out_info->available_bytes = (uint64_t)st.f_bavail * (uint64_t)st.f_frsize;
        out_info->block_size = (uint64_t)st.f_frsize;
        out_info->is_read_only = (st.f_flag & ST_RDONLY) != 0;
        /* statvfs does not name the filesystem type portably. */
        osls2_copy(out_info->filesystem, sizeof(out_info->filesystem), "unknown");
    }
    return OSLS_OK;
#endif
}

/* -------------------------------------------------------------------------
 * System load
 * ---------------------------------------------------------------------- */
osls_status osls_get_system_load(osls_system_load *out_info) {
    long cores;
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
    out_info->load_average_1min_milli = -1;
    out_info->load_average_5min_milli = -1;
    out_info->load_average_15min_milli = -1;
#if defined(_WIN32)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        out_info->logical_cores = si.dwNumberOfProcessors;
        /* Windows has no POSIX load-average equivalent; leave windows as -1. */
    }
    return OSLS_OK;
#else
    cores = sysconf(_SC_NPROCESSORS_ONLN);
    out_info->logical_cores = cores > 0 ? (uint32_t)cores : 0;
    {
        double avg[3];
        int n = getloadavg(avg, 3);
        if (n >= 1) out_info->load_average_1min_milli = (int64_t)(avg[0] * 1000.0 + 0.5);
        if (n >= 2) out_info->load_average_5min_milli = (int64_t)(avg[1] * 1000.0 + 0.5);
        if (n >= 3) out_info->load_average_15min_milli = (int64_t)(avg[2] * 1000.0 + 0.5);
    }
    return OSLS_OK;
#endif
}

/* -------------------------------------------------------------------------
 * Capability, runtime-info and native-error diagnostics
 * ---------------------------------------------------------------------- */
osls_status osls_get_capabilities(uint64_t *out_capabilities) {
    if (!out_capabilities) return OSLS_INVALID_ARGUMENT;
    /* This user-mode layer satisfies host-read inspection. */
    *out_capabilities = OSLS_VM_CAP_HOST_READ;
    return OSLS_OK;
}

osls_status osls_get_runtime_info(osls_runtime_info *out_info) {
    size_t count = 0;
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
#if defined(_WIN32)
    osls2_copy(out_info->backend, sizeof(out_info->backend), "windows");
#elif defined(__APPLE__)
    osls2_copy(out_info->backend, sizeof(out_info->backend), "macos");
#elif defined(__linux__)
    osls2_copy(out_info->backend, sizeof(out_info->backend), "linux");
#else
    osls2_copy(out_info->backend, sizeof(out_info->backend), "unknown");
#endif
    osls2_copy(out_info->abi_version, sizeof(out_info->abi_version), "1.1.0");
    (void)osls_vm_binding_table(&count);
    out_info->operation_count = (uint32_t)count;
    out_info->capabilities = OSLS_VM_CAP_HOST_READ;
    return OSLS_OK;
}

osls_status osls_get_native_error(osls_native_error *out_error) {
    if (!out_error) return OSLS_INVALID_ARGUMENT;
    memset(out_error, 0, sizeof(*out_error));
#if defined(_WIN32)
    {
        DWORD code = GetLastError();
        out_error->code = (int32_t)code;
        if (code) {
            wchar_t wbuf[256] = {0};
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                           code, 0, wbuf, 256, NULL);
            WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, out_error->message, (int)sizeof(out_error->message), NULL, NULL);
        }
    }
#else
    {
        int code = errno;
        out_error->code = (int32_t)code;
        if (code) osls2_copy(out_error->message, sizeof(out_error->message), strerror(code));
    }
#endif
    return OSLS_OK;
}

/* -------------------------------------------------------------------------
 * Path handling
 * ---------------------------------------------------------------------- */
#if defined(_WIN32)
static const char OSLS_SEP = '\\';
#else
static const char OSLS_SEP = '/';
#endif

static int osls2_is_sep(char c) { return c == '/' || c == '\\'; }

#define OSLS_MAX_SEGMENTS 256
osls_status osls_path_normalize(const char *path, char *out_buffer, size_t out_capacity) {
    const char *seg_ptr[OSLS_MAX_SEGMENTS];
    size_t seg_len[OSLS_MAX_SEGMENTS];
    size_t seg_count = 0;
    const char *p;
    int absolute;
    size_t used, i;
    if (!path || !out_buffer || out_capacity == 0) return OSLS_INVALID_ARGUMENT;
    absolute = path[0] && osls2_is_sep(path[0]);
    p = path;
    /* Tokenize into segments, resolving '.' and '..' lexically (no FS access). */
    while (*p) {
        const char *start;
        size_t len;
        while (*p && osls2_is_sep(*p)) ++p;
        start = p;
        while (*p && !osls2_is_sep(*p)) ++p;
        len = (size_t)(p - start);
        if (len == 0) continue;
        if (len == 1 && start[0] == '.') continue;
        if (len == 2 && start[0] == '.' && start[1] == '.') {
            if (seg_count > 0 && !(seg_len[seg_count - 1] == 2 &&
                                   seg_ptr[seg_count - 1][0] == '.' &&
                                   seg_ptr[seg_count - 1][1] == '.')) {
                --seg_count;            /* pop a real parent segment */
                continue;
            }
            if (absolute) continue;      /* '..' above root is dropped */
            /* else: keep leading '..' on a relative path (falls through) */
        }
        if (seg_count >= OSLS_MAX_SEGMENTS) return OSLS_BUFFER_TOO_SMALL;
        seg_ptr[seg_count] = start;
        seg_len[seg_count] = len;
        ++seg_count;
    }
    /* Reassemble. */
    used = 0;
    if (absolute) { if (used + 1 >= out_capacity) return OSLS_BUFFER_TOO_SMALL; out_buffer[used++] = OSLS_SEP; }
    for (i = 0; i < seg_count; ++i) {
        if (i > 0) { if (used + 1 >= out_capacity) return OSLS_BUFFER_TOO_SMALL; out_buffer[used++] = OSLS_SEP; }
        if (used + seg_len[i] >= out_capacity) return OSLS_BUFFER_TOO_SMALL;
        memcpy(out_buffer + used, seg_ptr[i], seg_len[i]);
        used += seg_len[i];
    }
    if (used == 0) { if (out_capacity < 2) return OSLS_BUFFER_TOO_SMALL; out_buffer[used++] = '.'; }
    out_buffer[used] = '\0';
    return OSLS_OK;
}

osls_status osls_path_join(const char *left, const char *right, char *out_buffer, size_t out_capacity) {
    char combined[OSLS_PATH_MAX];
    size_t ln, rn, need;
    if (!left || !right || !out_buffer || out_capacity == 0) return OSLS_INVALID_ARGUMENT;
    /* An absolute right component replaces left. */
    if (right[0] && osls2_is_sep(right[0])) return osls_path_normalize(right, out_buffer, out_capacity);
    ln = strlen(left);
    rn = strlen(right);
    if (ln == 0) return osls_path_normalize(right, out_buffer, out_capacity);
    need = ln + 1 + rn;
    if (need >= sizeof(combined)) return OSLS_BUFFER_TOO_SMALL;
    memcpy(combined, left, ln);
    if (!osls2_is_sep(left[ln - 1])) combined[ln++] = OSLS_SEP;
    memcpy(combined + ln, right, rn);
    combined[ln + rn] = '\0';
    return osls_path_normalize(combined, out_buffer, out_capacity);
}

osls_status osls_path_resolve(const char *path, char *out_buffer, size_t out_capacity) {
    if (!path || !*path || !out_buffer || out_capacity == 0) return OSLS_INVALID_ARGUMENT;
#if defined(_WIN32)
    {
        wchar_t wpath[OSLS_PATH_MAX], wfull[OSLS_PATH_MAX];
        if (MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, OSLS_PATH_MAX) == 0) return OSLS_INVALID_ARGUMENT;
        if (GetFullPathNameW(wpath, OSLS_PATH_MAX, wfull, NULL) == 0) return OSLS_SYSTEM_ERROR;
        if (WideCharToMultiByte(CP_UTF8, 0, wfull, -1, out_buffer, (int)out_capacity, NULL, NULL) == 0)
            return OSLS_BUFFER_TOO_SMALL;
    }
    return OSLS_OK;
#else
    {
        char resolved[PATH_MAX];
        if (!realpath(path, resolved)) {
            if (errno == ENOENT) return OSLS_NOT_FOUND;
            return OSLS_SYSTEM_ERROR;
        }
        if (strlen(resolved) >= out_capacity) return OSLS_BUFFER_TOO_SMALL;
        strcpy(out_buffer, resolved);
    }
    return OSLS_OK;
#endif
}

osls_status osls_get_path_info(const char *path, osls_path_info *out_info) {
    osls_status s;
    osls_file_info fi;
    if (!path || !*path || !out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
    if (!osls2_copy(out_info->input, sizeof(out_info->input), path)) return OSLS_BUFFER_TOO_SMALL;
    s = osls_path_normalize(path, out_info->normalized, sizeof(out_info->normalized));
    if (s != OSLS_OK) return s;
#if defined(_WIN32)
    out_info->is_absolute = (path[1] == ':' ) || osls2_is_sep(path[0]);
    if (path[1] == ':') { out_info->root[0] = path[0]; out_info->root[1] = ':'; out_info->root[2] = '\\'; out_info->root[3] = '\0'; }
    else if (osls2_is_sep(path[0])) osls2_copy(out_info->root, sizeof(out_info->root), "\\");
#else
    out_info->is_absolute = osls2_is_sep(path[0]);
    if (out_info->is_absolute) osls2_copy(out_info->root, sizeof(out_info->root), "/");
#endif
    out_info->exists = (osls_get_file_info(path, &fi) == OSLS_OK) ? 1 : 0;
    return OSLS_OK;
}

/* -------------------------------------------------------------------------
 * Hostname/service resolution (read-only DNS query; no connection is made)
 * ---------------------------------------------------------------------- */
osls_status osls_network_resolve(const char *host, char *out_buffer, size_t out_capacity) {
    struct addrinfo hints, *res = NULL, *it;
    char addr[64];
    size_t used = 0;
    int rc;
    if (!host || !*host || !out_buffer || out_capacity == 0) return OSLS_INVALID_ARGUMENT;
    out_buffer[0] = '\0';
#if defined(_WIN32)
    { WSADATA wsa; static int started = 0; if (!started) { if (WSAStartup(MAKEWORD(2,2), &wsa) == 0) started = 1; } }
#endif
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    rc = getaddrinfo(host, NULL, &hints, &res);
    if (rc != 0) return OSLS_NOT_FOUND;
    for (it = res; it; it = it->ai_next) {
        void *src;
        size_t n;
        if (it->ai_family == AF_INET) src = &((struct sockaddr_in *)it->ai_addr)->sin_addr;
        else if (it->ai_family == AF_INET6) src = &((struct sockaddr_in6 *)it->ai_addr)->sin6_addr;
        else continue;
        if (!inet_ntop(it->ai_family, src, addr, sizeof(addr))) continue;
        n = strlen(addr);
        if (used + n + (used ? 1 : 0) >= out_capacity) break;
        if (used) out_buffer[used++] = ',';
        memcpy(out_buffer + used, addr, n);
        used += n;
        out_buffer[used] = '\0';
    }
    freeaddrinfo(res);
    if (used == 0) return OSLS_NOT_FOUND;
    return OSLS_OK;
}
