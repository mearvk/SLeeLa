#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class ByteStream{std::string value_; public: ByteStream(); explicit ByteStream(std::string value); virtual ~ByteStream()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}