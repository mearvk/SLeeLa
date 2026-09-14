/* ========================================================================
 * sleela_io.c -- portable SLeeLa file/pipe backend.
 * ======================================================================== */
#include "sleela_io.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)
#define SLEEELA_NATIVE_WINDOWS 1
#include <windows.h>
#else
#define SLEEELA_NATIVE_WINDOWS 0
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

static SLIOPlatform requested_platform(void) {
    const char* p = getenv("SLEELA_IO_PLATFORM");
    if (!p || !*p || strcmp(p, "auto") == 0) return SL_IO_AUTO;
    if (strcmp(p, "linux") == 0 || strcmp(p, "posix") == 0) return SL_IO_LINUX;
    if (strcmp(p, "windows") == 0 || strcmp(p, "win32") == 0) return SL_IO_WINDOWS;
    return SL_IO_AUTO;
}

SLIOPlatform slio_platform(void) {
#if SLEEELA_NATIVE_WINDOWS
    return SL_IO_WINDOWS;
#else
    return SL_IO_LINUX;
#endif
}

const char* slio_platform_name(void) {
    return slio_platform() == SL_IO_WINDOWS ? "windows" : "linux";
}

int slio_platform_is_available(SLIOPlatform platform) {
    return platform == SL_IO_AUTO || platform == slio_platform();
}

#if SLEEELA_NATIVE_WINDOWS
static const char* pipe_name(const char* path) {
    if (!path) return NULL;
    if (strncmp(path, "\\\\.\\pipe\\", 9) == 0) return path;
    return path;
}

static DWORD access_flags(const char* mode, DWORD* creation, DWORD* attrs) {
    if (!mode || !creation || !attrs) return 0;
    *attrs = FILE_ATTRIBUTE_NORMAL;
    if (strchr(mode, 'n')) *attrs |= FILE_FLAG_OVERLAPPED;
    if (strchr(mode, 'w')) { *creation = CREATE_ALWAYS; return GENERIC_WRITE; }
    if (strchr(mode, 'a')) { *creation = OPEN_ALWAYS; return FILE_APPEND_DATA; }
    if (strstr(mode, "rw") || strchr(mode, '+')) { *creation = OPEN_EXISTING; return GENERIC_READ | GENERIC_WRITE; }
    *creation = OPEN_EXISTING;
    return GENERIC_READ;
}

int slio_pipe(SLIOHandle* read_handle, SLIOHandle* write_handle) {
    if (!read_handle || !write_handle) return -1;
    HANDLE r = NULL, w = NULL;
    SECURITY_ATTRIBUTES sa;
    memset(&sa, 0, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = FALSE;
    if (!CreatePipe(&r, &w, &sa, 0)) return -1;
    *read_handle = (SLIOHandle)(intptr_t)r;
    *write_handle = (SLIOHandle)(intptr_t)w;
    return 0;
}

int slio_named_pipe_create(const char* path, unsigned mode) {
    (void)mode;
    const char* name = pipe_name(path);
    if (!name) return -1;
    HANDLE h = CreateNamedPipeA(name,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES, 65536, 65536, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    CloseHandle(h);
    return 0;
}

SLIOHandle slio_open(const char* path, const char* mode) {
    DWORD creation = OPEN_EXISTING, attrs = FILE_ATTRIBUTE_NORMAL;
    DWORD access = access_flags(mode, &creation, &attrs);
    if (!path || !access) return SLIO_INVALID_HANDLE;
    HANDLE h = CreateFileA(path, access, FILE_SHARE_READ | FILE_SHARE_WRITE,
                           NULL, creation, attrs, NULL);
    if (h == INVALID_HANDLE_VALUE) return SLIO_INVALID_HANDLE;
    return (SLIOHandle)(intptr_t)h;
}

int slio_read(SLIOHandle handle, char* buffer, size_t capacity) {
    if (handle == SLIO_INVALID_HANDLE || !buffer || capacity == 0) return -1;
    DWORD got = 0;
    if (!ReadFile((HANDLE)(intptr_t)handle, buffer, (DWORD)(capacity - 1), &got, NULL)) return -1;
    buffer[got] = '\0';
    return (int)got;
}

int slio_write(SLIOHandle handle, const char* data, size_t length) {
    if (handle == SLIO_INVALID_HANDLE || !data) return -1;
    DWORD written = 0;
    if (!WriteFile((HANDLE)(intptr_t)handle, data, (DWORD)length, &written, NULL)) return -1;
    return (int)written;
}

int slio_close(SLIOHandle handle) {
    if (handle == SLIO_INVALID_HANDLE) return -1;
    return CloseHandle((HANDLE)(intptr_t)handle) ? 0 : -1;
}

int slio_unlink(const char* path) {
    /* Windows named-pipe names disappear when their server instances close. */
    (void)path;
    return 0;
}

int slio_peer(SLIOHandle handle, SLIOHandle* peer) {
    (void)handle; (void)peer;
    return -1; /* peer is tracked by the SLeeLa VM for anonymous pipes */
}

int slio_create_pipe_endpoint(const char* path, unsigned mode) {
    return slio_named_pipe_create(path, mode);
}

#else

static int parse_mode(const char* mode) {
    if (!mode || !*mode) return -1;
    int flags = 0;
    int plus = strchr(mode, '+') != NULL;
    if (mode[0] == 'r') flags = plus ? O_RDWR : O_RDONLY;
    else if (mode[0] == 'w') flags = (plus ? O_RDWR : O_WRONLY) | O_CREAT | O_TRUNC;
    else if (mode[0] == 'a') flags = (plus ? O_RDWR : O_WRONLY) | O_CREAT | O_APPEND;
    else return -1;
    flags |= O_CLOEXEC;
    if (strchr(mode, 'n')) flags |= O_NONBLOCK;
    return flags;
}

int slio_pipe(SLIOHandle* read_handle, SLIOHandle* write_handle) {
    if (!read_handle || !write_handle) return -1;
    int p[2];
    if (pipe(p) != 0) return -1;
    *read_handle = (SLIOHandle)p[0];
    *write_handle = (SLIOHandle)p[1];
    return 0;
}

int slio_named_pipe_create(const char* path, unsigned mode) {
    if (!path || !*path) return -1;
    if (mkfifo(path, (mode_t)mode) == 0 || errno == EEXIST) return 0;
    return -1;
}

SLIOHandle slio_open(const char* path, const char* mode) {
    int flags = parse_mode(mode);
    if (flags < 0 || !path) return SLIO_INVALID_HANDLE;
    int fd = open(path, flags, (mode_t)0666);
    return fd < 0 ? SLIO_INVALID_HANDLE : (SLIOHandle)fd;
}

int slio_read(SLIOHandle handle, char* buffer, size_t capacity) {
    if (handle == SLIO_INVALID_HANDLE || !buffer || capacity == 0) return -1;
    ssize_t n = read((int)handle, buffer, capacity - 1);
    if (n < 0) return -1;
    buffer[n] = '\0';
    return (int)n;
}

int slio_write(SLIOHandle handle, const char* data, size_t length) {
    if (handle == SLIO_INVALID_HANDLE || !data) return -1;
    ssize_t n = write((int)handle, data, length);
    return n < 0 ? -1 : (int)n;
}

int slio_close(SLIOHandle handle) {
    if (handle == SLIO_INVALID_HANDLE) return -1;
    return close((int)handle);
}

int slio_unlink(const char* path) {
    return path ? unlink(path) : -1;
}

int slio_peer(SLIOHandle handle, SLIOHandle* peer) {
    (void)handle; (void)peer;
    return -1;
}

int slio_create_pipe_endpoint(const char* path, unsigned mode) {
    return slio_named_pipe_create(path, mode);
}

#endif

/* A mismatched explicit platform is refused rather than silently emulated. */
int slio_platform_requested_is_native(void) {
    SLIOPlatform requested = requested_platform();
    return requested == SL_IO_AUTO || requested == slio_platform();
}
