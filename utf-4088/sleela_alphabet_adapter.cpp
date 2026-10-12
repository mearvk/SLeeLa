#include "sleela_alphabet_adapter.hpp"

namespace utf4088 {

bool is_frontend_identifier(std::uint64_t id) {
    const auto& records = frontend_registry();
    for (const auto& record : records) {
        if (record.codepoint == id) return true;
    }
    return false;
}

std::optional<CharacterRecord> lookup_frontend_identifier(std::uint64_t id) {
    const auto& records = frontend_registry();
    for (const auto& record : records) {
        if (record.codepoint == id) return record;
    }
    return std::nullopt;
}

} // namespace utf4088
