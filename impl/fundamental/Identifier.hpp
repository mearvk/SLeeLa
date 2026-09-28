#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Identifier{std::string value_; public: Identifier(); explicit Identifier(std::string value); virtual ~Identifier()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}