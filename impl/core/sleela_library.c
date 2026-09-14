#include "sleela_library.h"

#include <errno.h>
#include <stddef.h>

#ifdef _WIN32
#include <windows.h>

SLLibraryPlatform sllibrary_platform(void) { return SL_LIBRARY_WINDOWS; }
const char* sllibrary_platform_name(void) { return "windows-loadlibrary"; }
int sllibrary_platform_is_available(SLLibraryPlatform platform) {
    return platform == SL_LIBRARY_AUTO || platform == SL_LIBRARY_WINDOWS;
}

int sllibrary_open(SLLibraryHandle* library, const char* path) {
    HMODULE module;
    if (!library || !path || !*path) return EINVAL;
    module = LoadLibraryA(path);
    if (!module) return (int)GetLastError();
    *library = (SLLibraryHandle)(intptr_t)module;
    return 0;
}

SLLibrarySymbol sllibrary_symbol(SLLibraryHandle library, const char* name) {
    HMODULE module = (HMODULE)(intptr_t)library;
    if (!module || !name || !*name) return NULL;
    return (SLLibrarySymbol)GetProcAddress(module, name);
}

int sllibrary_close(SLLibraryHandle library) {
    HMODULE module = (HMODULE)(intptr_t)library;
    if (!module) return EINVAL;
    return FreeLibrary(module) ? 0 : (int)GetLastError();
}

int sllibrary_valid(SLLibraryHandle library) {
    return library != (SLLibraryHandle)0;
}

const char* sllibrary_last_error(void) {
    static char buffer[128];
    DWORD code = GetLastError();
    if (!code) return "no Windows loader error";
    (void)snprintf(buffer, sizeof(buffer), "Windows loader error %lu", (unsigned long)code);
    return buffer;
}

#else

#include <dlfcn.h>

SLLibraryPlatform sllibrary_platform(void) { return SL_LIBRARY_LINUX; }
const char* sllibrary_platform_name(void) { return "linux-dlopen"; }
int sllibrary_platform_is_available(SLLibraryPlatform platform) {
    return platform == SL_LIBRARY_AUTO || platform == SL_LIBRARY_LINUX;
}

int sllibrary_open(SLLibraryHandle* library, const char* path) {
    void* module;
    if (!library || !path || !*path) return EINVAL;
    dlerror();
    module = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!module) return ENOENT;
    *library = (SLLibraryHandle)(intptr_t)module;
    return 0;
}

SLLibrarySymbol sllibrary_symbol(SLLibraryHandle library, const char* name) {
    void* module = (void*)(intptr_t)library;
    if (!module || !name || !*name) return NULL;
    dlerror();
    return (SLLibrarySymbol)dlsym(module, name);
}

int sllibrary_close(SLLibraryHandle library) {
    void* module = (void*)(intptr_t)library;
    if (!module) return EINVAL;
    return dlclose(module) == 0 ? 0 : EINVAL;
}

int sllibrary_valid(SLLibraryHandle library) {
    return library != (SLLibraryHandle)0;
}

const char* sllibrary_last_error(void) {
    const char* error = dlerror();
    return error ? error : "no loader error";
}

#endif
