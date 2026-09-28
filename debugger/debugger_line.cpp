#include "debugger_line.hpp"
namespace sleela::debugger {
uint64_t LineController::registerPoint(const LinePoint& point) {
    return next_id_++;
}
bool LineController::hit(const LinePoint& point, const std::string& thread, const std::string& function) {
    DebugEvent event{};
    event.type = point.action==LineAction::Exception ? EventType::Exception :
                 point.action==LineAction::Stop ? EventType::Breakpoint : EventType::Info;
    event.severity = point.action==LineAction::Exception ? "exception" :
                     point.action==LineAction::Stop ? "stop" : "trace";
    event.message = function.empty() ? "source-line" : function;
    event.thread = thread;
    event.location = point.location;
    session_.emit(std::move(event));
    return point.action != LineAction::Trace;
}
}
