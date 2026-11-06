#include "encoder_decoder.hpp"
#include "algebra/glyph8x12.hpp"
#include "semantics/concept_symbols.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
void print_glyph(const utf4088::Glyph8x12& glyph) {
    for (const auto row : glyph.rows) {
        for (int x = 7; x >= 0; --x)
            std::cout << ((row >> x) & 1U ? "##" : "  ");
        std::cout << '\n';
    }
}
void usage() {
    std::cerr << "Usage:\n"
              << "  utf4088-inspect --id HEX_IDENTIFIER\n"
              << "  utf4088-inspect --concept CONCEPT_KEY\n"
              << "  utf4088-inspect --list-concepts\n";
}
}

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--list-concepts") {
        for (const auto& c : utf4088::semantics::concept_symbol_registry())
            std::cout << c.id << "\t" << c.key << "\t" << c.domain << "\n";
        return 0;
    }
    if (argc == 3 && std::string(argv[1]) == "--concept") {
        const auto* c = utf4088::semantics::find_concept_symbol(argv[2]);
        if (!c) { std::cerr << "Unknown concept key\n"; return 2; }
        std::cout << c->label << " [" << c->key << "]\n"
                  << c->definition << "\n"
                  << "status: provisional concept seed; glyph is not a validated character\n";
        print_glyph(c->glyph);
        return 0;
    }
    if (argc == 3 && std::string(argv[1]) == "--id") {
        try {
            const auto id = static_cast<utf4088::CodePoint>(std::stoull(argv[2], nullptr, 0));
            const auto f = utf4088::decode_input(id);
            if (!f) { std::cerr << "Identifier is not a valid V1 input-state identifier\n"; return 2; }
            std::cout << "UTF-4088 experimental identifier: 0x" << std::hex << id << std::dec << "\n"
                      << "x=" << unsigned(f->x) << " y=" << unsigned(f->y)
                      << " pressure=" << unsigned(f->pressure)
                      << " normalized_voltage=" << unsigned(f->voltage) << "\n"
                      << "Preview only: generated with the C++ seed-glyph routine; "
                         "not the SHA-256 catalogue bitmap.\n";
            const auto glyph = utf4088::seed_glyph("UTF4088-INPUT-V1:" + std::to_string(id));
            print_glyph(glyph);
            return 0;
        } catch (...) { std::cerr << "Invalid integer identifier\n"; return 2; }
    }
    usage();
    return 2;
}
