#ifndef SLEELA_DEBUGGER_PLATFORM_HPP
#define SLEELA_DEBUGGER_PLATFORM_HPP

#include "debugger_backend.hpp"
#include <memory>

namespace sleela::debugger {
std::unique_ptr<DebugBackend> makePlatformBackend();
const char* platformBackendName() noexcept;
}
#endif
