#ifdef NDEBUG
#undef NDEBUG
#endif

#include "character_map.hpp"
#include "digraph_engine.hpp"
#include "utf4088.hpp"
#include "encoder_decoder.hpp"
#include "sleela_alphabet_adapter.hpp"
#include "voltage_field.hpp"
#include "glyph8x12.hpp"
#include "semantics/concept_symbols.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

int main() {
    using namespace utf4088;

    assert(!is_valid_code_point(0));
    assert(!is_valid_code_point(0x10FFFFULL));
    assert(is_valid_code_point(0x110000ULL));
    assert(is_valid_code_point(0x1FFFFFFFFULL));
    assert(!is_valid_code_point(0x200000000ULL));

    const InputFields sample{3, 7, 25, 128};
    const auto packedSample = pack_input(sample);
    assert(packedSample.has_value());
    assert(symbol_from_input(*packedSample).has_value());
    assert(!symbol_from_input(1ULL << 21).has_value());

    const auto first = generate_frontend_registry();
    const auto& cached = frontend_registry();
    assert(first.size() == 16606);
    assert(cached.size() == 16606);
    assert(first.front().integer_id == 0);
    assert(first.back().integer_id == 16605);
    for (std::size_t i = 0; i < first.size(); ++i) {
        assert(first[i].integer_id == i);
        assert(first[i].codepoint == 0x110000ULL + i);
        assert(first[i].shape_id == cached[i].shape_id);
        assert(first[i].meaning_id == cached[i].meaning_id);
    }
    assert(sleela_catalogue_capacity == 1048576);
    assert(is_frontend_identifier(0x110000ULL));
    assert(!is_frontend_identifier(0x10FFFFULL));
    const auto firstRecord = lookup_frontend_identifier(0x110000ULL);
    assert(firstRecord.has_value());
    assert(firstRecord->integer_id == 0);
    assert(!lookup_frontend_identifier(0x1FFFFFFFFULL).has_value());

    assert(derive_remainder_symbol(1, 2, 3, 4) ==
           derive_remainder_symbol(1, 2, 3, 4));
    assert(stage_name(Stage::Start) == "start");
    assert(language_name(Language::Korean) == "korean");

    const auto glyph = seed_glyph("utf4088-regression-seed");
    assert(glyph_signature(glyph) == glyph_signature(glyph));
    assert(analyze_glyph(glyph).black_pixels <= 96);

    const auto& conceptSymbols = semantics::concept_symbol_registry();
    assert(conceptSymbols.size() == 44);
    for (std::size_t i = 0; i < conceptSymbols.size(); ++i) {
        const auto& concept = conceptSymbols[i];
        assert(concept.id == i + 1);
        assert(concept.metrics.black_pixels <= 96);
        for (double coordinate : concept.anchor_4d) {
            assert(std::isfinite(coordinate));
            assert(coordinate >= 0.0 && coordinate <= 1.0);
        }
        for (std::size_t j = 0; j < i; ++j) {
            assert(concept.key != conceptSymbols[j].key);
            assert(concept.id != conceptSymbols[j].id);
            assert(concept.glyph.rows != conceptSymbols[j].glyph.rows);
        }
    }
    for (const char* key : {"wealth", "wood", "sex", "pleasure", "quality", "asia"}) {
        assert(semantics::find_concept_symbol(key) != nullptr);
    }

    const std::vector<VoltageSample> samples{
        {0, 0, 0, 0}, {1, 0, 1, 1}, {-1, 0, -1, 2},
        {0, 1, 2, 3}, {0, -1, -2, 4},
        {std::numeric_limits<double>::quiet_NaN(), 5, 8, 5}
    };
    const auto field = analyze_field(samples, 0);
    assert(std::isfinite(field.voltage));
    assert(std::isfinite(field.magnitude));
    assert(std::isfinite(field.direction));
    assert(std::isfinite(field.uniformity));
    assert(field_to_input(field) == field_to_input(field));
    assert(field_to_input(FieldState{std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::quiet_NaN(), 0, 0, 0, 0}) == 0);

    const std::vector<FieldState> states{
        {1, 0, 0, 1, 0, 1},
        {2, 0, 0, 2, 0, 1},
        {1, 0, 0, 1, 0, 1},
        {2, 0, 0, 2, 0, 1}
    };
    const auto graph = build_digraph(states);
    assert(graph.nodes.size() == 2);
    assert(graph.edges.size() == 2);
    for (const auto& edge : graph.edges) assert(edge.weight == 2.0);

    return 0;
}
