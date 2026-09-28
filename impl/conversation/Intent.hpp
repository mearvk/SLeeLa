#pragma once
#include <string>
namespace sleela::conversation { class Intent { std::string value_; public: explicit Intent(std::string v={}):value_(std::move(v)){} const std::string& value()const noexcept{return value_;} }; }