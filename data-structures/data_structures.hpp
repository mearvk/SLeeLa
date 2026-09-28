#ifndef SLEELA_DATA_STRUCTURES_HPP
#define SLEELA_DATA_STRUCTURES_HPP

#include <cstdint>
#include <deque>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace sleela::data {

using Value = std::variant<std::monostate, std::int64_t, double, bool, std::string>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Stack = std::vector<T>;

template <typename T>
using Queue = std::deque<T>;

using Map = std::unordered_map<std::string, Value>;

struct ObjectRecord {
    std::string name;
    Map fields;

    void set(const std::string& key, Value value);
    bool get(const std::string& key, Value& out) const;
};

} // namespace sleela::data

#endif
