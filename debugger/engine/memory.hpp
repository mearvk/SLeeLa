#pragma once
#include <cstdint>
#include <string>
namespace sleela::debugger::engine {
struct MemoryRegion { std::uint64_t start{},end{}; std::string permissions; std::string module; };
class MemoryEngine { public: static std::string hex(const std::uint8_t*,std::size_t); static bool validRange(std::uint64_t,std::size_t,std::uint64_t=~std::uint64_t(0))noexcept; };
}