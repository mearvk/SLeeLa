#pragma once
#include "sleela/decompiler/decompiler.hpp"
#include <string>
#include <vector>

namespace sleela::decompiler {

enum class SourceLanguage { Java, Sleela, C, CPlusPlus };

struct DecompilationOptions {
    SourceLanguage language{SourceLanguage::CPlusPlus};
    std::string source_name{"decompiled"};
    bool emit_instruction_evidence{true};   // include the per-function rendered-instruction table
    bool emit_cfg_summary{true};            // include the control-flow summary block
};

class SourceEmitter {
public:
    static std::string language_name(SourceLanguage);
    static std::string extension(SourceLanguage);
    // Full translation-unit (source) rendering.
    static std::string emit(const Artifact&, const std::vector<Instruction>&,
                            const ControlFlowGraph&, const std::vector<Symbol>&,
                            const DecompilationOptions&);
    // Companion header (C/C++): include guard + documented prototypes for recovered
    // functions. For Java/Sleela, returns an interface-style declaration surface.
    static std::string emit_header(const Artifact&, const std::vector<Symbol>&,
                                   const DecompilationOptions&);
    static std::string header_extension(SourceLanguage);
};

} // namespace sleela::decompiler
