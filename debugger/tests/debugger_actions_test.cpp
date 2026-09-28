#include "../debugger.hpp"
#include "../debugger_actions.hpp"
#include <cassert>
#include <string>

int main() {
    using namespace sleela::debugger;
    DebugSession session;
    ActionController controller(session);

    const DebugAction actions[] = {
        DebugAction::Continue, DebugAction::Pause, DebugAction::StepIn,
        DebugAction::StepOver, DebugAction::StepOut, DebugAction::Break,
        DebugAction::BreakConditional, DebugAction::BreakOnce, DebugAction::Watch,
        DebugAction::Exception, DebugAction::Thread, DebugAction::Stack,
        DebugAction::Memory, DebugAction::Register, DebugAction::Inspect,
        DebugAction::Trace, DebugAction::History, DebugAction::Attach,
        DebugAction::Detach, DebugAction::Launch, DebugAction::Restart,
        DebugAction::Terminate, DebugAction::Report
    };
    for (const auto action : actions) {
        const auto result = controller.validate({action, ""});
        assert(result.success);
        assert(result.status == ActionStatus::Validated);
        assert(std::string(actionName(action)) != "unknown");
    }
    const auto bad = controller.validate({DebugAction::Inspect, std::string(4097, 'x')});
    assert(!bad.success);
    assert(bad.status == ActionStatus::Failed);
    return 0;
}
