#pragma once
#include <string>
#include <vector>
#include <unordered_map>
namespace sleela::conversation { class Content { std::string value_; std::vector<std::string> items_; std::unordered_map<std::string,std::string> fields_; public: explicit Content(std::string v={}):value_(std::move(v)){} const std::string& value()const noexcept{return value_;} void set(const std::string&k,const std::string&v){fields_[k]=v;} std::string get(const std::string&k)const{auto i=fields_.find(k);return i==fields_.end()?std::string{}:i->second;} void add(const std::string&v){items_.push_back(v);} const std::vector<std::string>& items()const noexcept{return items_;} }; }