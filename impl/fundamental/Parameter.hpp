#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Parameter{std::string value_; public: Parameter(); explicit Parameter(std::string value); virtual ~Parameter()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}