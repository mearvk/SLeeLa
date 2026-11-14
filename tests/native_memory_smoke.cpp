#include "../api/native/memory/sleela_memory_manager.h"
#include <cassert>
#include <cstring>

int main() {
    /* Exercise destruction with both an empty manager and outstanding blocks.
     * This catches mutex-lifetime errors that can appear nondeterministically
     * when the manager is deleted while its lock guard is still alive. */
    sleela_mm* empty = sleela_mm_create();
    assert(empty);
    sleela_mm_destroy(empty);

    sleela_mm* mm = sleela_mm_create();
    assert(mm);
    char* p = static_cast<char*>(sleela_mm_alloc(mm, 64));
    assert(p);
    std::memset(p, 0x5a, 64);
    assert(sleela_mm_validate(mm));
    p = static_cast<char*>(sleela_mm_realloc(mm, p, 128));
    assert(p);
    assert(sleela_mm_validate(mm));
    assert(sleela_mm_free(mm, p) == SLEELA_MM_OK);
    assert(sleela_mm_validate(mm));
    sleela_mm_destroy(mm);

    /* Manager teardown owns any blocks the caller deliberately leaves live. */
    sleela_mm* live = sleela_mm_create();
    assert(live);
    assert(sleela_mm_alloc(live, 256));
    assert(sleela_mm_alloc(live, 512));
    sleela_mm_destroy(live);
    return 0;
}
