#pragma once
#include "decompiler.hpp"
#include <vector>
namespace sleela::decompiler {
struct ElfAnalysis {
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
    std::vector<Import> imports;
    std::vector<Export> exports;
    std::vector<Relocation> relocations;
    LibraryMetadata library;
    KernelModuleMetadata kernel_module;
    bool valid{};
};
ElfAnalysis analyze_elf(std::span<const std::uint8_t> bytes, const std::string& name);
}
