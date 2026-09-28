#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Thread { std::string value_; public: Thread(); explicit Thread(std::string value); virtual ~Thread()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}