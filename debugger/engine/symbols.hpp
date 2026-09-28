#pragma once
#include "../debugger.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger::engine {
struct Symbol { std::string module,name; std::uint64_t address{}; SourceLocation location; };
class SymbolEngine { std::vector<Symbol> symbols_; public: void add(Symbol); const Symbol* byAddress(std::uint64_t)const noexcept; const Symbol* byName(const std::string&)const noexcept; const std::vector<Symbol>& all()const noexcept{return symbols_;} };
}