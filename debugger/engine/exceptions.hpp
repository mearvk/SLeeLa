#pragma once
#include <cstdint>
#include <string>
namespace sleela::debugger::engine {
enum class ExceptionKind { Signal, AccessViolation, IllegalInstruction, DivideByZero, Assertion, Sanitizer, Unknown };
struct ExceptionRecord { ExceptionKind kind{ExceptionKind::Unknown}; std::int64_t code{}; std::string message; std::uint64_t address{}; bool fatal{false}; };
const char* exceptionKindName(ExceptionKind)noexcept;
}