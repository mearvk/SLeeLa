#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger::engine {
struct ThreadRecord { std::uint64_t id{}; std::string name; bool stopped{false}; bool current{false}; };
class ThreadEngine { std::vector<ThreadRecord> threads_; std::uint64_t current_id_{}; public: void replace(std::vector<ThreadRecord>); bool select(std::uint64_t); const std::vector<ThreadRecord>& list()const noexcept{return threads_;} std::uint64_t current()const noexcept{return current_id_;} };
}