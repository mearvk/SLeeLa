#ifndef SLEELA_DEBUGGER_LINE_HPP
#define SLEELA_DEBUGGER_LINE_HPP
#include "debugger.hpp"
#include <string>
namespace sleela::debugger {
enum class LineAction { Trace, Stop, Exception };
struct LinePoint { SourceLocation location; LineAction action=LineAction::Trace; };
class LineController {
public:
    explicit LineController(DebugSession& session) : session_(session) {}
    uint64_t registerPoint(const LinePoint& point);
    std::size_t pointCount() const noexcept { return next_id_ - 1; }
    bool hit(const LinePoint& point, const std::string& thread, const std::string& function);
private:
    DebugSession& session_;
    uint64_t next_id_=1;
};
}
#endif
