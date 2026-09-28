#include "debugger_platform.hpp"

#if defined(__linux__)
#include "debugger_backend_linux.hpp"
#elif defined(__APPLE__)
#include "debugger_backend_macos.hpp"
#elif defined(_WIN32)
#include "debugger_backend_windows.hpp"
#endif

namespace sleela::debugger {

std::unique_ptr<DebugBackend> makePlatformBackend() {
#if defined(__linux__)
    return makeLinuxPtraceBackend();
#elif defined(__APPLE__)
    return makeMacOSLLDBBackend();
#elif defined(_WIN32)
    return makeWindowsDebugBackend();
#else
    return makePortableBackend();
#endif
}

const char* platformBackendName() noexcept {
#if defined(__linux__)
    return "linux-ptrace";
#elif defined(__APPLE__)
    return "macos-lldb";
#elif defined(_WIN32)
    return "windows-debug";
#else
    return "portable";
#endif
}
}
