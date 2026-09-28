#include "registers.hpp"
namespace sleela::debugger::engine { const RegisterValue* RegisterSet::find(const std::string&n)const noexcept{for(const auto&r:values)if(r.name==n)return&r;return nullptr;} }