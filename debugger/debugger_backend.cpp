#include "debugger_backend.hpp"
namespace sleela::debugger {
namespace {
class PortableBackend final : public DebugBackend {
public:
    BackendKind kind() const noexcept override { return BackendKind::Portable; }
    BackendCapabilities capabilities() const noexcept override { return {}; }
    bool launch(const BackendRequest&, std::string& e) override { e="native launch backend not installed"; return false; }
    bool attach(const BackendRequest&, std::string& e) override { e="native attach backend not installed"; return false; }
    bool resume(std::string& e) override { e="native process control not installed"; return false; }
    bool step(bool, std::string& e) override { e="native stepping not installed"; return false; }
    bool setLineStop(const SourceLocation&, std::string& e) override { e="native breakpoint binding not installed"; return false; }
    bool poll(DebugEvent&, std::string& e) override { e="native event polling not installed"; return false; }
};
}
std::unique_ptr<DebugBackend> makePortableBackend() { return std::make_unique<PortableBackend>(); }
}
