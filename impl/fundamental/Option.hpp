#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Option{std::string value_; public: Option(); explicit Option(std::string value); virtual ~Option()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}