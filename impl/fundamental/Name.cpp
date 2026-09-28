#include "Name.hpp"
namespace sleela::fundamental {
Name::Name()=default; Name::Name(std::string value):value_(std::move(value)){}
const std::string& Name::value() const noexcept{return value_;}
void Name::setValue(std::string value){value_=std::move(value);}
bool Name::empty() const noexcept{return value_.empty();}
const char* Name::responsibility() const noexcept{return "validated symbolic naming";}
}