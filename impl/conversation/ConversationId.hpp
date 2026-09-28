#pragma once
#include <string>
namespace sleela::conversation { class ConversationId { std::string value_; public: explicit ConversationId(std::string v={}):value_(std::move(v)){} const std::string& value()const noexcept{return value_;} bool empty()const noexcept{return value_.empty();} }; }