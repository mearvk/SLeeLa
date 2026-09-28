#include "Clock.hpp"
namespace sleela::fundamental {
Clock::Clock() = default;
Clock::Clock(std::string value) : value_(std::move(value)) {}
const std::string& Clock::value() const noexcept { return value_; }
void Clock::setValue(std::string value) { value_ = std::move(value); }
bool Clock::empty() const noexcept { return value_.empty(); }
const char* Clock::responsibility() const noexcept { return "monotonic and wall-clock time source"; }
}