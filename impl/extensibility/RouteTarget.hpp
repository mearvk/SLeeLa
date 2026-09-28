#pragma once
#include <string>
namespace sleela::extensibility { class RouteTarget { std::string value_; public: RouteTarget()=default; explicit RouteTarget(std::string v):value_(std::move(v)){} const std::string& value()const noexcept{return value_;} bool empty()const noexcept{return value_.empty();} }; }