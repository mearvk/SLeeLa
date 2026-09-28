#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Schema{std::string value_; public: Schema(); explicit Schema(std::string value); virtual ~Schema()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}