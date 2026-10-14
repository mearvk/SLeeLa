#include "sleela_memory.h"

#include <stdio.h>
#include <stdint.h>

int main(void) {
    const size_t alignment = 64;
    const size_t size = 256;
    void* memory = slmemory_alloc(size);
    void* zeroed = slmemory_calloc(4, 64);
    void* aligned = slmemory_aligned_alloc(alignment, size);

    if (memory == NULL || zeroed == NULL || aligned == NULL) {
        slmemory_free(memory);
        slmemory_free(zeroed);
        slmemory_aligned_free(aligned);
        fprintf(stderr, "memory platform smoke: allocation failed\n");
        return 1;
    }

    if (((uintptr_t)aligned % alignment) != 0) {
        slmemory_free(memory);
        slmemory_free(zeroed);
        slmemory_aligned_free(aligned);
        fprintf(stderr, "memory platform smoke: alignment failed\n");
        return 1;
    }

    if (slmemory_page_size() == 0) {
        slmemory_free(memory);
        slmemory_free(zeroed);
        slmemory_aligned_free(aligned);
        fprintf(stderr, "memory platform smoke: page size unavailable\n");
        return 1;
    }

    uint64_t before = slmemory_monotonic_millis();
    void* resized = slmemory_realloc(memory, 512);
    uint64_t after = slmemory_monotonic_millis();
    if (resized == NULL || after < before) {
        slmemory_free(resized);
        slmemory_free(zeroed);
        slmemory_aligned_free(aligned);
        fprintf(stderr, "memory platform smoke: realloc or monotonic clock failed\n");
        return 1;
    }

    slmemory_free(resized);
    slmemory_free(zeroed);
    slmemory_aligned_free(aligned);

    printf("memory/system platform: %s\n", slmemory_platform_name());
    printf("page size: %zu\n", slmemory_page_size());
    printf("memory/system platform smoke: PASS\n");
    return 0;
}
