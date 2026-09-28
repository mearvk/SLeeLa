#include "capabilities.hpp"
#include <sstream>
namespace sleela::debugger::engine {
const char* capabilityStateName(CapabilityState s) noexcept { switch(s){case CapabilityState::Unavailable:return "unavailable";case CapabilityState::Declared:return "declared";case CapabilityState::Implemented:return "implemented";case CapabilityState::Tested:return "tested";} return "unknown"; }
void CapabilityReport::add(std::string n,std::string){add(n,CapabilityState::Declared,{},{});}
void CapabilityReport::add(const std::string& n,CapabilityState s,std::string m,std::string p){entries.push_back({n,s,std::move(m),std::move(p)});}
bool CapabilityReport::supports(const std::string& n) const {for(const auto&e:entries)if(e.name==n&&(e.state==CapabilityState::Implemented||e.state==CapabilityState::Tested))return true;return false;}
std::string CapabilityReport::text() const {std::ostringstream o;for(const auto&e:entries)o<<e.name<<"="<<capabilityStateName(e.state)<<" mechanism="<<e.mechanism<<" platform="<<e.platform<<"\n";return o.str();}
}