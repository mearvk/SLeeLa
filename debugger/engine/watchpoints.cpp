#include "watchpoints.hpp"
namespace sleela::debugger::engine {
std::uint64_t WatchpointEngine::add(WatchpointSpec s){auto id=next_id_++;records_.push_back({id,std::move(s),true,0,{},{}});return id;}
bool WatchpointEngine::remove(std::uint64_t id){for(auto i=records_.begin();i!=records_.end();++i)if(i->id==id){records_.erase(i);return true;}return false;}
bool WatchpointEngine::updateValue(std::uint64_t id,std::string oldv,std::string newv){for(auto&r:records_)if(r.id==id&&r.enabled){r.last_value=std::move(oldv);r.new_value=std::move(newv);++r.hit_count;return r.last_value!=r.new_value;}return false;}
}