#pragma once
#include "decompiler.hpp"
#include <vector>
namespace sleela::decompiler {
// Static PE/COFF evidence. The image is never mapped or executed; every record is
// derived from on-disk header and directory bytes and carries file-offset provenance.
struct PeAnalysis {
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
    std::vector<Import> imports;
    std::vector<Export> exports;
    std::vector<Relocation> relocations;
    LibraryMetadata library;
    Architecture architecture{Architecture::Unknown};
    ArtifactClass artifact_class{ArtifactClass::Unknown};
    std::uint64_t image_base{};
    std::uint64_t entry_point{};
    bool is_driver{};
    bool valid{};
};
PeAnalysis analyze_pe(std::span<const std::uint8_t> bytes, const std::string& name);
}
