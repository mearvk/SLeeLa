#pragma once

#include "../algebra/glyph8x12.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace utf4088::semantics {

// A curated starter concept with an explicitly provisional normalized 4D anchor.
// Anchors are study fixtures, not measurements or validated semantic truths.
struct ConceptSymbol {
    std::uint32_t id;
    std::string_view key;
    std::string_view label;
    std::string_view domain;
    std::string_view definition;
    std::string_view related_keys;
    std::array<double, 4> anchor_4d; // x, y, normalized pressure, normalized voltage
    std::string glyph_seed;
    Glyph8x12 glyph;
    GlyphMetrics metrics;
};

// Return the immutable deterministic v1 concept-symbol registry.
const std::vector<ConceptSymbol>& concept_symbol_registry();
const ConceptSymbol* find_concept_symbol(std::string_view key);

} // namespace utf4088::semantics
