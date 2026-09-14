#include "sleela_memory.h"

#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

SLMemoryPlatform slmemory_platform(void) {
#ifdef _WIN32
    return SL_MEMORY_WINDOWS;
#else
    return SL_MEMORY_LINUX;
#endif
}

const char* slmemory_platform_name(void) {
#ifdef _WIN32
    return "windows-system";
#else
    return "linux-posix";
#endif
}

int slmemory_platform_is_available(SLMemoryPlatform platform) {
    if (platform == SL_MEMORY_AUTO) {
        return 1;
    }
#ifdef _WIN32
    return platform == SL_MEMORY_WINDOWS;
#else
    return platform == SL_MEMORY_LINUX;
#endif
}

void* slmemory_alloc(size_t size) {
    return malloc(size);
}

void* slmemory_calloc(size_t count, size_t size) {
    return calloc(count, size);
}

void* slmemory_realloc(void* memory, size_t size) {
    return realloc(memory, size);
}

void slmemory_free(void* memory) {
    free(memory);
}

void* slmemory_aligned_alloc(size_t alignment, size_t size) {
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        return NULL;
    }
#ifdef _WIN32
    return _aligned_malloc(size, alignment);
#else
    void* memory = NULL;
    if (posix_memalign(&memory, alignment, size) != 0) {
        return NULL;
    }
    return memory;
#endif
}

void slmemory_aligned_free(void* memory) {
#ifdef _WIN32
    _aligned_free(memory);
#else
    free(memory);
#endif
}

size_t slmemory_page_size(void) {
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (size_t)info.dwPageSize;
#else
    long size = sysconf(_SC_PAGESIZE);
    return size > 0 ? (size_t)size : 4096u;
#endif
}

uint64_t slmemory_monotonic_millis(void) {
#ifdef _WIN32
    return (uint64_t)GetTickCount64();
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
#endif
}
