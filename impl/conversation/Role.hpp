#pragma once
#include <string>
namespace sleela::conversation { class Role { std::string value_; public: explicit Role(std::string v={}):value_(std::move(v)){} const std::string& value()const noexcept{return value_;} }; }