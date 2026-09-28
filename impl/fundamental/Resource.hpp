#pragma once
#include <string>
#include <utility>
namespace sleela::fundamental {
class Resource{std::string value_;
public:
 Resource(); explicit Resource(std::string value); virtual ~Resource()=default;
 const std::string& value() const noexcept; void setValue(std::string value);
 bool empty() const noexcept; virtual const char* responsibility() const noexcept;
};}