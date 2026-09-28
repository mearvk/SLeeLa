#ifndef SLEELA_DEBUGGER_BACKEND_LINUX_HPP
#define SLEELA_DEBUGGER_BACKEND_LINUX_HPP
#include "debugger_backend.hpp"
#include <memory>
namespace sleela::debugger {
std::unique_ptr<DebugBackend> makeLinuxPtraceBackend();
}
#endif
