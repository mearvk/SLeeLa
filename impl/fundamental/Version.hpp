#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental { class Version{std::string value_; public: Version(); explicit Version(std::string value); virtual ~Version()=default; const std::string& value() const noexcept; void setValue(std::string value); bool empty() const noexcept; virtual const char* responsibility() const noexcept; };}