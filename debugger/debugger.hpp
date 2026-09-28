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

enum class StopReason { None, Breakpoint, Watchpoint, Exception, Signal, Assertion, Sanitizer, Crash, UserPause, TestFailure };
struct StopRecord { StopReason reason{StopReason::None}; std::uint64_t breakpoint_id{0}; std::uint64_t watchpoint_id{0}; std::string thread; SourceLocation location; std::string function; std::string detail; std::uint64_t event_index{0}; };
struct EvidenceRecord { std::uint64_t sequence{0}; std::string observation; std::string event; SourceLocation source; std::string thread; std::string stack_summary; std::string action; std::string backend_result; std::string diagnostic; };

struct DebugEvent {
  EventType type{EventType::Info};
  std::string severity{"info"};
  std::string message;
  std::string thread;
  SourceLocation location;
  std::vector<StackFrame> stack;
  std::map<std::string,std::string> fields;
};

enum class StopReason { None, Breakpoint, Watchpoint, Exception, Signal, Assertion, Sanitizer, Crash, UserPause, TestFailure };

struct StopRecord {
  StopReason reason{StopReason::None};
  std::uint64_t breakpoint_id{0};
  std::uint64_t watchpoint_id{0};
  std::string thread;
  SourceLocation location;
  std::string function;
  std::string detail;
  std::uint64_t event_index{0};
};

struct EvidenceRecord {
  std::uint64_t sequence{0};
  std::string observation;
  std::string event;
  SourceLocation source;
  std::string thread;
  std::string stack_summary;
  std::string action;
  std::string backend_result;
  std::string diagnostic;
};

class DiagnosticReport {
public:
  static void write(std::ostream&, const std::vector<DebugEvent>&);
  static void writeEvidence(std::ostream&, const std::vector<EvidenceRecord>&);
};

class DebugSession {
public:
  std::uint64_t addBreakpoint(Breakpoint);
  std::uint64_t addWatchpoint(Watchpoint);
  bool removeBreakpoint(std::uint64_t);
  bool removeWatchpoint(std::uint64_t);
  void emit(DebugEvent);
  void setStopRecord(StopRecord record) { stop_ = std::move(record); }
  const StopRecord& stopRecord() const noexcept { return stop_; }
  bool hasStopRecord() const noexcept { return stop_.reason != StopReason::None; }
  std::uint64_t addEvidence(EvidenceRecord record);
  const std::vector<EvidenceRecord>& evidence() const noexcept { return evidence_; }
  const std::vector<DebugEvent>& events() const noexcept { return events_; }

  void setStopRecord(StopRecord record) { stop_ = std::move(record); }
  const StopRecord& stopRecord() const noexcept { return stop_; }
  bool hasStopRecord() const noexcept { return stop_.reason != StopReason::None; }

  std::uint64_t addEvidence(EvidenceRecord record);
  const std::vector<EvidenceRecord>& evidence() const noexcept { return evidence_; }

  void report(std::ostream&) const;
private:
  std::uint64_t next_id_{1}; std::uint64_t next_evidence_{1};
  std::uint64_t next_evidence_{1};
  std::vector<Breakpoint> breakpoints_;
  std::vector<Watchpoint> watchpoints_;
  std::vector<DebugEvent> events_; std::vector<EvidenceRecord> evidence_; StopRecord stop_;
  std::vector<EvidenceRecord> evidence_;
  StopRecord stop_;
};

const char* stopReasonName(StopReason) noexcept;

}
