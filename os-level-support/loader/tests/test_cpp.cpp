#include "../native_loader.hpp"
#include <cassert>
#include <utility>

int main(int argc, char **argv) {
    sleela::NativeModule module;
    int result = -1;
    assert(argc == 2);
    assert(module.open(argv[1]) == SL_NATIVE_OK);
    assert(module.run({"alpha", "beta", "gamma"}, result) == SL_NATIVE_OK);
    assert(result == 3);
    assert(module.is_open());

    sleela::NativeModule moved(std::move(module));
    assert(!module.is_open());
    assert(moved.is_open());

    sleela::NativeModule assigned;
    assigned = std::move(moved);
    assert(!moved.is_open());
    assert(assigned.is_open());
    assert(assigned.close() == SL_NATIVE_OK);
    assert(!assigned.is_open());
    return 0;
}
