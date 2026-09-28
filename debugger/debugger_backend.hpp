#ifndef SLEELA_DEBUGGER_BACKEND_HPP
#define SLEELA_DEBUGGER_BACKEND_HPP
#include "debugger.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger {
enum class BackendKind { Portable, LinuxPtrace, MacOSLLDB, WindowsDebug };
struct BackendCapabilities {
    bool launch=false, attach=false, continue_execution=false, step=false;
    bool breakpoints=false, watchpoints=false, threads=false, stack=false;
    bool registers=false, memory=false, exceptions=false, source_mapping=false;
};
struct BackendRequest {
    std::string executable;
    std::vector<std::string> arguments;
    std::string process_id;
};
class DebugBackend {
public:
    virtual ~DebugBackend() = default;
    virtual BackendKind kind() const noexcept = 0;
    virtual BackendCapabilities capabilities() const noexcept = 0;
    virtual bool launch(const BackendRequest&, std::string& error) = 0;
    virtual bool attach(const BackendRequest&, std::string& error) = 0;
    virtual bool resume(std::string& error) = 0;
    virtual bool step(bool over, std::string& error) = 0;
    virtual bool setLineStop(const SourceLocation&, std::string& error) = 0;
    virtual bool poll(DebugEvent& event, std::string& error) = 0;
};
}
#endif
