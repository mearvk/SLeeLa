#ifndef SLEELA_DEBUGGER_ACTIONS_HPP
#define SLEELA_DEBUGGER_ACTIONS_HPP

#include "debugger.hpp"
#include <string>

namespace sleela::debugger {

enum class ActionStatus { Requested, Validated, Supported, Executed, Completed, Failed };

enum class DebugAction {
    Continue, Pause, StepIn, StepOver, StepOut, Break, BreakConditional, BreakOnce,
    Watch, Exception, Thread, Stack, Memory, Register, Inspect, Trace, History,
    Attach, Detach, Launch, Restart, Terminate, Report
};

struct ActionRequest {
    DebugAction action;
    std::string argument;
};

struct ActionResult {
    ActionStatus status = ActionStatus::Requested;
    bool success = false;
    std::string message;
};

const char* actionName(DebugAction action) noexcept;
const char* actionStatusName(ActionStatus status) noexcept;

class ActionController {
public:
    explicit ActionController(DebugSession& session) : session_(session) {}
    ActionResult validate(const ActionRequest& request) const;
private:
    DebugSession& session_;
};

}

#endif
