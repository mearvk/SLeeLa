#include "sleela_path.h"

/* Expose realpath()/getcwd() and related POSIX declarations. On macOS also
 * request the Darwin/BSD visibility so nothing is hidden by _XOPEN_SOURCE. */
#ifndef _WIN32
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
#define _DARWIN_C_SOURCE 1
#endif
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <sys/stat.h>   /* struct _stat64, _stat64(), _S_IFREG, _S_IFDIR */
#include <windows.h>
#define SL_PATH_NATIVE_SEPARATOR '\\'
#else
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#define SL_PATH_NATIVE_SEPARATOR '/'
#endif

static int slpath_copy(char* out, size_t out_size, const char* value) {
    size_t length;
    if (!out || out_size == 0 || !value) return -1;
    length = strlen(value);
    if (length + 1 > out_size) return -1;
    memcpy(out, value, length + 1);
    return 0;
}

SLPathPlatform slpath_platform(void) {
#if defined(_WIN32)
    return SL_PATH_WINDOWS;
#elif defined(__APPLE__)
    return SL_PATH_MACOS;
#else
    return SL_PATH_LINUX;
#endif
}

const char* slpath_platform_name(void) {
    switch (slpath_platform()) {
        case SL_PATH_WINDOWS: return "windows";
        case SL_PATH_MACOS:   return "macos";
        default:              return "linux";
    }
}

int slpath_platform_is_available(SLPathPlatform platform) {
    if (platform == SL_PATH_AUTO) return 1;
    return platform == slpath_platform();
}

char slpath_separator(void) {
    return SL_PATH_NATIVE_SEPARATOR;
}

int slpath_normalize(char* out, size_t out_size, const char* path) {
    size_t i;
    size_t j = 0;
    int previous_separator = 0;

    if (!out || out_size == 0 || !path || path[0] == '\0') return -1;

    for (i = 0; path[i] != '\0'; ++i) {
        char c = path[i];
        int separator = (c == '/' || c == '\\');
        if (separator) {
            if (previous_separator) continue;
            c = SL_PATH_NATIVE_SEPARATOR;
            previous_separator = 1;
        } else {
            previous_separator = 0;
        }
        if (j + 1 >= out_size) return -1;
        out[j++] = c;
    }

    while (j > 1 && out[j - 1] == SL_PATH_NATIVE_SEPARATOR) --j;
    out[j] = '\0';
    return 0;
}

int slpath_join(char* out, size_t out_size, const char* left, const char* right) {
    size_t left_length;
    size_t right_start = 0;
    int needs_separator;

    if (!out || out_size == 0 || !left || !right || left[0] == '\0') return -1;
    left_length = strlen(left);
    while (right[right_start] == '/' || right[right_start] == '\\') ++right_start;
    needs_separator = left_length > 0 && left[left_length - 1] != '/' && left[left_length - 1] != '\\';

    if (snprintf(out, out_size, "%s%s%s", left,
                 needs_separator ? (SL_PATH_NATIVE_SEPARATOR == '/' ? "/" : "\\") : "",
                 right + right_start) >= (int)out_size) return -1;
    return slpath_normalize(out, out_size, out);
}

int slpath_current_directory(char* out, size_t out_size) {
#ifdef _WIN32
    return _getcwd(out, (int)out_size) ? 0 : -1;
#else
    return getcwd(out, out_size) ? 0 : -1;
#endif
}

int slpath_absolute(char* out, size_t out_size, const char* path) {
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD length;
    if (!out || out_size == 0 || !path) return -1;
    length = GetFullPathNameA(path, (DWORD)sizeof(buffer), buffer, NULL);
    if (length == 0 || length >= sizeof(buffer)) return -1;
    if (length + 1 > out_size) return -1;
    return slpath_normalize(out, out_size, buffer);
#else
    char buffer[PATH_MAX];
    if (!out || out_size == 0 || !path) return -1;
    if (realpath(path, buffer)) {
        return slpath_copy(out, out_size, buffer);
    }
    /* realpath() fails when the path does not (yet) exist. Making a path
     * absolute is a lexical operation and must not require the file to exist:
     * fall back to a lexical resolution against the current directory. */
    if (path[0] == '/') {
        return slpath_normalize(out, out_size, path);
    }
    if (slpath_current_directory(buffer, sizeof(buffer)) != 0) return -1;
    if (slpath_join(out, out_size, buffer, path) != 0) return -1;
    return slpath_normalize(out, out_size, out);
#endif
}

int slpath_exists(const char* path) {
    if (!path) return 0;
#ifdef _WIN32
    return _access(path, 0) == 0;
#else
    return access(path, F_OK) == 0;
#endif
}

int slpath_is_file(const char* path) {
#ifdef _WIN32
    struct _stat64 st;
    return path && _stat64(path, &st) == 0 && (st.st_mode & _S_IFREG) != 0;
#else
    struct stat st;
    return path && stat(path, &st) == 0 && S_ISREG(st.st_mode);
#endif
}

int slpath_is_directory(const char* path) {
#ifdef _WIN32
    struct _stat64 st;
    return path && _stat64(path, &st) == 0 && (st.st_mode & _S_IFDIR) != 0;
#else
    struct stat st;
    return path && stat(path, &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

int slpath_make_directory(const char* path) {
    if (!path || path[0] == '\0') return -1;
    if (slpath_is_directory(path)) return 0;
#ifdef _WIN32
    return _mkdir(path) == 0 ? 0 : -1;
#else
    return mkdir(path, 0775) == 0 ? 0 : -1;
#endif
}

int slpath_remove(const char* path) {
    return path && remove(path) == 0 ? 0 : -1;
}

int slpath_rename(const char* old_path, const char* new_path) {
    return old_path && new_path && rename(old_path, new_path) == 0 ? 0 : -1;
}
