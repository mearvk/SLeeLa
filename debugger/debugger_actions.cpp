#include "debugger_actions.hpp"

namespace sleela::debugger {

const char* actionName(DebugAction a) noexcept {
    switch (a) {
        case DebugAction::Continue: return "continue";
        case DebugAction::Pause: return "pause";
        case DebugAction::StepIn: return "step-in";
        case DebugAction::StepOver: return "step-over";
        case DebugAction::StepOut: return "step-out";
        case DebugAction::Break: return "break";
        case DebugAction::BreakConditional: return "break-conditional";
        case DebugAction::BreakOnce: return "break-once";
        case DebugAction::Watch: return "watch";
        case DebugAction::Exception: return "exception";
        case DebugAction::Thread: return "thread";
        case DebugAction::Stack: return "stack";
        case DebugAction::Memory: return "memory";
        case DebugAction::Register: return "register";
        case DebugAction::Inspect: return "inspect";
        case DebugAction::Trace: return "trace";
        case DebugAction::History: return "history";
        case DebugAction::Attach: return "attach";
        case DebugAction::Detach: return "detach";
        case DebugAction::Launch: return "launch";
        case DebugAction::Restart: return "restart";
        case DebugAction::Terminate: return "terminate";
        case DebugAction::Report: return "report";
    }
    return "unknown";
}

const char* actionStatusName(ActionStatus s) noexcept {
    switch (s) {
        case ActionStatus::Requested: return "requested";
        case ActionStatus::Validated: return "validated";
        case ActionStatus::Supported: return "supported";
        case ActionStatus::Executed: return "executed";
        case ActionStatus::Completed: return "completed";
        case ActionStatus::Failed: return "failed";
    }
    return "unknown";
}

ActionResult ActionController::validate(const ActionRequest& request) const {
    if (request.argument.size() > 4096u)
        return {ActionStatus::Failed, false, "action argument too large"};
    return {ActionStatus::Validated, true, actionName(request.action)};
}

}
