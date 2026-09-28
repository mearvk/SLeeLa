#pragma once
#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <vector>
namespace sleela::debugger {
enum class EventType { Info, Breakpoint, Watchpoint, Exception, Assertion, Thread, Log, Coverage, Sanitizer, Test, Regression, Crash };
struct SourceLocation { std::string file; std::uint32_t line{0}; std::uint32_t column{0}; };
struct Breakpoint { std::uint64_t id{0}; std::string function; SourceLocation location; std::string condition; bool enabled{true}; };
struct Watchpoint { std::uint64_t id{0}; std::string expression; std::string last_value; bool enabled{true}; };
struct StackFrame { std::string module; std::string function; SourceLocation location; };
struct DebugEvent {
  EventType type{EventType::Info}; std::string severity{"info"}; std::string message; std::string thread;
  SourceLocation location; std::vector<StackFrame> stack; std::map<std::string,std::string> fields;
};
class DiagnosticReport { public: static void write(std::ostream&, const std::vector<DebugEvent>&); };
class DebugSession {
public:
  std::uint64_t addBreakpoint(Breakpoint); std::uint64_t addWatchpoint(Watchpoint);
  bool removeBreakpoint(std::uint64_t); bool removeWatchpoint(std::uint64_t);
  void emit(DebugEvent);
  const std::vector<DebugEvent>& events() const noexcept { return events_; }
  void report(std::ostream&) const;
private:
  std::uint64_t next_id_{1}; std::vector<Breakpoint> breakpoints_; std::vector<Watchpoint> watchpoints_; std::vector<DebugEvent> events_;
};
}
