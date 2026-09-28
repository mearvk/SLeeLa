#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Result{std::string value_; public: Result(); explicit Result(std::string value); virtual ~Result()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}