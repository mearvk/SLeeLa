#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Error{std::string value_; public: Error(); explicit Error(std::string value); virtual ~Error()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}