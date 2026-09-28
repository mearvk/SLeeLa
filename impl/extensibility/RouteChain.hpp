#pragma once
#include <vector>
#include "RouteNode.hpp"
#include "RouteContext.hpp"
#include "RouteDecision.hpp"
namespace sleela::extensibility { class RouteChain { std::vector<RouteNode> nodes_; public: void add(RouteNode n){nodes_.push_back(std::move(n));} RouteDecision resolve(RouteContext& c,RouteTarget& t)const{for(const auto& n:nodes_)if(c.has(n.name())){t=n.target();c.set("sleela.route.selected",n.name());return RouteDecision::Terminal;}return nodes_.empty()?RouteDecision::Unresolved:RouteDecision::Continue;} }; }