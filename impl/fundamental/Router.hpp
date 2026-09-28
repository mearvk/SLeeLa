#pragma once
#include <functional>
#include <string>
#include <unordered_map>
namespace sleela::fundamental { class Router { std::unordered_map<std::string,std::function<std::string(const std::string&)>> routes_; public: void route(std::string,std::string,std::function<std::string(const std::string&)>); std::string dispatch(const std::string&,const std::string&,const std::string&)const; }; }