#pragma once
#include <string>
#include <unordered_map>
#include "RouteChain.hpp"
namespace sleela::extensibility {
// A keyed collection of RouteChains. SourceRouter adds a chain under a string
// key and later selects the chain for a key to resolve a RouteContext against.
class HierarchyRouter {
    std::unordered_map<std::string, RouteChain> chains_;
public:
    void add(std::string key, RouteChain c) { chains_[std::move(key)] = std::move(c); }
    const RouteChain* select(const std::string& key) const {
        auto it = chains_.find(key);
        return it == chains_.end() ? nullptr : &it->second;
    }
    bool empty() const noexcept { return chains_.empty(); }
};
}
