#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Buffer{std::string value_; public: Buffer(); explicit Buffer(std::string value); virtual ~Buffer()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}