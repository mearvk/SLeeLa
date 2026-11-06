#include "sleela_alphabet_adapter.hpp"

#include <unordered_map>

namespace utf4088 {
namespace {
const std::unordered_map<std::uint64_t, CharacterRecord>& identifier_index() {
    static const auto index = [] {
        std::unordered_map<std::uint64_t, CharacterRecord> result;
        const auto& records = frontend_registry();
        result.reserve(records.size());
        for (const auto& record : records) result.emplace(record.codepoint, record);
        return result;
    }();
    return index;
}
}

bool is_frontend_identifier(std::uint64_t id) {
    return identifier_index().find(id) != identifier_index().end();
}

std::optional<CharacterRecord> lookup_frontend_identifier(std::uint64_t id) {
    const auto& index = identifier_index();
    const auto found = index.find(id);
    if (found == index.end()) return std::nullopt;
    return found->second;
}

} // namespace utf4088
