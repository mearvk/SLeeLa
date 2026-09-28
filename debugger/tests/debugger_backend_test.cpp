#include "../debugger_backend.hpp"

#include <cassert>
#include <memory>
#include <string>

namespace sleela::debugger {
std::unique_ptr<DebugBackend> makePortableBackend();
}

int main() {
    using namespace sleela::debugger;

    auto backend = makePortableBackend();
    assert(backend != nullptr);
    assert(backend->kind() == BackendKind::Portable);

    const auto caps = backend->capabilities();
    assert(!caps.launch);
    assert(!caps.attach);
    assert(!caps.continue_execution);
    assert(!caps.step);
    assert(!caps.breakpoints);
    assert(!caps.watchpoints);

    std::string error;
    BackendRequest request;
    assert(!backend->launch(request, error));
    assert(!error.empty());

    return 0;
}
