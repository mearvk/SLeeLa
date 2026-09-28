#include "sleela_reflection.hpp"
#include <functional>
namespace sleela::reflection {
bool Registry::add(Type t){if(t.name.empty())return false;std::lock_guard<std::mutex>g(mutex_);if(types_.find(t.name)!=types_.end())return false;types_.emplace(t.name,std::move(t));return true;}
bool Registry::remove(const std::string& n){std::lock_guard<std::mutex>g(mutex_);return types_.erase(n)!=0;}
const Type* Registry::find(const std::string& n)const{std::lock_guard<std::mutex>g(mutex_);auto i=types_.find(n);return i==types_.end()?nullptr:&i->second;}
std::vector<std::string> Registry::names()const{std::lock_guard<std::mutex>g(mutex_);std::vector<std::string>r;r.reserve(types_.size());for(const auto&x:types_)r.push_back(x.first);return r;}
void Registry::clear(){std::lock_guard<std::mutex>g(mutex_);types_.clear();}
std::string stableTypeId(const std::string& n,const std::string& v){return n+"@"+v;}
}
