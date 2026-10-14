#include "../include/sleela_vm_gc.hpp"
namespace sleela::vm {
/* One collector ABI is shared by SLVM/1-11. This facade supplies RAII for
 * C++ VM generations without creating a second heap or second policy. */
static_assert(sizeof(std::uintptr_t) >= sizeof(void*), "pointer-sized uintptr_t required");
}
