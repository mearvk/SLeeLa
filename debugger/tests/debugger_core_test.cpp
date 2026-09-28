#include "../debugger.hpp"

#include <cassert>
#include <sstream>
#include <string>

int main() {
    using namespace sleela::debugger;

    DebugSession session;

    Breakpoint breakpoint;
    breakpoint.function = "main";
    const auto breakpoint_id = session.addBreakpoint(breakpoint);

    Watchpoint watchpoint;
    watchpoint.expression = "counter";
    const auto watchpoint_id = session.addWatchpoint(watchpoint);

    DebugEvent event;
    event.type = EventType::Breakpoint;
    event.message = "breakpoint hit";
    event.thread = "thread-1";
    event.fields["breakpoint"] = "main";
    session.emit(event);

    std::ostringstream report;
    session.report(report);

    const std::string text = report.str();
    assert(text.find("breakpoint hit") != std::string::npos);
    assert(text.find("breakpoint=main") != std::string::npos);
    assert(session.removeBreakpoint(breakpoint_id));
    assert(session.removeWatchpoint(watchpoint_id));
    assert(!session.removeBreakpoint(breakpoint_id));

    return 0;
}
