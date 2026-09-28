#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental {
class Name{std::string value_;
public:
 Name(); explicit Name(std::string value); virtual ~Name()=default;
 const std::string& value() const noexcept; void setValue(std::string value);
 bool empty() const noexcept; virtual const char* responsibility() const noexcept;
};}