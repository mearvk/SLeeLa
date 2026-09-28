#include "threads.hpp"
namespace sleela::debugger::engine {
void ThreadEngine::replace(std::vector<ThreadRecord> t){threads_=std::move(t);current_id_=0;for(auto&x:threads_)x.current=false;}
bool ThreadEngine::select(std::uint64_t id){for(auto&x:threads_)x.current=(x.id==id);for(const auto&x:threads_)if(x.current){current_id_=id;return true;}return false;}
}