#include "debugger_backend_macos.hpp"
namespace sleela::debugger {
class MacOSLLDBBackend final : public DebugBackend {
public:
    BackendKind kind() const noexcept override { return BackendKind::MacOSLLDB; }
    BackendCapabilities capabilities() const noexcept override { BackendCapabilities c; c.source_mapping=true; return c; }
    bool launch(const BackendRequest&,std::string&e) override {e="LLDB adapter requires native LLDB integration";return false;}
    bool attach(const BackendRequest&,std::string&e) override {e="LLDB adapter requires native LLDB integration";return false;}
    bool resume(std::string&e) override {e="LLDB adapter requires native LLDB integration";return false;}
    bool step(bool,std::string&e) override {e="LLDB adapter requires native LLDB integration";return false;}
    bool setLineStop(const SourceLocation&,std::string&e) override {e="LLDB adapter requires native LLDB integration";return false;}
    bool poll(DebugEvent&,std::string&e) override {e="LLDB adapter requires native LLDB integration";return false;}
};
std::unique_ptr<DebugBackend> makeMacOSLLDBBackend(){return std::make_unique<MacOSLLDBBackend>();}
}
