#pragma once
#include <functional>
#include <string>
namespace sleela::fundamental { class Handler { std::function<std::string(const std::string&)> f_; public: explicit Handler(std::function<std::string(const std::string&)>={}); std::string invoke(const std::string&)const; bool valid()const noexcept; }; }