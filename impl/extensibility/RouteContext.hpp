#pragma once
#include <string>
#include <unordered_map>
namespace sleela::extensibility { class RouteContext { std::unordered_map<std::string,std::string> values_; public: void set(std::string k,std::string v){values_[std::move(k)]=std::move(v);} bool has(const std::string& k)const{return values_.count(k)!=0;} const std::string* get(const std::string& k)const{auto i=values_.find(k);return i==values_.end()?nullptr:&i->second;} }; }