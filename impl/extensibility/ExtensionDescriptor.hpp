#pragma once
#include <string>
#include <unordered_map>
namespace sleela::extensibility { class ExtensionDescriptor { std::unordered_map<std::string,std::string> fields_; public: void set(std::string k,std::string v){fields_[std::move(k)]=std::move(v);} const std::string* get(const std::string& k)const{auto i=fields_.find(k);return i==fields_.end()?nullptr:&i->second;} }; }