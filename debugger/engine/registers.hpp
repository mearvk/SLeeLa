#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger::engine {
struct RegisterValue { std::string name; std::uint64_t value{}; };
struct RegisterSet { std::string architecture; std::vector<RegisterValue> values; const RegisterValue* find(const std::string&)const noexcept; };
}