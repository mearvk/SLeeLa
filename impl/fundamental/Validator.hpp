#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Validator{std::string value_; public: Validator(); explicit Validator(std::string value); virtual ~Validator()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}