#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Timer { std::string value_; public: Timer(); explicit Timer(std::string value); virtual ~Timer()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}