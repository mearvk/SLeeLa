#include "Condition.hpp"
namespace sleela::fundamental {
Condition::Condition() = default;
Condition::Condition(std::string value) : value_(std::move(value)) {}
const std::string& Condition::value() const noexcept { return value_; }
void Condition::setValue(std::string value) { value_ = std::move(value); }
bool Condition::empty() const noexcept { return value_.empty(); }
const char* Condition::responsibility() const noexcept { return "condition synchronization"; }
}