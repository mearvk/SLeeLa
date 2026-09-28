#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
namespace sleela::debugger::engine {
struct ExpressionResult { bool success{false}; std::uint64_t value{}; std::string error; };
class ExpressionEngine { public: ExpressionResult evaluate(const std::string&,const std::unordered_map<std::string,std::uint64_t>&)const; };
}