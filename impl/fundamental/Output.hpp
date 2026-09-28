#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Output{std::string value_; public: Output(); explicit Output(std::string value); virtual ~Output()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}