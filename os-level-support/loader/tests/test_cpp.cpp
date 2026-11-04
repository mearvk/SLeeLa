#include "../native_loader.hpp"
#include <cassert>

int main(int argc, char **argv) {
    sleela::NativeModule module;
    int result = -1;
    assert(argc == 2);
    assert(module.open(argv[1]) == SL_NATIVE_OK);
    assert(module.run({"alpha", "beta", "gamma"}, result) == SL_NATIVE_OK);
    assert(result == 3);
    assert(module.is_open());
    assert(module.close() == SL_NATIVE_OK);
    assert(!module.is_open());
    return 0;
}
