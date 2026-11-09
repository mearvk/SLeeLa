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

    SLNativeValue a{};
    SLNativeValue b{};
    SLNativeValue sum{};
    a.tag = SL_NATIVE_VALUE_I64;
    a.as.i64 = 12;
    b.tag = SL_NATIVE_VALUE_I64;
    b.as.i64 = 30;
    assert(module.call("sum", {a, b}, sum) == SL_NATIVE_OK);
    assert(sum.tag == SL_NATIVE_VALUE_I64 && sum.as.i64 == 42);
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
