#include "Version.hpp"
namespace sleela::fundamental {
Version::Version()=default; Version::Version(std::string value):value_(std::move(value)){}
const std::string& Version::value() const noexcept{return value_;}
void Version::setValue(std::string value){value_=std::move(value);}
bool Version::empty() const noexcept{return value_.empty();}
const char* Version::responsibility() const noexcept{return "version representation";}
}