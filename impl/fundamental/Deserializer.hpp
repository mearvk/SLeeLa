#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Deserializer{std::string value_; public: Deserializer(); explicit Deserializer(std::string value); virtual ~Deserializer()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}