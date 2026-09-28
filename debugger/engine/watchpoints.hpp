#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger::engine {
enum class WatchpointAccess { Read, Write, ReadWrite };
struct WatchpointSpec { std::string expression; std::uint64_t address{}; std::size_t size{}; WatchpointAccess access{WatchpointAccess::Write}; };
struct WatchpointRecord { std::uint64_t id{}; WatchpointSpec spec; bool enabled{true}; std::uint64_t hit_count{}; std::string last_value; std::string new_value; };
class WatchpointEngine { std::uint64_t next_id_{1}; std::vector<WatchpointRecord> records_; public: std::uint64_t add(WatchpointSpec); bool remove(std::uint64_t); bool updateValue(std::uint64_t,std::string,std::string); const std::vector<WatchpointRecord>& records()const noexcept{return records_;} };
}