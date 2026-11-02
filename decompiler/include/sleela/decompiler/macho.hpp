#pragma once
#include "decompiler.hpp"
#include <vector>
namespace sleela::decompiler {
// Static Mach-O evidence for thin (32/64-bit) and FAT/universal images. The image is
// never mapped; records are derived from the header, load commands, and symbol table.
struct MachoAnalysis {
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
    std::vector<Import> imports;
    std::vector<Export> exports;
    LibraryMetadata library;
    Architecture architecture{Architecture::Unknown};
    ArtifactClass artifact_class{ArtifactClass::Unknown};
    bool fat{};
    std::uint32_t slice_count{};
    bool valid{};
};
MachoAnalysis analyze_macho(std::span<const std::uint8_t> bytes, const std::string& name);
}
