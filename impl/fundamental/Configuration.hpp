#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Configuration{std::string value_; public: Configuration(); explicit Configuration(std::string value); virtual ~Configuration()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}