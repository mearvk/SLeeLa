#include "../api/native/memory/sleela_memory_manager.h"
#include <cstdio>
#include <cstring>

static void checkpoint(const char* name) {
    std::fprintf(stderr, "[memory-smoke] %s\n", name);
    std::fflush(stderr);
}

static bool require(bool condition, const char* name) {
    if (condition) return true;
    std::fprintf(stderr, "[memory-smoke] FAIL: %s\n", name);
    std::fflush(stderr);
    return false;
}

int main() {
    checkpoint("create empty manager");
    sleela_mm* empty = sleela_mm_create();
    if (!require(empty != nullptr, "empty manager creation")) return 1;

    checkpoint("destroy empty manager");
    sleela_mm_destroy(empty);
    checkpoint("empty manager destroyed");

    checkpoint("create allocation manager");
    sleela_mm* mm = sleela_mm_create();
    if (!require(mm != nullptr, "manager creation")) return 1;

    checkpoint("allocate 64-byte block");
    char* p = static_cast<char*>(sleela_mm_alloc(mm, 64));
    if (!require(p != nullptr, "64-byte allocation")) return 1;

    checkpoint("write allocation pattern");
    std::memset(p, 0x5a, 64);

    checkpoint("validate first allocation");
    if (!require(sleela_mm_validate(mm) != 0, "validation after allocation")) return 1;

    checkpoint("reallocate 64-byte block to 128 bytes");
    p = static_cast<char*>(sleela_mm_realloc(mm, p, 128));
    if (!require(p != nullptr, "reallocation to 128 bytes")) return 1;

    checkpoint("verify reallocation preserved original bytes");
    for (size_t i = 0; i < 64; ++i) {
        if (!require(static_cast<unsigned char>(p[i]) == 0x5a,
                     "reallocation data preservation")) return 1;
    }

    checkpoint("validate reallocated block");
    if (!require(sleela_mm_validate(mm) != 0, "validation after reallocation")) return 1;

    checkpoint("free reallocated block");
    if (!require(sleela_mm_free(mm, p) == SLEELA_MM_OK, "free after reallocation")) return 1;

    checkpoint("validate after free");
    if (!require(sleela_mm_validate(mm) != 0, "validation after free")) return 1;

    checkpoint("destroy allocation manager");
    sleela_mm_destroy(mm);
    checkpoint("allocation manager destroyed");

    checkpoint("create outstanding-block manager");
    sleela_mm* live = sleela_mm_create();
    if (!require(live != nullptr, "outstanding-block manager creation")) return 1;

    checkpoint("allocate outstanding 256-byte block");
    void* first = sleela_mm_alloc(live, 256);
    if (!require(first != nullptr, "first outstanding allocation")) return 1;

    checkpoint("allocate outstanding 512-byte block");
    void* second = sleela_mm_alloc(live, 512);
    if (!require(second != nullptr, "second outstanding allocation")) return 1;

    checkpoint("destroy manager with outstanding blocks");
    sleela_mm_destroy(live);
    checkpoint("outstanding-block manager destroyed");

    checkpoint("PASS");
    return 0;
}
