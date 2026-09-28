#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Serializer{std::string value_; public: Serializer(); explicit Serializer(std::string value); virtual ~Serializer()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}