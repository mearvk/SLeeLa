#include "debug_conformance.hpp"
#include <sstream>
namespace sleela::debugger {
void ConformanceSuite::add(ConformanceRecord r){records_.push_back(std::move(r));}
bool ConformanceSuite::passes(const ConformanceRecord&r)const noexcept{return r.model&&r.implemented&&r.integrated&&r.verified&&!r.capability.empty()&&!r.platform.empty();}
bool ConformanceSuite::releaseReady()const noexcept{if(records_.empty())return false;for(const auto&r:records_)if(!passes(r))return false;return true;}
std::string ConformanceSuite::report()const{std::ostringstream o;for(const auto&r:records_)o<<r.capability<<" ["<<(passes(r)?"PASS":"FAIL")<<"] platform="<<r.platform<<" backend="<<r.backend<<"\n";o<<"release_ready="<<(releaseReady()?"true":"false")<<"\n";return o.str();}
}