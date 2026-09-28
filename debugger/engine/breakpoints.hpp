#pragma once
#include "../debugger.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger::engine {
enum class BreakpointKind { Software, Hardware, Function, Source, Address, Conditional, Temporary };
struct BreakpointSpec { BreakpointKind kind{BreakpointKind::Source}; SourceLocation location; std::string function; std::string address; std::string condition; bool temporary{false}; unsigned ignore_count{0}; };
struct BreakpointRecord { std::uint64_t id{}; BreakpointSpec spec; bool enabled{true}; std::uint64_t hit_count{}; };
class BreakpointEngine { std::uint64_t next_id_{1}; std::vector<BreakpointRecord> records_; public: std::uint64_t add(BreakpointSpec); bool remove(std::uint64_t); bool setEnabled(std::uint64_t,bool); bool hit(std::uint64_t); const std::vector<BreakpointRecord>& records()const noexcept{return records_;} };
}