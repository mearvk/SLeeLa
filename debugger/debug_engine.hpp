#pragma once
#include "debugger.hpp"
#include "debugger_backend.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
namespace sleela::debugger {
enum class BreakpointKind { Software, Hardware, Function, Source, Address, Conditional, Temporary };
enum class WatchpointAccess { Read, Write, ReadWrite };
enum class ExceptionKind { Signal, AccessViolation, IllegalInstruction, DivideByZero, Assertion, Sanitizer, Unknown };
struct BreakpointSpec { BreakpointKind kind{BreakpointKind::Source}; SourceLocation location; std::string function; std::string address; std::string condition; bool temporary{false}; unsigned ignore_count{}; };
struct BreakpointRecord { std::uint64_t id{}; BreakpointSpec spec; bool enabled{true}; std::uint64_t hit_count{}; };
struct WatchpointSpec { std::string expression; std::uint64_t address{}; std::size_t size{}; WatchpointAccess access{WatchpointAccess::Write}; };
struct WatchpointRecord { std::uint64_t id{}; WatchpointSpec spec; bool enabled{true}; std::uint64_t hit_count{}; std::string last_value,new_value; };
struct ThreadRecord { std::uint64_t id{}; std::string name; bool stopped{}; bool current{}; };
struct StackFrameRecord { std::uint64_t address{}; std::string module,function,arguments; SourceLocation location; };
struct SymbolRecord { std::string module,name; std::uint64_t address{}; SourceLocation location; };
struct MemoryRegion { std::uint64_t start{},end{}; std::string permissions,module; };
struct RegisterValue { std::string name; std::uint64_t value{}; };
struct RegisterSnapshot { std::string architecture; std::vector<RegisterValue> values; };
struct ExceptionRecord { ExceptionKind kind{ExceptionKind::Unknown}; std::int64_t code{}; std::string message; std::uint64_t address{}; bool fatal{}; };
struct EvidenceBundle { std::string source_revision,executable_hash,toolchain,platform,command_line,exception,registers,stack,modules,memory_map,test_id,timestamp; std::vector<std::string> logs; std::string text() const; };
enum class CapabilityState { Unavailable, Declared, Implemented, Tested };
struct Capability { std::string name; CapabilityState state{CapabilityState::Unavailable}; std::string mechanism,platform; };
class DebugEngine {
 std::uint64_t next_breakpoint_{1},next_watchpoint_{1},current_thread_{};
 std::vector<BreakpointRecord> breakpoints_; std::vector<WatchpointRecord> watchpoints_; std::vector<ThreadRecord> threads_; std::vector<StackFrameRecord> stack_; std::vector<SymbolRecord> symbols_; std::vector<MemoryRegion> memory_; std::unordered_map<std::string,std::uint64_t> variables_; RegisterSnapshot registers_; ExceptionRecord exception_; EvidenceBundle evidence_; std::vector<Capability> capabilities_;
 public:
 DebugEngine();
 std::uint64_t addBreakpoint(BreakpointSpec); bool removeBreakpoint(std::uint64_t); bool setBreakpointEnabled(std::uint64_t,bool); bool hitBreakpoint(std::uint64_t);
 std::uint64_t addWatchpoint(WatchpointSpec); bool removeWatchpoint(std::uint64_t); bool updateWatchpoint(std::uint64_t,std::string,std::string);
 void setThreads(std::vector<ThreadRecord>); bool selectThread(std::uint64_t); std::uint64_t currentThread() const noexcept;
 void setStack(std::vector<StackFrameRecord>); void setSymbols(std::vector<SymbolRecord>); void setMemoryMap(std::vector<MemoryRegion>); void setRegisters(RegisterSnapshot); void setException(ExceptionRecord); void setEvidence(EvidenceBundle);
 const std::vector<BreakpointRecord>& breakpoints()const noexcept{return breakpoints_;} const std::vector<WatchpointRecord>& watchpoints()const noexcept{return watchpoints_;} const std::vector<ThreadRecord>& threads()const noexcept{return threads_;} const std::vector<StackFrameRecord>& stack()const noexcept{return stack_;} const std::vector<SymbolRecord>& symbols()const noexcept{return symbols_;} const RegisterSnapshot& registers()const noexcept{return registers_;} const ExceptionRecord& exception()const noexcept{return exception_;} const EvidenceBundle& evidence()const noexcept{return evidence_;}
 bool hasCapability(const std::string&)const; std::string capabilityReport()const; void setVariable(std::string,std::uint64_t); bool evaluate(const std::string&,std::uint64_t&)const;
 static std::string memoryHex(const std::uint8_t*,std::size_t); static bool validMemoryRange(std::uint64_t,std::size_t,std::uint64_t=~std::uint64_t(0))noexcept;
}; 
const char* capabilityStateName(CapabilityState)noexcept; const char* exceptionKindName(ExceptionKind)noexcept;
}