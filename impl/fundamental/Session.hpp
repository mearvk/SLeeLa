#pragma once
#include <string>
namespace sleela::fundamental { class Session { std::string id_;bool open_{true}; public: explicit Session(std::string={}); const std::string& id()const noexcept; void close()noexcept; bool open()const noexcept; }; }