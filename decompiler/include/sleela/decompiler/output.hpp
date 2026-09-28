#pragma once
#include "sleela/decompiler/decompiler.hpp"
#include <string>
#include <vector>

namespace sleela::decompiler {

enum class SourceLanguage { Java, Sleela, C, CPlusPlus };

struct DecompilationOptions {
    SourceLanguage language{SourceLanguage::CPlusPlus};
    std::string source_name{"decompiled"};
};

class SourceEmitter {
public:
    static std::string language_name(SourceLanguage);
    static std::string extension(SourceLanguage);
    static std::string emit(const Artifact&, const std::vector<Instruction>&,
                            const ControlFlowGraph&, const std::vector<Symbol>&,
                            const DecompilationOptions&);
};

} // namespace sleela::decompiler
