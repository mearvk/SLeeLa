#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class TypeDescriptor{std::string value_; public: TypeDescriptor(); explicit TypeDescriptor(std::string value); virtual ~TypeDescriptor()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}