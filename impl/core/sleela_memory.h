#ifndef SLEELA_MEMORY_H
#define SLEELA_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef enum {
    SL_MEMORY_AUTO = 0,
    SL_MEMORY_LINUX = 1,
    SL_MEMORY_WINDOWS = 2,
    SL_MEMORY_MACOS = 3
} SLMemoryPlatform;

SLMemoryPlatform slmemory_platform(void);
const char* slmemory_platform_name(void);
int slmemory_platform_is_available(SLMemoryPlatform platform);

void* slmemory_alloc(size_t size);
void* slmemory_calloc(size_t count, size_t size);
void* slmemory_realloc(void* memory, size_t size);
void slmemory_free(void* memory);

void* slmemory_aligned_alloc(size_t alignment, size_t size);
void slmemory_aligned_free(void* memory);

size_t slmemory_page_size(void);
uint64_t slmemory_monotonic_millis(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_MEMORY_H */
