#pragma once
#include <string>
#include <unordered_map>
namespace sleela::fundamental { class Event { std::string name_,payload_;std::unordered_map<std::string,std::string> fields_; public: Event(std::string={},std::string={}); const std::string& name()const noexcept; const std::string& payload()const noexcept; void set(const std::string&,const std::string&); std::string get(const std::string&)const; }; }