#pragma once
#include <vector>
#include "SourceBinding.hpp"
#include "RouteContext.hpp"
namespace sleela::extensibility { class SourceResolver { std::vector<SourceBinding> bindings_; public: void bind(SourceBinding b){bindings_.push_back(std::move(b));} const std::string* resolve(const RouteContext& c)const{for(const auto& b:bindings_)if(c.has(b.route()))return &b.source();return nullptr;} }; }