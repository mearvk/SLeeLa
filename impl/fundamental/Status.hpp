#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental {
class Status{std::string value_;
public:
 Status(); explicit Status(std::string value); virtual ~Status()=default;
 const std::string& value() const noexcept; void setValue(std::string value);
 bool empty() const noexcept; virtual const char* responsibility() const noexcept;
};}