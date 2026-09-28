#pragma once
#include <string>
namespace sleela::fundamental { class Environment { public: static bool has(const std::string&); static std::string get(const std::string&,const std::string&={}); static bool set(const std::string&,const std::string&); static bool unset(const std::string&); }; }