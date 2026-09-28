#include "../debugger.hpp"
#include "../debugger_line.hpp"

#include <cassert>
#include <string>

int main() {
    using namespace sleela::debugger;

    DebugSession session;
    LineController controller(session);

    LinePoint trace{{"fixture.cpp", 10u, 1u}, LineAction::Trace};
    LinePoint stop{{"fixture.cpp", 11u, 1u}, LineAction::Stop};
    LinePoint exception{{"fixture.cpp", 12u, 1u}, LineAction::Exception};

    assert(controller.registerPoint(trace) == 1u);
    assert(controller.registerPoint(stop) == 2u);
    assert(controller.registerPoint(exception) == 3u);

    assert(!controller.hit(trace, "thread-1", "fixture"));
    assert(controller.hit(stop, "thread-1", "fixture"));
    assert(controller.hit(exception, "thread-1", "fixture"));

    const auto& events = session.events();
    assert(events.size() == 3u);
    assert(events[0].type == EventType::Info);
    assert(events[1].type == EventType::Breakpoint);
    assert(events[2].type == EventType::Exception);
    assert(events[1].location.line == 11u);
    assert(events[2].location.line == 12u);

    return 0;
}
