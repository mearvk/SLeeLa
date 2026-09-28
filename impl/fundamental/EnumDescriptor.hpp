#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class EnumDescriptor{std::string value_; public: EnumDescriptor(); explicit EnumDescriptor(std::string value); virtual ~EnumDescriptor()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}