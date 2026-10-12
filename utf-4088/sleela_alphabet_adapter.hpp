#pragma once

#include "character_map.hpp"
#include <cstdint>
#include <optional>

namespace utf4088 {

// Adapter for SLeeLa character-catalogue clients. Experimental IDs remain
// uint64_t and are never coerced into Unicode scalar values.
bool is_frontend_identifier(std::uint64_t id);
std::optional<CharacterRecord> lookup_frontend_identifier(std::uint64_t id);
constexpr std::uint32_t sleela_catalogue_capacity = 1048576;

} // namespace utf4088
