#ifndef SLEELA_DEBUGGER_BACKEND_HPP
#define SLEELA_DEBUGGER_BACKEND_HPP
#include "debugger.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace sleela::debugger {
enum class BackendKind { Portable, LinuxPtrace, MacOSLLDB, WindowsDebug };
struct BackendCapabilities {
    bool launch=false, attach=false, continue_execution=false, step=false;
    bool breakpoints=false, watchpoints=false, threads=false, stack=false;
    bool registers=false, memory=false, exceptions=false, source_mapping=false;
};
// Named distinctly from debug_engine.hpp's RegisterSnapshot (which carries a
// vector<RegisterValue>); this backend view holds the raw pointer registers.
struct BackendRegisterSnapshot { std::string architecture; std::uint64_t instruction_pointer{0}; std::uint64_t stack_pointer{0}; std::uint64_t frame_pointer{0}; };
struct BackendRequest { std::string executable; std::vector<std::string> arguments; std::string process_id; };
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
    virtual bool readMemory(std::uint64_t address, void* buffer, std::size_t size, std::string& error) {
        (void)address; (void)buffer; (void)size; error="memory read not implemented by backend"; return false;
    }
    virtual bool writeMemory(std::uint64_t address, const void* buffer, std::size_t size, std::string& error) {
        (void)address; (void)buffer; (void)size; error="memory write not implemented by backend"; return false;
    }
    virtual bool readRegisters(BackendRegisterSnapshot&, std::string& error) {
        error="register access not implemented by backend"; return false;
    }
};
std::unique_ptr<DebugBackend> makePortableBackend();
}
#endif
