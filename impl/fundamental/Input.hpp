#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Input{std::string value_; public: Input(); explicit Input(std::string value); virtual ~Input()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}