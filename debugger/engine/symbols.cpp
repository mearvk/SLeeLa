#include "symbols.hpp"
namespace sleela::debugger::engine {
void SymbolEngine::add(Symbol s){symbols_.push_back(std::move(s));}
const Symbol* SymbolEngine::byAddress(std::uint64_t a)const noexcept{const Symbol*best=nullptr;for(const auto&s:symbols_)if(s.address<=a&&(!best||s.address>best->address))best=&s;return best;}
const Symbol* SymbolEngine::byName(const std::string&n)const noexcept{for(const auto&s:symbols_)if(s.name==n)return&s;return nullptr;}
}