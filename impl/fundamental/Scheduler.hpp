#pragma once
#include <functional>
#include <chrono>
#include <vector>
namespace sleela::fundamental { class Scheduler { struct Entry{std::size_t id;std::chrono::steady_clock::time_point due;std::function<void()> fn;}; std::vector<Entry> entries_;std::size_t next_{1}; public: std::size_t schedule(std::function<void()>,std::chrono::milliseconds); void run_due(); }; }