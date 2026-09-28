#include "Status.hpp"
namespace sleela::fundamental {
Status::Status()=default; Status::Status(std::string value):value_(std::move(value)){}
const std::string& Status::value() const noexcept{return value_;}
void Status::setValue(std::string value){value_=std::move(value);}
bool Status::empty() const noexcept{return value_.empty();}
const char* Status::responsibility() const noexcept{return "operation state";}
}