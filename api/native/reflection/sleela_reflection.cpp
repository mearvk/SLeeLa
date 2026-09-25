#include "sleela_reflection.hpp"
namespace sleela::reflection{bool Registry::add(Type t){return types.emplace(t.name,std::move(t)).second;}const Type* Registry::find(const std::string&n)const{auto i=types.find(n);return i==types.end()?nullptr:&i->second;}std::vector<std::string> Registry::names()const{std::vector<std::string>r;for(auto const&x:types)r.push_back(x.first);return r;}}
