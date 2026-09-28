#include "debugger_backend_windows.hpp"
namespace sleela::debugger {
class WindowsDebugBackend final : public DebugBackend {
public:
    BackendKind kind() const noexcept override { return BackendKind::WindowsDebug; }
    BackendCapabilities capabilities() const noexcept override { BackendCapabilities c; return c; }
    bool launch(const BackendRequest&,std::string&e) override {e="Windows Debug API adapter requires native implementation";return false;}
    bool attach(const BackendRequest&,std::string&e) override {e="Windows Debug API adapter requires native implementation";return false;}
    bool resume(std::string&e) override {e="Windows Debug API adapter requires native implementation";return false;}
    bool step(bool,std::string&e) override {e="Windows Debug API adapter requires native implementation";return false;}
    bool setLineStop(const SourceLocation&,std::string&e) override {e="Windows Debug API adapter requires native implementation";return false;}
    bool poll(DebugEvent&,std::string&e) override {e="Windows Debug API adapter requires native implementation";return false;}
};
std::unique_ptr<DebugBackend> makeWindowsDebugBackend(){return std::make_unique<WindowsDebugBackend>();}
}
