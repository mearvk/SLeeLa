#include "data_structures.hpp"

namespace sleela::data {

void ObjectRecord::set(const std::string& key, Value value) {
    fields[key] = std::move(value);
}

bool ObjectRecord::get(const std::string& key, Value& out) const {
    auto it = fields.find(key);
    if (it == fields.end()) return false;
    out = it->second;
    return true;
}

} // namespace sleela::data
