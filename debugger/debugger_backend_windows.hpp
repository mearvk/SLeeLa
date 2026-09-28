#ifndef SLEELA_DEBUGGER_BACKEND_WINDOWS_HPP
#define SLEELA_DEBUGGER_BACKEND_WINDOWS_HPP
#include "debugger_backend.hpp"
#include <memory>
namespace sleela::debugger {
std::unique_ptr<DebugBackend> makeWindowsDebugBackend();
}
#endif
