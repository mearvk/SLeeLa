#include "breakpoints.hpp"
namespace sleela::debugger::engine {
std::uint64_t BreakpointEngine::add(BreakpointSpec s){auto id=next_id_++;records_.push_back({id,std::move(s),true,0});return id;}
bool BreakpointEngine::remove(std::uint64_t id){for(auto i=records_.begin();i!=records_.end();++i)if(i->id==id){records_.erase(i);return true;}return false;}
bool BreakpointEngine::setEnabled(std::uint64_t id,bool e){for(auto&r:records_)if(r.id==id){r.enabled=e;return true;}return false;}
bool BreakpointEngine::hit(std::uint64_t id){for(auto&r:records_)if(r.id==id&&r.enabled){++r.hit_count;if(r.spec.temporary)r.enabled=false;return r.hit_count>r.spec.ignore_count;}return false;}
}