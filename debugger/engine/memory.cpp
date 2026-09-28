#include "memory.hpp"
#include <iomanip>
#include <sstream>
namespace sleela::debugger::engine {
std::string MemoryEngine::hex(const std::uint8_t*p,std::size_t n){std::ostringstream o;o<<std::hex<<std::setfill('0');for(std::size_t i=0;i<n;++i)o<<std::setw(2)<<(unsigned)p[i];return o.str();}
bool MemoryEngine::validRange(std::uint64_t a,std::size_t n,std::uint64_t l)noexcept{return a<=l&&n<=l-a;}
}