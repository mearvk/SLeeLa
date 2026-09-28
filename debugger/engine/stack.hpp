#pragma once
#include "../debugger.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger::engine {
struct Frame { std::uint64_t address{}; std::string module; std::string function; SourceLocation location; std::string arguments; };
class StackEngine { std::vector<Frame> frames_; std::size_t selected_{}; public: void replace(std::vector<Frame>); bool select(std::size_t); const std::vector<Frame>& frames()const noexcept{return frames_;} const Frame* selected()const noexcept; };
}