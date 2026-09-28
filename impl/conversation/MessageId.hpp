#pragma once
#include <string>
namespace sleela::conversation { class MessageId { std::string value_; public: explicit MessageId(std::string v={}):value_(std::move(v)){} const std::string& value()const noexcept{return value_;} bool empty()const noexcept{return value_.empty();} }; }