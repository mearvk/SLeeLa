#pragma once
#include <string>
#include "RouteTarget.hpp"
namespace sleela::extensibility { class RouteNode { std::string name_; RouteTarget target_; public: RouteNode(std::string n,RouteTarget t):name_(std::move(n)),target_(std::move(t)){} const std::string& name()const noexcept{return name_;} const RouteTarget& target()const noexcept{return target_;} }; }