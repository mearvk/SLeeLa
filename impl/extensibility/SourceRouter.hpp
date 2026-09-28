#pragma once
#include <string>
#include "HierarchyRouter.hpp"
#include "SourceResolver.hpp"
namespace sleela::extensibility { class SourceRouter { HierarchyRouter hierarchy_; SourceResolver resolver_; public: void add(std::string key,RouteChain c){hierarchy_.add(std::move(key),std::move(c));} void bind(SourceBinding b){resolver_.bind(std::move(b));} const std::string* route(const std::string& key,RouteContext& c)const{if(const auto* chain=hierarchy_.select(key)){RouteTarget t;if(chain->resolve(c,t)==RouteDecision::Terminal)c.set("sleela.route.target",t.value());}return resolver_.resolve(c);} }; }